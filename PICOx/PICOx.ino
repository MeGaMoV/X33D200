#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
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

/////// CODE TINYPICO X ou Y ///////
bool PicoX = true;

////////////////////////////////////
// Gestion OSC
WiFiUDP Udp;
const char* ssid = "Private";
const char* password = "zaza4014";
const unsigned int localPort = 8888;
uint8_t bFixture[3]; //Stockage données OSC fixture
uint8_t bCarre[12];  //Stockage données OSC carree
  
// Structure de fixture
struct fixture {
  short rang;
  short r;
  short g;
  short b;
};

// Un carré, avec ses 4 fixtures dans un tableau
struct carre {
  fixture fixtures[4];
  short pos = 0;
};

// Tableau de carré
carre matrice[8][8] = {};

// String fixture carre OSC pour parsing
String carreX, carreY, numFixture;

// Position des LED inversées
// Exemple : 0 pour des LED 0 à 149
short ledInverses[] = {0, 2249};

CRGB rawleds[LIGNES * LED_PAR_LIGNE];
CRGBSet leds(rawleds, LIGNES * LED_PAR_LIGNE);

void setup() {
  Serial.begin(115200);

  // Initialisation Wifi
  WiFi.disconnect(true);
  WiFi.onEvent(Wifi_connected, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(Get_IPAddress, SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(Wifi_disconnected, SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.begin(ssid, password);
  Udp.begin(localPort);

  //Initialisation de toutes les LED, nettoyage et luminosité
  FastLED.addLeds<CHIPSET, DATA_PIN, ORDRE_LED>(leds, LIGNES * LED_PAR_LIGNE);
  FastLED.setBrightness(BRIGHTNESS);

  //Initialiser la position des LED sur la matrice
  initialisationPositionMatrice();

  //Tout à zero
  couleurMatrice(0,0,0);
  majMatrice();
  FastLED.show();
}

void loop() {
  OSCMessage msg;
  int size;
  char strAddress[100];
  String addressSTO;

  // Tant que l'on reçoit quelque-chose en UDP
  if((size = Udp.parsePacket()) > 0)
  {
    //Recuperation du flux UDP dans msg
    while(size--) msg.fill(Udp.read());

    //On récupère l'adresse
    msg.getAddress(strAddress, 0);
    addressSTO = (char*)strAddress;

    if (addressSTO == "/print") {
      majMatrice();
      FastLED.show();
    }

    if (addressSTO == "/clear") {
      couleurMatrice(0,0,0);
      majMatrice();
      FastLED.show();
    }

    if (msg.match("/intensite") > 0) {
      int intensite = msg.getInt(0);
      if(intensite <= 255) FastLED.setBrightness(intensite);
    }
    
    if (msg.match("/fixture") > 0) {
      carreX = addressSTO.substring(9, 10);
      carreY = addressSTO.substring(11, 12);
      numFixture = addressSTO.substring(13);
      msg.getBlob(0, bFixture, 3);
      majFixture(carreX.toInt(), carreY.toInt(), numFixture.toInt());
    }
    
    if (msg.match("/carre") > 0) {
      carreX = addressSTO.substring(7, 8);
      carreY = addressSTO.substring(9);
      msg.getBlob(0, bCarre, 24);
      //Serial.println((String)carreX+" "+carreY);
      majCarre(carreX.toInt(), carreY.toInt());
    }
  }
  // END LOOP
}

////////////////// PROGRAMMES DE MISE A JOUR //////////////////////
void majFixture(int carreX, int carreY, int numFixture) {
  carreX--;
  carreY--;
  if(carreX < 0 || carreX > COTES - 1 || carreY < 0 || carreY > COTES - 1) return;
  matrice[carreX][carreY].fixtures[numFixture].r = bFixture[0];
  matrice[carreX][carreY].fixtures[numFixture].g = bFixture[1];
  matrice[carreX][carreY].fixtures[numFixture].b = bFixture[2];
}

void majCarre(int carreX, int carreY) {
  int posTotal = 0;
  carreX--;
  carreY--;
  if(carreX < 0 || carreX > COTES - 1 || carreY < 0 || carreY > COTES - 1) return;
  for (int i = 0; i < 4; i++) {
    matrice[carreX][carreY].fixtures[i].r = bCarre[posTotal];
    matrice[carreX][carreY].fixtures[i].g = bCarre[posTotal + 1];
    matrice[carreX][carreY].fixtures[i].b = bCarre[posTotal + 2];
    posTotal += 3;
  }
}

////////////// FONCTIONS GENERALES DE LA MATRICE /////////////////
void couleurMatrice(short r, short g, short b) {
  for (short y = 0; y < 8; y++) {
    for (short x = 0; x < 8; x++) {
      for (short f = 0; f < 4; f++) {
        matrice[x][y].fixtures[f].r = r;
        matrice[x][y].fixtures[f].g = g;
        matrice[x][y].fixtures[f].b = b;
      }
    }
  }
}

// Fonction qui initialise tous les matrice et leurs fixtures respectives
void initialisationPositionMatrice() {
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
        if(PicoX){
          if (ordreActuel) posx++; else posx--;
        } else {
          if (ordreActuel) posy++; else posy--;
        }

        // On est de retour au début de ligne ou à la fin
        if(PicoX){ //version PicoX
          if (posx < 0 || posx * 2 > LIGNES - 1) {
            decalageEnCours += DECALAGE_PAR_COTE * 2; //On ajoute le decalage
            if (ordreActuel) {
              posx = COTES - 1;
              ordreActuel = false;
            } else {
              posx = 0;
              ordreActuel = true;
              posy++;
            }
          }
          // Si on sort du tableau
          if (posy * 2 >= LIGNES) break;
          
        } else { //version PicoY
          
          // On est de retour au début de ligne ou à la fin
          if (posy < 0 || posy * 2 > LIGNES - 1) {
            decalageEnCours += DECALAGE_PAR_COTE * 2; //On ajoute le decalage
            if (ordreActuel) {
              posy = COTES - 1;
              ordreActuel = false;
            } else {
              posy = 0;
              ordreActuel = true;
              posx++;
            }
          }
          // Si on sort du tableau
          if (posx * 2 >= LIGNES) break;
        }
      }
    }
  }
}

