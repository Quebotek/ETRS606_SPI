# 🌤️ IoT Weather Station & AI Prediction (Zabbix + MQTT + TensorFlow)

Ce projet est une infrastructure complète de supervision météorologique IoT intégrant de l'Intelligence Artificielle. Il permet de remonter les données de capteurs physiques (Microcontrôleur STM32) et de stations virtuelles (API Open-Meteo) vers un serveur Zabbix, tout en prédisant l'état de la météo en temps réel grâce à un modèle de Machine Learning.

## 🏗️ Architecture du Projet

Le projet repose sur une architecture moderne orientée **Microservices** via Docker Compose :

* **Mosquitto (Broker MQTT) :** Réceptionne les données brutes envoyées par la carte STM32 (Température, Humidité, Pression).
* **MQTT Bridge (Python) :** Écoute les topics MQTT, formate les données et les envoie à Zabbix via `zabbix_sender`.
* **Météo France API (Python) :** Scrapeur qui interroge l'API Open-Meteo pour générer des stations virtuelles (Lyon, Marseille, Bordeaux, etc.) et les intègre dans Zabbix.
* **AI Brain (Flask + TensorFlow) :** Un microservice dédié à l'IA. Il reçoit les données environnementales, calcule l'encodage temporel cyclique (heure), et utilise un modèle de réseau de neurones (MLP) entraîné sur des données historiques pour prédire la condition météo (Dégagé, Nuageux, Pluie, etc.).

## 🚀 Fonctionnalités

* Supervision en temps réel sur carte **Geomap Zabbix**.
* Ingestion de données hybrides (Physiques via MQTT + Virtuelles via API REST).
* Déploiement conteneurisé (Docker).
* Intelligence Artificielle découplée ("AI as a Service") pour optimiser la consommation de RAM.
* Ingénierie des caractéristiques temporelles (Feature Engineering : encodage cyclique de l'heure) pour améliorer la précision de l'IA de ~5%.

## 📂 Structure du Dépôt

```text
meteo-stack/
├── docker-compose.yml       # Déploiement de toute l'infrastructure
├── mqtt_to_zabbix.py        # Script pont MQTT -> Zabbix -> IA
├── meteo_france.py          # Script Villes Virtuelles -> Zabbix -> IA
├── ai_brain.py              # Serveur Flask hébergeant le modèle TensorFlow
├── modele_meteo_v2.h5       # Modèle IA pré-entraîné
└── scaler_meteo_v2.pkl      # Scaler pour la normalisation des données IA
