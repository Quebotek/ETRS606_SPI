from flask import Flask, request, jsonify
import numpy as np
import joblib
from tensorflow.keras.models import load_model
from datetime import datetime
import os

app = Flask(__name__)

print("🧠 Chargement du modèle TensorFlow en mémoire...")
# Désactiver les logs TF inutiles
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3' 
model = load_model('/app/modele_meteo_v2.h5')
scaler = joblib.load('/app/scaler_meteo_v2.pkl')
CLASSES = {0: "Dégagé", 1: "Nuageux", 2: "Pluie", 3: "Brouillard", 4: "Extrême"}
print("✅ IA prête à recevoir des requêtes !")

@app.route('/predict', methods=['POST'])
def predict():
    try:
        # On récupère les données envoyées par les autres scripts
        data = request.json
        temp = float(data['temp'])
        hum = float(data['humidity'])
        pres = float(data['pressure'])

        # Calcul du temps
        heure = datetime.now().hour
        heure_sin = np.sin(2 * np.pi * heure / 24.0)
        heure_cos = np.cos(2 * np.pi * heure / 24.0)

        # Prédiction
        X = np.array([[temp, hum, pres, heure_sin, heure_cos]])
        X_scaled = scaler.transform(X)
        pred_idx = np.argmax(model.predict(X_scaled, verbose=0))

        # On renvoie la réponse au format JSON
        return jsonify({"status": "success", "prediction": CLASSES[pred_idx]})

    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 400

if __name__ == '__main__':
    # Le serveur écoute sur le port 5000 à l'intérieur du réseau Docker
    app.run(host='0.0.0.0', port=5000)
