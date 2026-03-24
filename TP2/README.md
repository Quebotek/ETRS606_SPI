TP 2 : Interface Capteur & STM32

La carte de développement NUCLEO-N657X0 est basée sur un microcontrôleur ARM Cortex-M33 cadencé jusqu’à 160 MHz,
avec une mémoire RAM de 320 ko et une mémoire Flash de 512 Ko. Elle est illustrée sur la figure 1. Elle offre des interfaces 
variées et dispose de ressources hardware avancé, comme un accélérateur cryptographique et neuromorphic processing unit (NPU)
type Neural-ART (600 GOPS, 1 GHz), optimisé pourl'inférence de modèles CNN. La carte capteurs X-NUCLEO-IKS01A3 regroupe 
plusieurs capteurs MEMS permettant de mesurer différentes grandeurs physiques telles que l’accélération, le champ magnétique,
la température, l’humidité ou encore la pression atmosphérique. Ces capteurs communiquent avec le microcontrôleur via un bus
I2C, largement utilisé dans les systèmes embarqués pour connecter des périphériques à faible débit. 

L’I2C est un protocole de communication de niveau physique. Son fonctionnement s’appui sur trois fils :
-SDA : Serial Data
-SCL : Serial Clock
-La référence : Masse
Le fil nommé SCL transmet l’horloge pour la synchronisation de communication; alors que la
ligne SDA transmet les informations (adresses et données ).
Un seul boîtier maître peut commander plusieurs boîtiers esclaves, (chacun d’entre eux
répondant à une adresse unique).
Un esclave ne s’exprime que sur ordre d’un maître et plusieurs maîtres peuvent partager le même réseau.

Nous avions pour objectif de récupérer les valeurs des capteurs de température, d'humidité et de pression. En effet 
ces valeurs nous seront précieuses. C'est pourquoi nous nous sommes penchés sur les capteurs suivants :

-HTS221 pour l'humidité et la température
-LPS22HH pour la pression 

Nous sommes donc bel et bien parvenus à obtenir ces 3 valeurs comme le démontre cette capture d'écran : 

<img width="247" height="363" alt="image" src="https://github.com/user-attachments/assets/fbc59507-cfb4-4891-ba60-f4ab3681f20a" />
