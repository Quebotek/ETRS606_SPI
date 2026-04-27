import numpy as np
import tensorflow as tf
from datetime import datetime
from meteostat import Point, Hourly
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import confusion_matrix, ConfusionMatrixDisplay
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
import joblib
import matplotlib.pyplot as plt

# ==============================================================================
# --- 1. Téléchargement des données ---
# ==============================================================================
print("📥 Téléchargement des données météo...")
start = datetime(2020, 1, 1)
end = datetime(2023, 12, 31)
le_bourget = Point(48.9556, 2.4331)
data = Hourly(le_bourget, start, end).fetch()

# On ne garde QUE ce que la STM32 peut mesurer !
df = data[['temp', 'rhum', 'pres', 'coco']].dropna()

# --- NOUVEAU : Estimation du Point de Rosée (Règle de Lawrence) ---
df['dwpt_est'] = df['temp'] - ((100.0 - df['rhum']) / 5.0)

# ==============================================================================
# --- 2. Regroupement des Classes ---
# ==============================================================================
def regrouper_classes(coco):
    if coco in [1, 2]: return 0             # Dégagé
    elif coco in [3, 4]: return 1           # Nuageux
    elif coco in [7, 8, 9, 17, 18]: return 2 # Pluie
    elif coco in [5, 6]: return 3           # Brouillard
    else: return 4                          # Extrême (Neige/Orage)

df['classe_cible'] = df['coco'].apply(regrouper_classes)

# ==============================================================================
# --- 3. Ajout du Temps (Cyclique) ---
# ==============================================================================
heures = df.index.hour
mois = df.index.month

df['heure_sin'] = np.sin(2 * np.pi * heures / 24.0)
df['heure_cos'] = np.cos(2 * np.pi * heures / 24.0)
df['mois_sin'] = np.sin(2 * np.pi * mois / 12.0)
df['mois_cos'] = np.cos(2 * np.pi * mois / 12.0)

# ==============================================================================
# --- 4. Préparation pour l'IA ---
# ==============================================================================
X = df[['temp', 'dwpt_est', 'rhum', 'pres', 'heure_sin', 'heure_cos', 'mois_sin', 'mois_cos']].values
Y = df['classe_cible'].values

X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.2, random_state=42)

# Normalisation
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# ==============================================================================
# --- 5. Création et Entraînement du Modèle ---
# ==============================================================================
print("🚀 Entraînement de l'IA (8 variables)...")
model = Sequential([
    Dense(32, activation='relu', input_shape=(8,)),
    Dense(16, activation='relu'),
    Dense(5, activation='softmax')
])

model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
model.fit(X_train_scaled, Y_train, epochs=20, batch_size=32, validation_data=(X_test_scaled, Y_test))

# ==============================================================================
# --- 6. Sauvegarde classique ---
# ==============================================================================
model.save('modele_meteo_stm32.keras')
joblib.dump(scaler, 'scaler_meteo_stm32.pkl')

# ==============================================================================
# --- 7. CONVERSION POUR LE NPU (TFLite INT8) ---
# ==============================================================================
print("⚙️ Génération du modèle quantifié (INT8) pour le NPU de la STM32...")

converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]

# Fonction de calibration
def representative_data_gen():
    for i in range(500):
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

# ==============================================================================
# --- 8. MATRICE DE CONFUSION ---
# ==============================================================================
print("📊 Génération de la matrice de confusion...")

# Faire les prédictions
Y_pred_prob = model.predict(X_test_scaled)
Y_pred = np.argmax(Y_pred_prob, axis=1)

# Calculer la matrice
cm = confusion_matrix(Y_test, Y_pred)
noms_classes = ['Dégagé (0)', 'Nuageux (1)', 'Pluie (2)', 'Brouillard (3)', 'Extrême (4)']

# Créer l'affichage
fig, ax = plt.subplots(figsize=(8, 6))
disp = ConfusionMatrixDisplay(confusion_matrix=cm, display_labels=noms_classes)
disp.plot(cmap=plt.cm.Blues, ax=ax, xticks_rotation=45)

plt.title("Matrice de Confusion du Modèle Météo")
plt.tight_layout()

# Sauvegarder et afficher
plt.savefig("matrice_confusion.png")
print("✅ Matrice de confusion sauvegardée sous 'matrice_confusion.png'")
plt.show()
