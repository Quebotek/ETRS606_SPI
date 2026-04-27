# Projet IA Météo — STM32N6 (Nx_TCP_Echo_Client)

Projet embarqué sur **STM32N657X0** (Cortex-M55) réalisant une inférence météo en temps réel avec publication MQTT.

---

## Architecture générale

```
Capteurs I2C (HTS221 + LPS22HH)
        │
        ▼
  Normalisation Z-score
        │
        ▼
  Inférence X-CUBE-AI (INT8)        ←── Modèle réseau de neurones quantifié
        │
        ▼
  Déquantification + Argmax
        │
        ▼
  Publication MQTT ──────────────────► meteo.quebotek.fr:18830
        │
        ▼
  Topic: meteo/data
  Payload: {"ts":..., "meteo":"...", "proba":..., "temp":..., "hum":..., "pres":...}
```

---

## Matériel requis

| Composant | Référence |
|---|---|
| Carte | STM32N657X0-DK (NUCLEO-N657X0-Q) |
| Capteur température/humidité | HTS221 (I2C1) |
| Capteur pression | LPS22HH (I2C1) |
| Connexion réseau | Ethernet RJ45 |

---

## Stack logicielle

| Couche | Technologie |
|---|---|
| RTOS | Azure RTOS ThreadX |
| Stack réseau | NetXDuo (DHCP, DNS, SNTP, MQTT) |
| Inférence IA | X-CUBE-AI (NetworkRuntime 10.2.0) |
| HAL | STM32N6xx HAL |
| Compilateur | GCC ARM 14.3 (arm-none-eabi) |
| IDE | STM32CubeIDE |

---

## Configuration mémoire

### Linker script (`STM32N657X0HXQ_AXISRAM2_fsbl.ld`)

| Région | Adresse | Taille | Usage |
|---|---|---|---|
| ROM | 0x34180400 | 255 Ko | Code + constantes |
| RAM | 0x341C0000 | 256 Ko | Stack, BSS, données |
| RAM_NX | 0x34000000 | 320 Ko | Pools ThreadX/NetX + modèle IA |

### Pools mémoire (`app_azure_rtos_config.h`)

```c
#define TX_APP_MEM_POOL_SIZE     4096    //  4 Ko — pool ThreadX (inutilisé)
#define NX_APP_MEM_POOL_SIZE     57344   // 56 Ko — pool NetXDuo
```

### Packet pool (`app_netxduo.h`)

```c
#define NX_APP_PACKET_POOL_SIZE    ((DEFAULT_PAYLOAD_SIZE + sizeof(NX_PACKET)) * 20)
#define Nx_IP_INSTANCE_THREAD_SIZE    8 * 1024
```

---

## Configuration réseau

| Paramètre | Valeur |
|---|---|
| Adresse IP | DHCP automatique |
| Broker MQTT | `meteo.quebotek.fr:18830` |
| Topic MQTT | `meteo/data` |
| Client ID | `STM32_ALBERT_N6` |
| Serveur SNTP | `172.232.44.156` (pool.ntp.org) |
| Fuseau horaire | Europe/Paris (UTC+1/+2 DST auto) |

---

## Paramètres du modèle IA

### Classes de sortie

| Index | Label |
|---|---|
| 0 | Dégagé |
| 1 | Nuageux |
| 2 | Pluie |
| 3 | Brouillard |
| 4 | Orage |

### Features d'entrée (8 features)

| Index | Feature | Mean | Std |
|---|---|---|---|
| 0 | Température (°C) | 12.96 | 7.18 |
| 1 | Point de rosée estimé | 7.53 | 5.68 |
| 2 | Humidité (%) | 72.87 | 17.99 |
| 3 | Pression (hPa) | 1017.13 | 9.44 |
| 4 | sin(heure) | -0.001 | 0.707 |
| 5 | cos(heure) | 0.001 | 0.707 |
| 6 | sin(mois) | -0.003 | 0.708 |
| 7 | cos(mois) | 0.009 | 0.705 |

### Quantification INT8

```c
input_scale      = 0.027173925f
input_zeropoint  = 23
output_scale     = 0.00390625f
output_zeropoint = -128
```

---

## Threads ThreadX

| Thread | Stack | Rôle |
|---|---|---|
| NxAppThread | 4 Ko | Orchestrateur : DHCP → DNS → SNTP → démarrage IA |
| AppTCPThread | 16 Ko | Inférence IA + capteurs + publication MQTT |
| AppLinkThread | 2 Ko | Surveillance lien Ethernet |
| IP Instance Thread | 8 Ko | Thread interne NetXDuo (driver ETH) |

