# Matrice OSC Module for Chataigne
This module is designed to control the LED Matrix, directly connected to the main ESP OR a TouchOSC software that simulates the entire matrix (using OSC...)
## X33D200 Matrix
Crest Hackathon 2022 matrix of the 5Tribus association (https://www.5tribus.art/)

---

# The matrix
The matrix is composed of 8x8 squares whose lighting on each side is remotely controlled.
https://github.com/MeGaMoV/X33D200

## Physical Composition
- 3 TinyPicos to control the lighting of the X and Y axes and the turn of the square
- 1 Master ESP
- 16x2 (32) strips of 150 LEDs each for the X and Y axes
- 4 ribbons of 300 LEDs each around the square

---

# Matrice OSC Module for Chataigne
This is based on the Chataigne sample custom module. More info about Chataigne : https://benjamin.kuperberg.fr/chataigne
You may want to check out the custom module documentation here : https://bkuperberg.gitbook.io/chataigne-docs/modules/custom-modules/making-your-own-module
