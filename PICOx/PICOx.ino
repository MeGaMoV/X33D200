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

// Structure de fixture
struct fixture {
  short rang;
  short r;
  short g;
  short b;
};

// Un carré, avec ses fixtures horizontales 1, 2, 5 et 6
struct carre {
  fixture fixtures[4];
  short pos = 0;
};

// Structure d'un message reçu via ESP-NOW
typedef struct espnow_msg {
  carre matrice[8][8];
  byte intensite;
  byte fonction;
} espnow_msg;

// Message reçu type "espnow_msg"
espnow_msg messageRecu;

// Tableau de carré
carre matrice[8][8] = {};

// Position des LED inversées
// Exemple : 0 pour des LED 0 à 149
short ledInverses[] = {0, 2249};

CRGB rawleds[LIGNES * LED_PAR_LIGNE];
CRGBSet leds(rawleds, LIGNES * LED_PAR_LIGNE);

void setup() {
  //Serial.begin(9600);

  //Initialisation de toutes les LED, nettoyage et luminosité
  FastLED.addLeds<CHIPSET, DATA_PIN, ORDRE_LED>(leds, LIGNES * LED_PAR_LIGNE);
  FastLED.clear();
  FastLED.setBrightness(BRIGHTNESS);
  initialisationPositionmatrice();

  // Ecouter en ESP-NOW
  // Mode "Wifi station" et initialisation
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // On se prépare à recevoir une trame
  esp_now_register_recv_cb(majMatrice);
}

void loop() {
}

void majMatrice(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&messageRecu, incomingData, sizeof(messageRecu));

  for(short y = 0; y < 8; y++){
    for(short x = 0; x < 8; x++){
      for(short f = 0; f <4; f++){
        matrice[x][y].fixtures[f].r = messageRecu.matrice[x][y].fixtures[f].r;
        matrice[x][y].fixtures[f].g = messageRecu.matrice[x][y].fixtures[f].g;
        matrice[x][y].fixtures[f].b = messageRecu.matrice[x][y].fixtures[f].b;
      }
    }
  }

  afficherMatrice();
 
  FastLED.show();
}

// Fonction qui initialise tous les matrice et leurs fixtures respectives
void initialisationPositionmatrice() {
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
  fixture maFixture = {0,0,0,0};
  maFixture.rang = tmpPosLED;
  matrice[posx][posy].fixtures[matrice[posx][posy].pos] = maFixture;
  matrice[posx][posy].pos++;
}

void afficherMatrice(){
  short posDebut = 0;
  short posFin = 0;

  CRGB couleur = CRGB::Black;

  for(short y = 0; y < 8; y++){
    for(short x = 0; x < 8; x++){
      for(short f = 0; f <4; f++){

        //Determiner la couleur
        couleur.r = matrice[x][y].fixtures[f].r;
        couleur.g = matrice[x][y].fixtures[f].g;
        couleur.b = matrice[x][y].fixtures[f].b;

        // Cas des inversion de couleurs
        for(int pos=0; pos < sizeof ledInverses/sizeof ledInverses[0]; pos++) {
          if(posDebut >= ledInverses[pos] && posDebut <= ledInverses[pos] + LED_PAR_LIGNE){
            int tmpR = couleur.r;
            couleur.r = couleur.g;
            couleur.g = tmpR;
          }
        }
  
        posDebut = matrice[x][y].fixtures[f].rang;
        posFin = matrice[x][y].fixtures[f].rang + LARGEUR_FIXTURE;

        CRGBSet fixture(leds(posDebut, posFin));
        fill_solid(fixture, LARGEUR_FIXTURE, couleur);
      }
    }
  }
}
