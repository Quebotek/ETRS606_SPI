from flask import Flask, request, jsonify
import numpy as np
import joblib
from datetime import datetime
import os

# Import de TFLite
try:
    import tflite_runtime.interpreter as tflite
except ImportError:
    import tensorflow.lite as tflite

app = Flask(__name__)

print("🧠 Chargement du modèle TFLite (7 variables) en mémoire...")

# Noms de vos fichiers (Ajustez si nécessaire)
MODEL_PATH = 'modele_meteo_stm32.tflite'
SCALER_PATH = 'scaler_meteo_stm32.pkl'

# 1. Charger l'interpréteur TFLite
interpreter = tflite.Interpreter(model_path=MODEL_PATH)
interpreter.allocate_tensors()

input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

# 2. Charger le scaler
scaler = joblib.load(SCALER_PATH)

CLASSES = {0: "Dégagé", 1: "Nuageux", 2: "Pluie", 3: "Brouillard", 4: "Extrême"}

print("✅ IA prête à recevoir des requêtes (Format 7 variables) !")

@app.route('/predict', methods=['POST'])
def predict():
    try:
        # 1. Récupération des 3 capteurs bruts envoyés par MQTT
        data = request.json
        temp = float(data['temp'])
        hum = float(data['humidity'])
        pres = float(data['pressure'])

        dwpt_est = temp - ((100.0 - hum) / 5.0)
        # 2. Calcul du temps cyclique (Heure et Mois)
        maintenant = datetime.now()
        heure = maintenant.hour
        mois = maintenant.month

        heure_sin = np.sin(2 * np.pi * heure / 24.0)
        heure_cos = np.cos(2 * np.pi * heure / 24.0)
        mois_sin = np.sin(2 * np.pi * mois / 12.0)
        mois_cos = np.cos(2 * np.pi * mois / 12.0)

        # 3. Création du tableau EXACTEMENT à 7 variables
        # Ordre : [temp, rhum, pres, heure_sin, heure_cos, mois_sin, mois_cos]
        X = np.array([[temp, dwpt_est, hum, pres, heure_sin, heure_cos, mois_sin, mois_cos]])

        # 4. Normalisation avec le Scaler ET conversion pour TFLite (float32)
        X_scaled = scaler.transform(X).astype(np.float32)

        # 5. Injection et prédiction
        interpreter.set_tensor(input_details[0]['index'], X_scaled)
        interpreter.invoke()

        output_data = interpreter.get_tensor(output_details[0]['index'])
        pred_idx = np.argmax(output_data)

        return jsonify({"status": "success", "prediction": CLASSES[int(pred_idx)]})

    except Exception as e:
        # Affichage rouge en cas de crash
        print(f"🔥 ERREUR DANS L'IA : {e}", flush=True)
        return jsonify({"status": "error", "message": str(e)}), 400

if __name__ == '__main__':
    # Cette option permet d'ignorer les erreurs "Host not trusted"
    from werkzeug.serving import run_simple
    run_simple('0.0.0.0', 5000, app)
