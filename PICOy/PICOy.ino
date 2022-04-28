#include <esp_now.h>
#include <WiFi.h>
#include <FastLED.h>

#define DATA_PIN 25
#define CHIPSET WS2812
#define ORDRE_LED RGB

#define BRIGHTNESS 25
#define LED_PAR_LIGNE 150
#define DECALAGE_PAR_COTE 3
#define LIGNES 16
#define LARGEUR_FIXTURE 9
#define COTES 8

////////////////////////////////////

// Un carré, avec ses fixtures verticales 8, 7, 4 et 3
struct carre {
  short f8 = 0;
  short f7 = 0;
  short f4 = 0;
  short f3 = 0;
  short pos = 1;
};

// Structure d'un message reçu via ESP-NOW
typedef struct espnow_msg {
  byte col;
  byte ligne;
  byte fixture;
  byte R;
  byte G;
  byte B;
  byte intensite;
  byte fonction;
} espnow_msg;

// Message reçu type "espnow_msg"
espnow_msg messageRecu;

// Tableau de carré
carre carres[8][8] = {};

// Lignes inversées - différentes de ORDRE_LED (RGB / GRB)
// (Non implémenté pour l'instant)
short lignesInverses[] = {1, 16};

CRGB rawleds[LIGNES * LED_PAR_LIGNE];
CRGBSet leds(rawleds, LIGNES * LED_PAR_LIGNE);

void setup() {
  //Serial.begin(9600);

  //Initialisation de toutes les LED, nettoyage et luminosité
  FastLED.addLeds<CHIPSET, DATA_PIN, ORDRE_LED>(leds, LIGNES * LED_PAR_LIGNE);
  FastLED.clear();
  FastLED.setBrightness(BRIGHTNESS);
  initialisationPositionCarres();

  // Ecouter en ESP-NOW
  // Mode "Wifi station" et initialisation
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // On se prépare à recevoir une trame
  esp_now_register_recv_cb(receptionDonneeESPNOW);
}

void loop() {
}

void receptionDonneeESPNOW(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&messageRecu, incomingData, sizeof(messageRecu));

  CRGB couleur = CRGB::Black;
  couleur.r = messageRecu.R;
  couleur.g = messageRecu.G;
  couleur.b = messageRecu.B;
  
  if(messageRecu.fonction == 0){
    allumerCarre(carres[messageRecu.ligne][messageRecu.col], couleur);
    FastLED.show();
    //Serial.println((String) "Carre " + messageRecu.col + ":" + messageRecu.ligne + " => " + couleur.r + " " + couleur.g + " " + couleur.b);
  }
}

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
      carres[posx][posy].f3 = tmpPosLED;
      break;
    case 2:
      carres[posx][posy].f4 = tmpPosLED;
      break;
    case 3:
      carres[posx][posy].f7 = tmpPosLED;
      break;
    case 4:
      carres[posx][posy].f8 = tmpPosLED;
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
    CRGBSet fixture(leds(leCarre.f3, leCarre.f3 + LARGEUR_FIXTURE));
    fill_solid(fixture, LARGEUR_FIXTURE, couleur);
  } else {
    if (numeroFixture == 2) {
      CRGBSet fixture(leds(leCarre.f4, leCarre.f4 + LARGEUR_FIXTURE));
      fill_solid(fixture, LARGEUR_FIXTURE, couleur);
    } else {
      if (numeroFixture == 3) {
        CRGBSet fixture(leds(leCarre.f7, leCarre.f7 + LARGEUR_FIXTURE));
        fill_solid(fixture, LARGEUR_FIXTURE, couleur);
      } else {
         CRGBSet fixture(leds(leCarre.f8, leCarre.f8 + LARGEUR_FIXTURE));
         fill_solid(fixture, LARGEUR_FIXTURE, couleur);
      }
    }
  }
}
