🌦️ STM32 Edge AI - Prédiction Météo (TFLite INT8)
Ce projet implémente un modèle de Machine Learning léger conçu pour être embarqué sur un microcontrôleur STM32. Il utilise des données météorologiques de base (Température, Humidité, Pression) pour prédire la condition météo actuelle, avec une quantification totale en INT8 via TensorFlow Lite pour optimiser l'inférence sur le NPU de la carte.

📖 Présentation du Projet
L'objectif est de fournir une estimation météorologique locale sans nécessiter de capteurs complexes (comme un pluviomètre ou un luxmètre) ou de connexion internet constante.

Le script Python réalise l'ensemble du pipeline :

Extraction des données historiques (Le Bourget) via l'API meteostat.

Ingénierie des caractéristiques (Feature Engineering) : Calcul du point de rosée estimé et encodage cyclique du temps (heure/mois).

Entraînement d'un réseau de neurones avec Keras/TensorFlow.

Quantification post-entraînement (INT8) pour générer un fichier .tflite compatible avec STM32Cube.AI.

Évaluation du modèle via une matrice de confusion.

🚀 Fonctionnalités
Classes prédites : Dégagé (0), Nuageux (1), Pluie (2), Brouillard (3), Extrême (4).

Entrées (Features) : Température, Point de rosée estimé, Humidité, Pression, Heure (sin/cos), Mois (sin/cos).

Optimisation Edge AI : Modèle converti au format TensorFlow Lite avec quantification entière (INT8) de bout en bout grâce à un jeu de données représentatif.

🛠️ Prérequis et Installation
Assurez-vous d'avoir Python installé, puis installez les dépendances requises :

Bash
pip install numpy tensorflow meteostat pandas scikit-learn matplotlib joblib
📊 Analyse des Performances
Bien que la précision globale stricte (accuracy) du modèle soit d'environ 65%, ce chiffre cache une excellente fiabilité en conditions réelles (usage "station météo grand public").

Ce que nous apprend la Matrice de Confusion :
Précision "Temps Sec" (95%) : La majorité des erreurs du modèle consiste à confondre un ciel "Dégagé" avec un ciel "Nuageux". En situation réelle, lorsque la carte annonce "Dégagé", il y a plus de 95% de chances qu'il ne pleuve pas (bonnes prédictions de temps sec par rapport aux vraies intempéries).

Détection de la Pluie : Le modèle détecte bien les chutes de pression, mais a tendance à classer la pluie dans la catégorie "Nuageux". Sans pluviomètre physique, il est difficile pour l'IA de différencier un nuage dense d'une averse.

Événements Rares : Les conditions extrêmes (orages, neige) et le brouillard manquent d'exemples dans le jeu de données historique pour être détectés avec une grande fiabilité.

📂 Fichiers Générés par le Script
Après exécution, le script produit les fichiers suivants :

modele_meteo_stm32.keras : Le modèle Keras original (pour référence).

scaler_meteo_stm32.pkl : Le scaler pour normaliser les données sur la STM32 (à retranscrire en C).

modele_meteo_INT8.tflite : Le modèle final quantifié à importer dans STM32CubeMX.

matrice_confusion.png : La visualisation des performances du modèle.
