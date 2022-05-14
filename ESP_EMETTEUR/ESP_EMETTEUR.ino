#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

// Delay minimum entre deux messages OSC
#define MIN_OSC_DELAY 15
#define CASES 8

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
typedef struct struct_message {
  carre matrice[8][8];
  byte intensite;
  byte fonction;
} struct_message;

WiFiUDP Udp;
const char* ssid = "Private";
const char* password = "zaza4014";

const IPAddress PicoX(192, 168, 0, 103);
const IPAddress PicoY(192, 168, 0, 108);
const IPAddress Pico3(192, 168, 0, 254);

const unsigned int port = 8888; //Port de reception des informations sur les Picos
const unsigned int localPort = 8888; // Port local pour écouter en OSC sur cet emetteur

// Tableau de carré
carre matrice[8][8] = {};

//-----------------------------SETUP-------------------------------------

void setup() {
  Serial.begin(115200);
  
  WiFi.disconnect(true);
  WiFi.onEvent(Wifi_connected, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(Get_IPAddress, SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(Wifi_disconnected, SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.begin(ssid, password);
  Udp.begin(localPort);
}

void loop() {
  rgbMatrice();
  envoyerIntensite(25);
  envoyerMatrice();
  envoyerPrint();

  delay(2000);
  
  blackMatrice();
  envoyerClear();
  delay(1000);
}

/////////////////// FONCTIONS D'ENVOI OSC /////////////////
void envoyerFixture(short x, short y, short fixture){
  char laTrame[15];
  uint8_t donnee[3];
  
  sprintf(laTrame, "/fixture/%d/%d/%d", x, y, fixture);
  OSCMessage msg(laTrame);

  donnee[0] = matrice[x - 1][y - 1].fixtures[fixture].r;
  donnee[1] = matrice[x - 1][y - 1].fixtures[fixture].g;
  donnee[2] = matrice[x - 1][y - 1].fixtures[fixture].b;
  
  msg.add(donnee, sizeof(donnee));
  Udp.beginPacket(PicoX, port);
  msg.send(Udp);
  Udp.endPacket();

  Udp.beginPacket(PicoY, port);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}

void envoyerCarre(short x, short y){
  char laTrame[11];
  uint8_t donnee[12];
  short posTotale = 0;
  
  sprintf(laTrame, "/carre/%d/%d", x, y);
  OSCMessage msg(laTrame);

  for(short tmpFix = 0; tmpFix < 4; tmpFix++){
    donnee[posTotale] = matrice[x - 1][y - 1].fixtures[tmpFix].r;
    donnee[posTotale + 1] = matrice[x - 1][y - 1].fixtures[tmpFix].g;
    donnee[posTotale + 2] = matrice[x - 1][y - 1].fixtures[tmpFix].b;
    posTotale += 3;
  }
  
  msg.add(donnee, sizeof(donnee));
  Udp.beginPacket(PicoX, port);
  msg.send(Udp);
  Udp.endPacket();

  Udp.beginPacket(PicoY, port);
  msg.send(Udp);
  Udp.endPacket();
  
  msg.empty();
}

void envoyerMatrice(){
  for(short y = 1; y <= 8; y++){
    for(short x = 1; x <= 8; x++){
      envoyerCarre(x,y);
      delay(MIN_OSC_DELAY);
    }
  }
}

void envoyerClear(){
  OSCMessage msg("/clear");
  Udp.beginPacket(PicoX, port);
  msg.send(Udp);
  Udp.endPacket();

  Udp.beginPacket(PicoY, port);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}

void envoyerPrint(){
  OSCMessage msg("/print");
  Udp.beginPacket(PicoX, port);
  msg.send(Udp);
  Udp.endPacket();

  Udp.beginPacket(PicoY, port);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}

void envoyerIntensite(int intensite){
  OSCMessage msg("/intensite");
  msg.add(intensite);
  Udp.beginPacket(PicoX, port);
  msg.send(Udp);
  Udp.endPacket();

  Udp.beginPacket(PicoY, port);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}

////////// FONCTIONS ALLUMAGE MATRICE //////////
void redMatrice(){
  for(short y = 0; y < 8; y++){
    for(short x = 0; x < 8; x++){
      for(short f = 0; f <4; f++){
        matrice[x][y].fixtures[f].r = 255;
        matrice[x][y].fixtures[f].g = 0;
        matrice[x][y].fixtures[f].b = 0;
      }
    }
  }
}

void blueMatrice(){
  for(short y = 0; y < 8; y++){
    for(short x = 0; x < 8; x++){
      for(short f = 0; f <4; f++){
        matrice[x][y].fixtures[f].r = 0;
        matrice[x][y].fixtures[f].g = 0;
        matrice[x][y].fixtures[f].b = 255;
      }
    }
  }
}

void greenMatrice(){
  for(short y = 0; y < 8; y++){
    for(short x = 0; x < 8; x++){
      for(short f = 0; f <4; f++){
        matrice[x][y].fixtures[f].r = 0;
        matrice[x][y].fixtures[f].g = 255;
        matrice[x][y].fixtures[f].b = 0;
      }
    }
  }
}

void whiteMatrice(){
  for(short y = 0; y < 8; y++){
    for(short x = 0; x < 8; x++){
      for(short f = 0; f <4; f++){
        matrice[x][y].fixtures[f].r = 255;
        matrice[x][y].fixtures[f].g = 255;
        matrice[x][y].fixtures[f].b = 255;
      }
    }
  }
}

void blackMatrice(){
  for(short y = 0; y < 8; y++){
    for(short x = 0; x < 8; x++){
      for(short f = 0; f <4; f++){
        matrice[x][y].fixtures[f].r = 0;
        matrice[x][y].fixtures[f].g = 0;
        matrice[x][y].fixtures[f].b = 0;
      }
    }
  }
}

void rgbMatrice(){
  short r, g, b = 0;
  short couleurActuelle = 0;

  for (int y = 1; y <= CASES; y++) {
    for (int x = 1; x <= CASES; x++) {
      switch (couleurActuelle) {
        case 0:
          r = 255;
          g = 0;
          b = 0;
          couleurActuelle++;
          break;
        case 1:
          r = 0;
          g = 255;
          b = 0;
          couleurActuelle++;
          break;
        case 2:
          r = 0;
          g = 0;
          b = 255;
          couleurActuelle = 0;
          break;
      }

      for(short tmpFix = 0; tmpFix <= 3; tmpFix++){
        matrice[x - 1][y - 1].fixtures[tmpFix].r = r;
        matrice[x - 1][y - 1].fixtures[tmpFix].g = g;
        matrice[x - 1][y - 1].fixtures[tmpFix].b = b;
      }
    }
  }
}
////////////////////////////////////////////////////////


/////////////////// FONCTIONS WIFI ///////////////////
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
