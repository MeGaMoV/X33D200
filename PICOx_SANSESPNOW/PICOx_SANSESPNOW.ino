#include <FastLED.h>

#define DATA_PIN 25
#define CHIPSET WS2812
#define ORDRE_LED RGB

#define BRIGHTNESS 10
#define LED_PAR_LIGNE 150
#define DECALAGE_PAR_COTE 3
#define LIGNES 16
#define LARGEUR_FIXTURE 9
#define COTES 8

////////////////////////////////////

// Un carré, avec ses fixtures horizontales 1, 2, 5 et 6
struct carre {
  short f1 = 0;
  short f2 = 0;
  short f5 = 0;
  short f6 = 0;
  short pos = 1;
};

// Tableau de carré
carre carres[8][8] = {};

// Lignes inversées - différentes de ORDRE_LED (RGB / GRB)
// (Non implémenté pour l'instant)
short lignesInverses[] = {1, 16};

CRGB rawleds[LIGNES * LED_PAR_LIGNE];
CRGBSet leds(rawleds, LIGNES * LED_PAR_LIGNE);

void setup() {
  Serial.begin(9600);

  //Initialisation de toutes les LED, nettoyage et luminosité
  FastLED.addLeds<CHIPSET, DATA_PIN, ORDRE_LED>(leds, LIGNES * LED_PAR_LIGNE);
  FastLED.clear();
  FastLED.setBrightness(BRIGHTNESS);
  initialisationPositionCarres();
}

void loop() {
  short positionCouleur = 0;
      
  CRGB couleur = CRGB::Green;
  
  for(int y = 0; y < 8; y++) {
    for(int x = 0; x < 8; x++) {
      if(positionCouleur == 0) couleur = CRGB::Red; 
      if(positionCouleur == 1) couleur = CRGB::Green; 
      if(positionCouleur == 2) couleur = CRGB::Blue; 

      //Serial.println((String) "Carre " + x + ":" + y + " Allumage Fixture : 1");
      //Serial.print((String) " position " + x + ":" + y);

      allumerCarre(carres[x][y], couleur);
      FastLED.show();
 
      positionCouleur++;
      if(positionCouleur == 3) positionCouleur = 0;
    }
  }
  delay(2000);
}

/*void allumerLigne(int ligne, CRGB couleur) {
  int interval_min = ((ligne - 1) * LED_PAR_LIGNE ) + DECALAGE_PAR_COTE ;
  int interval_max = (interval_min + (LED_PAR_LIGNE - 1)) - DECALAGE_PAR_COTE;

  CRGBSet ligne_set(leds(interval_min, interval_max));
  fill_solid(ligne_set, LED_PAR_LIGNE - 2 * DECALAGE_PAR_COTE , couleur);
  FastLED.show();
}*/

// Fonction qui initialise tous les carres et leurs fixtures respectives
void initialisationPositionCarres() {
  short posx = 0; //Pos X du carré en cours
  short posy = 0; //Pos Y du carré en cours
  short posFixture = 0; //Incrémente à chaque fixture

  short decalageEnCours = DECALAGE_PAR_COTE; // Sera incrémenté à chaque ligne

  short ordreActuel = true; //Sens de "lecure" des LED : aller (true) / retour (false)

  // Pour chaque LED
  for (int tmpPosLED = 0; tmpPosLED < LIGNES * LED_PAR_LIGNE; tmpPosLED++) {
    // On est sur un début de fixture
    if (tmpPosLED % LARGEUR_FIXTURE == 0) {
      //On ajoute la nouvelle fixture au carré concerné
      ajouterNouvelleFixture(posx, posy, tmpPosLED + decalageEnCours);

      posFixture++;

      // Nouveau carré (une fixture sur 2)
      if (posFixture % 2 == 0) {
        // On avance ou "recul" vision "carrés"
        if (ordreActuel) posx++; else posx--;

        // On est de retour au début de ligne ou à la fin
        if (posx < 0 || posx * 2 > 15) {
          decalageEnCours += DECALAGE_PAR_COTE * 2; //On ajoute le decalage
          if (ordreActuel) {
            posx = 7;
            ordreActuel = false;
          } else {
            posx = 0;
            ordreActuel = true;
            posy++;
          }
        }
        // Si on sort du tableau
        if (posy * 2 >= 16) break;
      }
    }
  }
}

void ajouterNouvelleFixture(short posx, short posy, short tmpPosLED) {
  //Serial.println((String)"x:" + posx + " y:" + posy + " LED:" + tmpPosLED + " sur la pos:" +carres[posx][posy].pos);

  switch (carres[posx][posy].pos) {
    case 1:
      carres[posx][posy].f1 = tmpPosLED;
      break;
    case 2:
      carres[posx][posy].f2 = tmpPosLED;
      break;
    case 3:
      carres[posx][posy].f5 = tmpPosLED;
      break;
    case 4:
      carres[posx][posy].f6 = tmpPosLED;
      break;
  }

  carres[posx][posy].pos++;
}

void allumerCarre(carre leCarre, CRGB couleur){
  allumerFixture(leCarre, 1, couleur);
  allumerFixture(leCarre, 2, couleur);
  allumerFixture(leCarre, 3, couleur);
  allumerFixture(leCarre, 4, couleur);
}

void allumerFixture(carre leCarre, short numeroFixture, CRGB couleur) {
  //Generer un set pour la fixture
  if (numeroFixture == 1) {
    CRGBSet fixture(leds(leCarre.f1, leCarre.f1 + LARGEUR_FIXTURE));
    fill_solid(fixture, LARGEUR_FIXTURE, couleur);
  } else {
    if (numeroFixture == 2) {
      CRGBSet fixture(leds(leCarre.f2, leCarre.f2 + LARGEUR_FIXTURE));
      fill_solid(fixture, LARGEUR_FIXTURE, couleur);
    } else {
      if (numeroFixture == 3) {
        CRGBSet fixture(leds(leCarre.f5, leCarre.f5 + LARGEUR_FIXTURE));
        fill_solid(fixture, LARGEUR_FIXTURE, couleur);
      } else {
         CRGBSet fixture(leds(leCarre.f6, leCarre.f6 + LARGEUR_FIXTURE));
         fill_solid(fixture, LARGEUR_FIXTURE, couleur);
      }
    }
  }
}
