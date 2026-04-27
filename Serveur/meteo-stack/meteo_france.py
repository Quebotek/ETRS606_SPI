import requests
import subprocess
import time

ZABBIX_SERVER = "10.1.110.50"
AI_BRAIN_URL = "http://172.19.0.3:5000/predict"


VILLES = {
    "Meteo-Lyon": {"lat": 45.76, "lon": 4.83},
    "Meteo-Marseille": {"lat": 43.29, "lon": 5.36},
    "Meteo-Bordeaux": {"lat": 44.83, "lon": -0.57},
    "Meteo-Strasbourg": {"lat": 48.57, "lon": 7.75},
    "Meteo-Lille": {"lat": 50.62, "lon": 3.05}
}

print("Démarrage du service Météo France API (Temp, Hum, Pres)...")

while True:
    print("\n--- Récupération API Météo (Open-Meteo) ---")
    for host, coords in VILLES.items():
        try:
            # Nouvelle URL demandant température, humidité et pression
            url = f"https://api.open-meteo.com/v1/forecast?latitude={coords['lat']}&longitude={coords['lon']}&current=temperature_2m,relative_humidity_2m,surface_pressure"
            data = requests.get(url).json()

            # Extraction des 3 valeurs
            temp = data["current"]["temperature_2m"]
            hum = data["current"]["relative_humidity_2m"]
            pres = data["current"]["surface_pressure"]

	    # Interrogation du Cerveau IA local
            payload = {"temp": temp, "humidity": hum, "pressure": pres}
            reponse_ia = requests.post(AI_BRAIN_URL, json=payload).json()
            prediction_texte = reponse_ia.get("prediction", "Erreur IA")

            print(f"{host} : {temp} C | {hum} % | {pres} hPa  ----> {prediction_texte}  ")

            # Envoi des 3 valeurs vers Zabbix
            subprocess.run(f"zabbix_sender -z {ZABBIX_SERVER} -s '{host}' -k 'meteo.temp' -o '{temp}'", shell=True)
            subprocess.run(f"zabbix_sender -z {ZABBIX_SERVER} -s '{host}' -k 'meteo.humidity' -o '{hum}'", shell=True)
            subprocess.run(f"zabbix_sender -z {ZABBIX_SERVER} -s '{host}' -k 'meteo.pressure' -o '{pres}'", shell=True)
            subprocess.run(f"zabbix_sender -z {ZABBIX_SERVER} -s '{host}' -k 'meteo.prediction' -o '{prediction_texte}'", shell=True)

        except Exception as e:
            print(f"Erreur avec {host}: {e}")

    print("Mise à jour terminée. Prochaine exécution dans 1 heure.")
    time.sleep(3600)
