import numpy as np
import tensorflow as tf
from datetime import datetime
from meteostat import Point, Hourly
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
import joblib

# --- 1. Téléchargement des données ---
print("📥 Téléchargement des données météo...")
start = datetime(2020, 1, 1)
end = datetime(2023, 12, 31)
le_bourget = Point(48.9556, 2.4331)
data = Hourly(le_bourget, start, end).fetch()

# On ne garde QUE ce que la STM32 peut mesurer !
df = data[['temp', 'rhum', 'pres', 'coco']].dropna()

# --- NOUVEAU : Estimation du Point de Rosée (Règle de Lawrence) ---
df['dwpt_est'] = df['temp'] - ((100.0 - df['rhum']) / 5.0)

# --- 2. Regroupement des Classes ---
def regrouper_classes(coco):
    if coco in [1, 2]: return 0             # Dégagé
    elif coco in [3, 4]: return 1           # Nuageux
    elif coco in [7, 8, 9, 17, 18]: return 2 # Pluie
    elif coco in [5, 6]: return 3           # Brouillard
    else: return 4                          # Extrême (Neige/Orage)
df['classe_cible'] = df['coco'].apply(regrouper_classes)

# --- 3. Ajout du Temps (Cyclique) ---
heures = df.index.hour
mois = df.index.month

df['heure_sin'] = np.sin(2 * np.pi * heures / 24.0)
df['heure_cos'] = np.cos(2 * np.pi * heures / 24.0)
df['mois_sin'] = np.sin(2 * np.pi * mois / 12.0)
df['mois_cos'] = np.cos(2 * np.pi * mois / 12.0)

# --- 4. Préparation pour l'IA ---
X = df[['temp', 'dwpt_est', 'rhum', 'pres', 'heure_sin', 'heure_cos', 'mois_sin', 'mois_cos']].values
Y = df['classe_cible'].values

X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.2, random_state=42)

# Normalisation
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# --- 5. Création et Entraînement du Modèle ---
print("🚀 Entraînement de l'IA (8 variables)...")
model = Sequential([
    Dense(32, activation='relu', input_shape=(8,)),
    Dense(16, activation='relu'),
    Dense(5, activation='softmax')
])

model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
model.fit(X_train_scaled, Y_train, epochs=20, batch_size=32, validation_data=(X_test_scaled, Y_test))

# --- 6. Sauvegarde classique ---
model.save('modele_meteo_stm32.keras')
joblib.dump(scaler, 'scaler_meteo_stm32.pkl')

# ==============================================================================
# --- 7. NOUVEAU : CONVERSION POUR LE NPU (TFLite INT8) ---
# ==============================================================================
print("⚙️ Génération du modèle quantifié (INT8) pour le NPU de la STM32...")

converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]

# Fonction de calibration (Utilise les données NORMALISÉES car c'est ce que la STM32 va envoyer)
def representative_data_gen():
    # On prend 500 exemples de ton dataset d'entraînement pour calibrer les poids entiers
    for i in range(500):
        # Format attendu par Keras/TFLite : tableau 2D (1, 8)
        data = np.array([X_train_scaled[i]], dtype=np.float32)
        yield [data]

converter.representative_dataset = representative_data_gen

# On force strictement l'utilisation de INT8 de bout en bout
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
converter.inference_input_type = tf.int8
converter.inference_output_type = tf.int8

# Conversion
tflite_quant_model = converter.convert()

# Sauvegarde
with open('modele_meteo_INT8.tflite', 'wb') as f:
    f.write(tflite_quant_model)

print("✅ Terminé ! Le fichier 'modele_meteo_INT8.tflite' est prêt pour CubeMX !")
