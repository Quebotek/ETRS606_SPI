%matplotlib inline
import tensorflow as tf
from tensorflow.keras.utils import plot_model
from callbackPlots import AccuracyPlotCallback, count_neurons_and_synapses
import numpy as np
import random

# ==========================================
# --- 1. CONFIGURATION GLOBALE DU TEST ---
# ==========================================

# 1. Choix de l'architecture (Fonction d'activation)
# Options : 'softmax_only', 'relu', 'tanh', 'sigmoid'
ACTIVATION_CHOICE = 'tanh' 

# 2. Choix de l'algorithme d'optimisation
# Options : 'sgd', 'adam', 'rmsprop', 'adagrad'
OPTIMIZER_CHOICE = 'adagrad' 

# Fixer les graines pour garantir que les mêmes conditions donnent les mêmes résultats
np.random.seed(42)
tf.random.set_seed(42)
random.seed(42)

# ==========================================
# --- 2. PRÉPARATION DES DONNÉES ---
# ==========================================
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

# ==========================================
# --- 3. CONSTRUCTION DU MODÈLE ---
# ==========================================
print(f"Construction du modèle : Architecture [{ACTIVATION_CHOICE.upper()}] | Optimiseur [{OPTIMIZER_CHOICE.upper()}]")

if ACTIVATION_CHOICE == 'softmax_only':
    model = tf.keras.Sequential([
        tf.keras.layers.Dense(10, activation='softmax', input_shape=(784,))
    ])
elif ACTIVATION_CHOICE == 'relu':
    model = tf.keras.Sequential([
        tf.keras.layers.Dense(128, activation='relu', input_shape=(784,)),
        tf.keras.layers.Dense(64, activation='relu'),
        tf.keras.layers.Dense(10, activation='softmax')
    ])
elif ACTIVATION_CHOICE == 'tanh':
    model = tf.keras.Sequential([
        tf.keras.layers.Dense(64, activation='tanh', input_shape=(784,)),
        tf.keras.layers.Dense(10, activation='softmax')
    ])
elif ACTIVATION_CHOICE == 'sigmoid':
    model = tf.keras.Sequential([
        tf.keras.layers.Dense(64, activation='sigmoid', input_shape=(784,)),
        tf.keras.layers.Dense(10, activation='softmax')
    ])
else:
    raise ValueError("Choix d'activation non reconnu.")

# ==========================================
# --- 4. COMPILATION ET ENTRAÎNEMENT ---
# ==========================================
model.compile(optimizer=OPTIMIZER_CHOICE,
              loss='categorical_crossentropy',
              metrics=['accuracy'])

plot_callback = AccuracyPlotCallback()

print("Début de l'entraînement...")
history = model.fit(x_train, y_train, 
                    epochs=5, 
                    batch_size=32, 
                    validation_data=(x_test, y_test), 
                    callbacks=[plot_callback])

# ==========================================
# --- 5. ÉVALUATION ET EXPORT ---
# ==========================================
test_loss, test_acc = model.evaluate(x_test, y_test, verbose=0)
print(f"\n--- RÉSULTATS FINAUX ---")
print(f"Architecture : {ACTIVATION_CHOICE.upper()}")
print(f"Optimiseur   : {OPTIMIZER_CHOICE.upper()}")
print(f"Test accuracy: {test_acc:.4f}")

# Le nom du fichier inclut maintenant les deux variables pour un tri facile
filename = f"model_architecture_{ACTIVATION_CHOICE}_{OPTIMIZER_CHOICE}.png"
plot_model(model, to_file=filename, show_shapes=True, show_layer_names=True, dpi=300)
print(f"Architecture sauvegardée sous : {filename}")

count_neurons_and_synapses(model)