void ajouterNouvelleFixture(short posx, short posy, short tmpPosLED) {
  fixture maFixture = {0, 0, 0, 0};
  maFixture.rang = tmpPosLED;
  matrice[posx][posy].fixtures[matrice[posx][posy].pos] = maFixture;
  matrice[posx][posy].pos++;
}

void majMatrice() {
  short posDebut = 0;
  short posFin = 0;

  CRGB couleur = CRGB::Black;

  for (short y = 0; y < 8; y++) {
    for (short x = 0; x < 8; x++) {
      for (short f = 0; f < 4; f++) {

        //Determiner la couleur
        couleur.r = matrice[x][y].fixtures[f].r;
        couleur.g = matrice[x][y].fixtures[f].g;
        couleur.b = matrice[x][y].fixtures[f].b;

        posDebut = matrice[x][y].fixtures[f].rang;
        posFin = matrice[x][y].fixtures[f].rang + LARGEUR_FIXTURE;

        // Cas des inversion de couleurs
        for (int pos = 0; pos < sizeof ledInverses / sizeof ledInverses[0]; pos++) {
          if (posDebut >= ledInverses[pos] && posDebut <= ledInverses[pos] + LED_PAR_LIGNE) {
            int tmpR = couleur.r;
            couleur.r = couleur.g;
            couleur.g = tmpR;
          }
        }
        
        CRGBSet fixture(leds(posDebut, posFin));
        fill_solid(fixture, LARGEUR_FIXTURE, couleur);
      }
    }
  }
}

///////////////////  Fonctions WIFI ////////////////////////
void Wifi_connected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Successfully connected to Access Point");
}

void Get_IPAddress(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WIFI is connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void Wifi_disconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Disconnected from WIFI access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.disconnected.reason);
  Serial.println("Reconnecting...");
  WiFi.begin(ssid, password);
  Udp.begin(localPort);
}
////////////////////////////////////
