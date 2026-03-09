# TP 1 : Problème MNIST - IA Embarquée

Ce document synthétise les résultats des différentes simulations menées sur le jeu de données MNIST en utilisant des Perceptrons Multicouches (MLP). L'objectif est de comparer les architectures et les hyperparamètres dans une optique de contraintes d'IA embarquée (mémoire, temps de calcul, précision).

---

## 1. Choix de la Fonction d'Activation

**Conditions de test :**
* Optimiseur : `Adam`
* Fonction coût : `categorical_crossentropy`
* Époques : 5
* Batch size : 32

### 1.1 Tableau des Résultats

| Configuration | Architecture (Couches) | Activation Cachée | Nb Synapses (Poids) | Accuracy (Test) |
| :--- | :--- | :--- | :--- | :--- |
| **a. Baseline** | Entrée(784) -> Sortie(10) | N/A (Softmax) | *7 850* | *92.61%* |
| **b. Profond** | Entrée -> Dense(128) -> Dense(64) -> Sortie | `relu` | *109 386* | *97.24%* |
| **c. Tanh** | Entrée -> Dense(64) -> Sortie | `tanh` | *50 890* | *96.99%* |
| **d. Sigmoid** | Entrée -> Dense(64) -> Sortie | `sigmoid` | *50 890* | *96.39%* |

---

### Architectures générées

Voici les graphes des modèles correspondants aux configurations testées :

**Modèle a. Baseline (Softmax direct)**

<img width="608" height="480" alt="image" src="https://github.com/user-attachments/assets/a74fab49-2f59-443a-9348-e30976a7919a" />


**Modèle b. Profond (ReLU, 2 couches cachées)**

<img width="608" height="480" alt="image" src="https://github.com/user-attachments/assets/5aa2c089-ca44-482e-9722-8050318f57e5" />


**Modèle c. Tanh (1 couche cachée)**

<img width="608" height="480" alt="image" src="https://github.com/user-attachments/assets/6f716811-e1df-40ab-a4a6-e3cf6b7cc002" />


**Modèle d. Sigmoid (1 couche cachée)**

<img width="608" height="480" alt="image" src="https://github.com/user-attachments/assets/99877b0a-4e78-4acf-b7c7-14aeda2b1d27" />

