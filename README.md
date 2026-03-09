# ETRS606 : IA Embarquée (Edge AI) - Travaux Pratiques et Projets

## 📖 Description du Module
[cite_start]Ce dépôt regroupe les travaux pratiques et projets réalisés dans le cadre du module **ETRS606 : IA Embarquée**[cite: 1, 46, 197]. [cite_start]L'objectif de ce cours est de concevoir des systèmes embarqués d'interface capteurs sous le paradigme des circuits neuromorphiques, à la convergence de la microélectronique et de l'intelligence artificielle[cite: 279]. 

[cite_start]Le module explore les compromis nécessaires pour déployer des réseaux de neurones sur des architectures matérielles contraintes (mémoire, complexité de calcul, précision, consommation énergétique)[cite: 282, 358].

## 🛠️ Matériel Utilisé
Les déploiements et tests sont réalisés sur des cartes de développement de la famille STM32 :
* [cite_start]**NUCLEO-N657X0** : Basée sur un ARM Cortex-M33 (jusqu'à 160 MHz) avec 320 ko de RAM et 512 ko de Flash[cite: 49, 373]. [cite_start]Elle intègre un accélérateur matériel neuromorphique (NPU Neural-ART à 600 GOPS) optimisé pour l'inférence de modèles CNN[cite: 50, 375].
* [cite_start]**NUCLEO-F446RE** : Basée sur un Cortex-M4 (180 MHz) avec 128 ko de RAM et 512 ko de Flash[cite: 367].
* [cite_start]**Shield X-NUCLEO-IKS01A3** : Carte d'interface regroupant plusieurs capteurs MEMS (accélération, champ magnétique, température, humidité, pression) communicant via un bus I2C[cite: 51, 52].

---

## 🚀 TP1 : Problème MNIST (Apprentissage Automatique)
[cite_start]Ce TP se concentre sur la classification d'images de chiffres manuscrits (0 à 9) à partir du jeu de données classique MNIST[cite: 4, 5]. 
* [cite_start]**Objectif** : Implémenter un réseau de neurones dense (Multi-Layer Perceptron / MLP) avec une couche d'entrée de 784 neurones (images aplaties de 28x28 pixels) et une sortie de 10 neurones (pour les 10 classes)[cite: 14, 15, 17, 18].
* **Expérimentations** : 
  * [cite_start]Comparaison de différentes fonctions d'activation : *softmax*, *ReLU*, *tanh*, *sigmoid*[cite: 30, 31, 33, 34].
  * [cite_start]Évaluation de différents optimiseurs : *SGD* (classique), *Adam* (moderne et rapide), *RMSprop* (taux adaptatif), *Adagrad*[cite: 39, 40].
  * [cite_start]Choix de la fonction coût adaptée (*categorical_crossentropy* ou *sparse_categorical_crossentropy*) en justifiant les compromis entre précision, temps d'apprentissage et taille du modèle[cite: 29, 37, 43, 44].

---

## 🔌 TP2 : Interface Capteurs & Réseau sur STM32
[cite_start]Ce second TP est orienté programmation embarquée bas niveau et intégration système[cite: 73, 75].
* [cite_start]**Partie 1 - LED Blink** : Prise en main de STM32 Cube IDE avec la réalisation d'un chenillard de 3 secondes sur les LEDs (Rouge, Vert, Bleu) et affichage des logs sur la console[cite: 67, 69, 71].
* [cite_start]**Partie 2 - Interface Capteurs (I2C)** : Intégration des drivers matériels STMicroelectronics pour lire les valeurs des capteurs (LSM6DSO, LIS2MDL, LIS2DW12, HTS221, LPS22HH, STTS751) [cite: 73, 76-82]. [cite_start]Implémentation des fonctions bas niveau (`platform_read`, `platform_write`) via HAL I2C[cite: 106, 113]. [cite_start]Les LEDs servent de repère visuel de l'état (Rouge = occupé, Vert = disponible)[cite: 85, 87, 88].
* [cite_start]**Partie 3 - Réseau Ethernet** : Mise en place du système d'exploitation temps réel FreeRTOS et de la pile réseau LWIP (DHCP + ICMP) pour exécuter un ping vers `google.com` via la prise RJ45 de la carte [cite: 139-142].

---

## 🧠 Projets : Déploiement IA Embarquée (Edge AI)
[cite_start]Le cours intègre deux projets d'application permettant de comprendre la différence radicale entre l'entraînement sur PC et l'inférence sur microcontrôleur[cite: 421, 422, 455].

1. **Projet Sin(x)** : 
   * [cite_start]**Concept** : Approximer la fonction $sin(x)$ avec un MLP (3 couches cachées de 16 neurones avec activation ReLU)[cite: 457, 462, 464, 466].
   * [cite_start]**Analyse** : Comparaison entre la méthode classique optimisée matériellement et l'inférence IA[cite: 489, 490]. [cite_start]L'approche classique consomme moins (environ 17 mA) et s'exécute plus vite que l'IA (qui peut monter à 36 mA)[cite: 492, 493, 496].
2. **Projet Meteostat** : 
   * [cite_start]**Concept** : Prédire un événement météorologique à partir de 3 paramètres (Température, Humidité, Pression) [cite: 508, 511-515].
   * [cite_start]**Analyse** : Met en avant l'importance du prétraitement des données[cite: 509]. [cite_start]Contrairement au sinus, la relation entre ces variables météo n'est pas analytique, ce qui justifie ici l'utilisation de l'IA[cite: 543].

## 🌿 Sobriété Numérique et Conclusion
[cite_start]L'un des enseignements majeurs de ce module est l'approche critique vis-à-vis de l'IA[cite: 637]. [cite_start]L'utilisation d'un modèle neuronal sur un système embarqué a un coût important (énergie, latence, mémoire) [cite: 570-572, 638]. [cite_start]Ce dépôt met en évidence la nécessité d'optimiser les modèles (quantification int8, pruning) et souligne que l'IA n'est pertinente que si elle apporte un bénéfice clair face à une solution déterministe simple[cite: 411, 413, 622, 623, 639].
