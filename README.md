
# X33D200

Matrice du Hackathon 2022 de Crest de l'association 5Tribus (https://www.5tribus.art/)

## La matrice

La matrice est composée de 8x8 carrés dont l'éclairage de chaque cotés est contrôlé à distance.

## Composition physique

- 3 TinyPicos afin de contrôler l'éclairage des axes X et Y et le tour du carré
- 1 ESP Maître
- 16x2 (32) rubans de 150 LED chacun pour les axes X et Y
- 4 rubans de 300 LED chacun pour le tour du carré

## Installation Physique

- PicoX : Code du TinyPico qui gère l'axe X de la Matrice (16 x 150 LED's) depuis son PIN 25
- PicoY : Code du TinyPico qui gère l'axe Y de la Matrice (16 x 150 LED's) depuis son PIN 25
- Pico3 : Code du TinyPico qui gère le cadre de la Matrice (4 x 150 LED's) depuis son PIN 25
- ESP_EMETTEUR : ESP Maître maintient la liaison avec la partie logicielle via OSC

## Logicielle
1. Simulateur TouchOSC
- Fichier Matrice.tosc écoutant en OSC les trames venant du logiciel Maître (permet de simuler la matrice physique)

2. Logiciel Chataigne et Module "Matrice"
- Installer le logiciel Chataigne https://benjamin.kuperberg.fr/chataigne/
- Déplacer le module dans le dossier Documents/Chataigne/modules
- Demarrer Chataigne