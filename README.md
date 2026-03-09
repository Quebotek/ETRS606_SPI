# ETRS606 : IA Embarquée (Edge AI) - Travaux Pratiques et Projets

## 📖 Description du Module
Ce dépôt regroupe les travaux pratiques et projets réalisés dans le cadre du module **ETRS606 : IA Embarquée**. L'objectif de ce cours est de concevoir des systèmes embarqués d'interface capteurs sous le paradigme des circuits neuromorphiques, à la convergence de la microélectronique et de l'intelligence artificielle. 

Le module explore les compromis nécessaires pour déployer des réseaux de neurones sur des architectures matérielles contraintes (mémoire, complexité de calcul, précision, consommation énergétique).

## 🛠️ Matériel Utilisé
Les déploiements et tests sont réalisés sur des cartes de développement de la famille STM32 :
* **NUCLEO-N657X0** : Basée sur un ARM Cortex-M33 (jusqu'à 160 MHz) avec 320 ko de RAM et 512 ko de Flash. Elle intègre un accélérateur matériel neuromorphique (NPU Neural-ART à 600 GOPS) optimisé pour l'inférence de modèles CNN.
* **NUCLEO-F446RE** : Basée sur un Cortex-M4 (180 MHz) avec 128 ko de RAM et 512 ko de Flash.
* **Shield X-NUCLEO-IKS01A3** : Carte d'interface regroupant plusieurs capteurs MEMS (accélération, champ magnétique, température, humidité, pression) communicant via un bus I2C.

---

## 🚀 TP1 : Problème MNIST 
Ce TP se concentre sur la classification d'images de chiffres manuscrits (0 à 9) à partir du jeu de données classique MNIST. 
* **Objectif** : Implémenter un réseau de neurones dense (Multi-Layer Perceptron / MLP) avec une couche d'entrée de 784 neurones (images aplaties de 28x28 pixels) et une sortie de 10 neurones (pour les 10 classes).
* **Expérimentations** : 
Comparaison de différentes fonctions d'activation : *softmax*, *ReLU*, *tanh*, *sigmoid*.
Évaluation de différents optimiseurs : *SGD* (classique), *Adam* (moderne et rapide), *RMSprop* (taux adaptatif), *Adagrad*.
Choix de la fonction coût adaptée (*categorical_crossentropy* ou *sparse_categorical_crossentropy*) en justifiant les compromis entre précision, temps d'apprentissage et taille du modèle.

---

## 🔌 TP2 : Interface Capteurs & Réseau sur STM32
Ce second TP est orienté programmation embarquée bas niveau et intégration système.
* **Partie 1 - LED Blink** : Prise en main de STM32 Cube IDE avec la réalisation d'un chenillard de 3 secondes sur les LEDs (Rouge, Vert, Bleu) et affichage des logs sur la console.
* **Partie 2 - Interface Capteurs (I2C)** : Intégration des drivers matériels STMicroelectronics pour lire les valeurs des capteurs (LSM6DSO, LIS2MDL, LIS2DW12, HTS221, LPS22HH, STTS751). Implémentation des fonctions bas niveau (`platform_read`, `platform_write`) via HAL I2C. Les LEDs servent de repère visuel de l'état (Rouge = occupé, Vert = disponible).
* **Partie 3 - Réseau Ethernet** : Mise en place du système d'exploitation temps réel FreeRTOS et de la pile réseau LWIP (DHCP + ICMP) pour exécuter un ping vers `google.com` via la prise RJ45 de la carte.

---

## 🧠 Projets : Déploiement IA Embarquée (Edge AI)
Le cours intègre deux projets d'application permettant de comprendre la différence radicale entre l'entraînement sur PC et l'inférence sur microcontrôleur.

1. **Projet Sin(x)** : 
* **Concept** : Approximer la fonction $sin(x)$ avec un MLP (3 couches cachées de 16 neurones avec activation ReLU).
* **Analyse** : Comparaison entre la méthode classique optimisée matériellement et l'inférence IA. L'approche classique consomme moins (environ 17 mA) et s'exécute plus vite que l'IA (qui peut monter à 36 mA).
2. **Projet Meteostat** : 
* **Concept** : Prédire un événement météorologique à partir de 3 paramètres (Température, Humidité, Pression).
* **Analyse** : Met en avant l'importance du prétraitement des données. Contrairement au sinus, la relation entre ces variables météo n'est pas analytique, ce qui justifie ici l'utilisation de l'IA.

## 🌿 Conclusion
L'un des enseignements majeurs de ce module est l'approche critique vis-à-vis de l'IA. L'utilisation d'un modèle neuronal sur un système embarqué a un coût important (énergie, latence, mémoire). Ce dépôt met en évidence la nécessité d'optimiser les modèles (quantification int8, pruning) et souligne que l'IA n'est pertinente que si elle apporte un bénéfice clair face à une solution déterministe simple.
