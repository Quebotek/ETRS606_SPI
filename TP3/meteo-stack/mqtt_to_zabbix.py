import paho.mqtt.client as mqtt
import requests
import subprocess

# Configuration
MQTT_HOST = "mosquitto"
MQTT_PORT = 1883
MQTT_USER = "albert"
MQTT_PASS = "pass123" # Mettez votre vrai mot de passe
ZABBIX_SERVER = "10.1.110.50"
ZABBIX_HOST = "STM32-Station-Meteo"
AI_BRAIN_URL = "http://ai_brain:5000/predict"

# Mémoire temporaire pour stocker les capteurs au fur et à mesure
capteurs = {"temp": None, "humidity": None, "pressure": None}

def on_message(client, userdata, msg):
    topic = msg.topic
    valeur = msg.payload.decode().strip()

    # 1. Mise à jour de la mémoire et envoi immédiat à Zabbix
    key = None
    if topic == "meteo/temperature":
        key = "meteo.temp"
        capteurs["temp"] = float(valeur)
    elif topic == "meteo/humidite":
        key = "meteo.humidity"
        capteurs["humidity"] = float(valeur)
    elif topic == "meteo/pression":
        key = "meteo.pressure"
        capteurs["pressure"] = float(valeur)

    if key:
        print(f"📥 [{topic}] -> {valeur} (Envoi Zabbix...)")
        subprocess.run(f"zabbix_sender -z {ZABBIX_SERVER} -s '{ZABBIX_HOST}' -k '{key}' -o '{valeur}'", shell=True)

    # 2. Si on a les 3 valeurs en mémoire, on interroge le Cerveau IA !
    if all(v is not None for v in capteurs.values()):
        try:
            payload = {"temp": capteurs["temp"], "humidity": capteurs["humidity"], "pressure": capteurs["pressure"]}
            reponse_ia = requests.post(AI_BRAIN_URL, json=payload).json()
            prediction_texte = reponse_ia.get("prediction", "Erreur")

            print(f"🧠 L'IA a analysé les données STM32 et prédit : {prediction_texte}")

            # Envoi de la prédiction à Zabbix
            subprocess.run(f"zabbix_sender -z {ZABBIX_SERVER} -s '{ZABBIX_HOST}' -k 'meteo.prediction' -o '{prediction_texte}'", shell=True)

            # On vide la mémoire pour attendre la prochaine salve de 3 capteurs du STM32
            capteurs["temp"] = None
            capteurs["humidity"] = None
            capteurs["pressure"] = None

        except Exception as e:
            print(f"⚠️ Erreur de communication avec le Cerveau IA : {e}")

# Initialisation MQTT
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1)
client.username_pw_set(MQTT_USER, MQTT_PASS)
client.on_message = on_message
client.connect(MQTT_HOST, MQTT_PORT, 60)
client.subscribe("meteo/#")

print("🚀 Pont MQTT <-> Zabbix (Version Microservice IA) démarré !")
client.loop_forever()

