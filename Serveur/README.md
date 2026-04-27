# Meteo Stack — Partie Serveur

Stack Docker de collecte, traitement et supervision météo, combinant des données issues de capteurs physiques (STM32N6) et de l'API Open-Meteo, avec inférence IA et envoi vers Zabbix.

---

## Architecture

```
STM32N6 (capteurs)         Open-Meteo API
       │                         │
       │ MQTT (meteo/data)        │ HTTP
       ▼                         ▼
  mqtt_broker  ◄────────  meteo_france_api
  (Mosquitto)                    │
       │                         │ HTTP POST /predict
       ▼                         ▼
  mqtt_bridge ──────────►  ai_brain (TFLite)
       │                         │
       │ zabbix_sender            │ prediction météo
       ▼                         ▼
   Zabbix (VM distante)    réponse JSON
```

Deux sources de données alimentent le pipeline en parallèle :
- La **carte STM32N6** publie les relevés capteurs en temps réel via MQTT
- Le service **meteo_france_api** interroge Open-Meteo toutes les heures pour 5 villes françaises

Les deux flux passent par **ai_brain** pour obtenir une prédiction météo, puis sont envoyés vers **Zabbix** via `zabbix_sender`.

---

## Conteneurs

| Conteneur | Image | Rôle |
|---|---|---|
| `mqtt_broker` | eclipse-mosquitto:latest | Bus de messages MQTT (port 1883) |
| `mqtt_bridge` | python:3.9-slim | Relais MQTT → Zabbix + appel IA |
| `meteo_france_api` | python:3.9-slim | Collecte Open-Meteo → Zabbix + appel IA |
| `ai_brain` | python:3.9-slim | Serveur Flask d'inférence TFLite (port 5000) |

---

## Prérequis

- Docker & Docker Compose
- Zabbix Server accessible sur le réseau (items `meteo.temp`, `meteo.humidity`, `meteo.pressure`, `meteo.prediction` créés)
- Fichiers modèle présents à la racine du projet :
  - `modele_meteo_stm32.tflite`
  - `scaler_meteo_stm32.pkl`

---

## Structure du projet

```
meteo-stack/
├── docker-compose.yml
├── ai_brain.py               # Serveur Flask TFLite
├── meteo_france.py           # Collecte Open-Meteo (5 villes)
├── mqtt_to_zabbix.py         # Bridge MQTT → IA → Zabbix
├── modele_meteo_stm32.tflite # Modèle TFLite (STM32N6)
├── modele_meteo_stm32.keras  # Modèle Keras (entraînement)
├── scaler_meteo_stm32.pkl    # Scaler sklearn
└── mosquitto/
    ├── config/
    │   ├── mosquitto.conf
    │   └── password.txt      # ⚠️ Non versionné
    ├── data/
    └── log/
```

---

## Démarrage

```bash
git clone <repo>
cd meteo-stack
docker compose up -d
```

Les dépendances Python sont installées automatiquement au démarrage de chaque conteneur.

---

## Modèle IA

Le modèle TFLite prédit la condition météo à partir de 8 variables :

| Variable | Source |
|---|---|
| Température (°C) | Capteur / API |
| Point de rosée estimé | Calculé depuis temp + humidité |
| Humidité relative (%) | Capteur / API |
| Pression atmosphérique (hPa) | Capteur / API |
| heure_sin / heure_cos | Temps cyclique (heure) |
| mois_sin / mois_cos | Temps cyclique (mois) |

Classes de sortie : `Dégagé`, `Nuageux`, `Pluie`, `Brouillard`, `Extrême`

---

## Items Zabbix requis

À créer sur chaque host Zabbix (`STM32-Station-Meteo`, `Meteo-Lyon`, etc.) :

| Clé | Type | Description |
|---|---|---|
| `meteo.temp` | Zabbix trapper | Température (°C) |
| `meteo.humidity` | Zabbix trapper | Humidité (%) |
| `meteo.pressure` | Zabbix trapper | Pression (hPa) |
| `meteo.prediction` | Zabbix trapper | Prédiction IA |

---

## Configuration MQTT

Le broker Mosquitto utilise une authentification par mot de passe.  
Le fichier `mosquitto/config/password.txt` n'est pas versionné.

Pour régénérer les credentials :

```bash
docker exec mqtt_broker mosquitto_passwd -c /mosquitto/config/password.txt albert
```

Les credentials par défaut sont définis dans `mqtt_to_zabbix.py` via `client.username_pw_set()`.

---

## Notes

- L'IP de `ai_brain` est actuellement codée en dur (`172.19.0.3`) dans `meteo_france.py` et `mqtt_to_zabbix.py` — à remplacer par le nom du service Docker (`ai-brain`) pour plus de robustesse
- Le modèle `.keras` est inclus pour permettre le ré-entraînement, mais seul le `.tflite` est utilisé en production