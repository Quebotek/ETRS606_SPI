# 🌦️ STM32 Edge AI - Prédiction Météo (TFLite INT8)

Ce projet implémente un modèle de Machine Learning léger conçu pour être embarqué sur un microcontrôleur STM32. Il utilise des données météorologiques de base (Température, Humidité, Pression) pour prédire la condition météo actuelle, avec une quantification totale en INT8 via TensorFlow Lite pour optimiser l'inférence sur le NPU de la carte.

## 📖 Présentation du Projet

L'objectif est de fournir une estimation météorologique locale sans nécessiter de capteurs complexes (comme un pluviomètre ou un luxmètre) ou de connexion internet constante. 

Le script Python réalise l'ensemble du pipeline :
1. **Extraction des données** historiques (Le Bourget) via l'API `meteostat`.
2. **Ingénierie des caractéristiques** (Feature Engineering) : Calcul du point de rosée estimé et encodage cyclique du temps (heure/mois).
3. **Entraînement d'un réseau de neurones** avec Keras/TensorFlow.
4. **Quantification post-entraînement (INT8)** pour générer un fichier `.tflite` compatible avec STM32Cube.AI.
5. **Évaluation du modèle** via une matrice de confusion.

## 🚀 Fonctionnalités

* **Classes prédites :** Dégagé (0), Nuageux (1), Pluie (2), Brouillard (3), Extrême (4).
* **Entrées (Features) :** Température, Point de rosée estimé, Humidité, Pression, Heure (sin/cos), Mois (sin/cos).
* **Optimisation Edge AI :** Modèle converti au format TensorFlow Lite avec quantification entière (INT8) de bout en bout grâce à un jeu de données représentatif.

## 🛠️ Prérequis et Installation

Assurez-vous d'avoir Python installé, puis installez les dépendances requises :

```bash
pip install numpy tensorflow meteostat pandas scikit-learn matplotlib joblib