---

## Configuration MPU (points critiques STM32N6)

La MPU est configurée pour rendre non-cacheables les zones accédées par le DMA ETH :

| Région | Adresse | Taille | Usage |
|---|---|---|---|
| 0 | 0x341F8000 | 384 o | Descripteurs DMA RX ETH |
| 1 | 0x341F8180 | ~512 Ko | Reste RAM ETH |
| 2 | 0x34000000 | 320 Ko | RAM_NX — pools NetX (packet pool) |

> **Important** : Sans cette configuration MPU, le DMA ETH écrit en RAM mais le CPU lit le cache périmé → crash silencieux lors de la réception DHCP Reply.

---

## Build

### Prérequis

- STM32CubeIDE (testé avec la version incluant GCC 14.3)
- Toolchain : `arm-none-eabi-gcc 14.3.rel1` (macOS aarch64)

### Workflow de build hybride

STM32CubeIDE regénère `objects.list` à chaque build et écrase les `.o` compilés manuellement. Pour les fichiers non détectés automatiquement :

```bash
# Répertoire de build
cd "/chemin/vers/STM32CubeIDE/FSBL/Debug"

# Compiler les fichiers manuels si nécessaire
GCC=".../arm-none-eabi-gcc"
$GCC <fichier.c> <flags> -o <fichier.o>

# Linker depuis le terminal après build STM32CubeIDE
arm-none-eabi-gcc -o Nx_TCP_Echo_Client_FSBL.elf @objects.list \
  -l:NetworkRuntime1020_CM55_GCC.a ...
```

### Fichiers ajoutés manuellement au build

- `stm32n6xx_hal_i2c.c` / `stm32n6xx_hal_i2c_ex.c` — driver HAL I2C
- `hts221.c` — driver capteur température/humidité
- `lps22hh.c` — driver capteur pression

---

## Problèmes connus et solutions

### Reboot en boucle lors du DHCP
**Cause** : D-Cache activé, descripteurs ETH DMA en mémoire cacheable.  
**Fix** : Déplacer les descripteurs en `.noncacheable` ET configurer la MPU sur `RAM_NX`.

### `ai_run` / `AI_Run` — undefined reference
**Cause** : Fonction déclarée `static` dans `app_x-cube-ai.c`.  
**Fix** : Supprimer le `static` devant `int ai_run(void)`.

### Doublons X-CUBE-AI au link
**Cause** : CubeMX régénère les sources dans un nouveau dossier sans supprimer l'ancien.  
**Fix** : Supprimer les anciens `.o` (`App/app_x-cube-ai.o`, `App/network.o`, etc.) de `objects.list`.

### DHCP timeout (`semaphore ret = 0x0D`)
**Cause** : Pool NetX trop grand → débordement de `RAM_NX` → corruption silencieuse.  
**Fix** : `TX=4Ko + NX=56Ko = 60Ko < 64Ko` alloué dans le linker.

---

## Structure du projet

```
Nx_TCP_Echo_Client/
├── FSBL/
│   ├── Core/Src/
│   │   ├── main.c              # MPU config, init HAL, démarrage ThreadX
│   │   └── stm32n6xx_it.c      # IRQ handlers (ETH1_IRQHandler)
│   ├── NetXDuo/App/
│   │   ├── app_netxduo.c       # Pipeline principal : DHCP→SNTP→IA→MQTT
│   │   └── app_netxduo.h       # Defines mémoire et réseau
│   ├── AZURE_RTOS/App/
│   │   └── app_azure_rtos_config.h  # Tailles des pools mémoire
│   └── X-CUBE-AI/App/
│       ├── app_x-cube-ai.c     # Wrapper inférence (ai_run)
│       ├── network.c           # Modèle généré par X-CUBE-AI
│       └── network_data_params.c  # Poids quantifiés INT8
├── Drivers/
│   └── BSP/Components/
│       ├── hts221/             # Driver capteur HTS221
│       └── lps22hh/            # Driver capteur LPS22HH
└── STM32CubeIDE/FSBL/
    ├── STM32N657X0HXQ_AXISRAM2_fsbl.ld  # Linker script
    └── Debug/                  # Artefacts de build
```

---

## Auteur

Albert — ETRS606, S6  
Projet IA embarquée sur STM32N6