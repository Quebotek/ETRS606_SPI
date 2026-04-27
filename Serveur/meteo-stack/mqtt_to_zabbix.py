import paho.mqtt.client as mqtt
import json
import subprocess
import requests
import os
import time

# --- Configuration ---
MQTT_BROKER = os.getenv("MQTT_BROKER", "mqtt_broker")
MQTT_PORT = int(os.getenv("MQTT_PORT", 1883))
MQTT_TOPIC = os.getenv("MQTT_TOPIC", "meteo/data") # Adaptez si votre topic est différent

ZABBIX_SERVER = "10.1.110.50"
ZABBIX_HOST = "STM32-Station-Meteo"

AI_BRAIN_URL = "http://172.19.0.3:5000/predict"


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print(f"✅ Connecté avec succès au broker MQTT ({MQTT_BROKER})", flush=True)
        client.subscribe(MQTT_TOPIC)
        print(f"📡 En écoute sur le topic : {MQTT_TOPIC}", flush=True)
    else:
        print(f"❌ Échec de la connexion MQTT (Code: {rc})", flush=True)

def on_message(client, userdata, msg):
    try:
       # 1. Décodage ultra-propre (on enlève les caractères nuls \0 du C)
        raw_payload = msg.payload.decode('utf-8', errors='ignore').strip().replace('\x00', '')
        data = json.loads(raw_payload)

        # 2. Extraction flexible (on cherche la clé STM32 'hum' OU la clé IA 'humidity')
        t = float(data.get('temp', 0))
        h = float(data.get('hum', data.get('humidity', 0)))
        p = float(data.get('press', data.get('pressure', 1013)))

        # 3. Log de confirmation immédiat
        print(f"✅ Données extraites : T={t}, H={h}, P={p}", flush=True)

        # 4. Envoi à l'IA (L'IA veut 'humidity' et 'pressure')
# On s'assure que ce sont des FLOTTANTS et non des chaînes
        payload_ia = {
            "temp": float(t),
            "humidity": float(h),
            "pressure": float(p)
        }

        # On force l'en-tête JSON
        headers = {'Content-Type': 'application/json'}

        try:
            # On utilise l'argument 'json=' qui gère automatiquement le formatage
            reponse = requests.post(AI_BRAIN_URL, json=payload_ia, headers=headers, timeout=5)

            if reponse.status_code == 200:
                prediction = reponse.json().get("prediction", "Inconnue")
                print(f"🧠 IA prédit : {prediction}", flush=True)
            else:
                # Si ça échoue, on affiche le contenu de l'erreur renvoyé par Flask
                print(f"❌ Erreur IA {reponse.status_code} : {reponse.text}", flush=True)
                prediction = "Erreur IA"
        except Exception as e:
            print(f"📡 Erreur connexion IA : {e}", flush=True)
            prediction = "Erreur Connexion"
        # 5. Envoi à Zabbix (On utilise les clés Item que vous avez créées)
        # Note : Vérifiez bien que 'meteo.temp', etc. existent dans Zabbix
        subprocess.run(f"zabbix_sender -z {ZABBIX_SERVER} -s '{ZABBIX_HOST}' -k 'meteo.temp' -o '{t}'", shell=True)
        subprocess.run(f"zabbix_sender -z {ZABBIX_SERVER} -s '{ZABBIX_HOST}' -k 'meteo.humidity' -o '{h}'", shell=True)
        subprocess.run(f"zabbix_sender -z {ZABBIX_SERVER} -s '{ZABBIX_HOST}' -k 'meteo.pressure' -o '{p}'", shell=True)
        subprocess.run(f"zabbix_sender -z {ZABBIX_SERVER} -s '{ZABBIX_HOST}' -k 'meteo.prediction' -o '{prediction}'", shell=True)

    except Exception as e:
        print(f"🔥 Erreur critique : {e} | Message brut : {msg.payload}", flush=True)
# --- Initialisation MQTT ---
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set("albert", "pass123")

# Boucle de reconnexion automatique en cas de perte du broker
while True:
    try:
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        client.loop_forever()
    except Exception as e:
        print(f"⏳ Broker MQTT introuvable, nouvelle tentative dans 5s... ({e})", flush=True)
        time.sleep(5)
