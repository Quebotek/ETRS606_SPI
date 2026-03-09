%matplotlib inline
import tensorflow as tf
from tensorflow.keras.utils import plot_model
from callbackPlots import AccuracyPlotCallback, count_neurons_and_synapses
import numpy as np
import random

# --- 1. CONFIGURATION ---
# Changez cette variable pour tester les différentes questions du TP :
# Options disponibles : 'softmax_only' (1.a), 'relu' (1.b)
ACTIVATION_CHOICE = 'relu' 

# Fixer les graines pour la reproductibilité
np.random.seed(42)
tf.random.set_seed(42)
random.seed(42)

# --- 2. PRÉPARATION DES DONNÉES ---
print("Chargement et préparation des données...")
(x_train, y_train), (x_test, y_test) = tf.keras.datasets.mnist.load_data()

# Normalisation (0 à 1)
x_train, x_test = x_train / 255.0, x_test / 255.0

# Aplatissement (Flattening) : 28x28 -> 784
x_train = x_train.reshape(-1, 784)
x_test = x_test.reshape(-1, 784)

# One-hot encoding pour utiliser categorical_crossentropy
y_train = tf.keras.utils.to_categorical(y_train, 10)
y_test = tf.keras.utils.to_categorical(y_test, 10)

# --- 3. CONSTRUCTION DU MODÈLE ---
print(f"Construction du modèle avec la configuration : {ACTIVATION_CHOICE}")

if ACTIVATION_CHOICE == 'softmax_only':
    # 1.a : Couche d'entrée et sortie directe (Softmax)
    model = tf.keras.Sequential([
        tf.keras.layers.Dense(10, activation='softmax', input_shape=(784,))
    ])

elif ACTIVATION_CHOICE == 'relu':
    # 1.b : 2 couches cachées avec ReLU
    model = tf.keras.Sequential([
        tf.keras.layers.Dense(128, activation='relu', input_shape=(784,)),
        tf.keras.layers.Dense(64, activation='relu'),
        tf.keras.layers.Dense(10, activation='softmax')
    ])

else:
    raise ValueError("Choix d'activation non reconnu.")

# --- 4. COMPILATION ET ENTRAÎNEMENT ---
model.compile(optimizer='adam',
              loss='categorical_crossentropy',
              metrics=['accuracy'])

plot_callback = AccuracyPlotCallback()

print("Début de l'entraînement...")
history = model.fit(x_train, y_train, 
                    epochs=5, 
                    batch_size=32, 
                    validation_data=(x_test, y_test), 
                    callbacks=[plot_callback])

# --- 5. ÉVALUATION ET EXPORT ---
test_loss, test_acc = model.evaluate(x_test, y_test, verbose=0)
print(f"\n--- RÉSULTATS POUR {ACTIVATION_CHOICE.upper()} ---")
print(f"Test accuracy : {test_acc:.4f}")

# Exporter l'architecture et compter les paramètres
filename = f"model_architecture_{ACTIVATION_CHOICE}.png"
plot_model(model, to_file=filename, show_shapes=True, show_layer_names=True, dpi=300)
print(f"Architecture sauvegardée sous : {filename}")

count_neurons_and_synapses(model)