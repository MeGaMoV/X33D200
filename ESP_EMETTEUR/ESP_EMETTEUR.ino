//MAC MAITRE 50:02:91:86:88:F0
//PICOx : VERTE : D8:A0:1D:47:70:18
//PICOy : JAUNE : 50:02:91:86:88:A0
//PICO3 : ROUGE  : 50:02:91:86:88:F0

#include <esp_now.h>
#include <WiFi.h>

// Actualisation de 50ms minimum
#define MIN_REFRESH_TIME 50
#define CASES 8

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t PICOxAddress[] = {0xD8, 0xA0, 0x1D, 0x47, 0x70, 0x18};
uint8_t PICOyAddress[] = {0x50, 0x02, 0x91, 0x86, 0x88, 0xA0};
uint8_t PICO3Address[] = {0x50, 0x02, 0x91, 0x86, 0x88, 0xF0};

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

// Create a struct_message called 
struct_message espnow_msg;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
}

void setup() {
  // Init Serial Monitor
  Serial.begin(9600);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) return;
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, PICOxAddress, 6);
  //peerInfo.channel = 0;
  //peerInfo.encrypt = false;
  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add PicoX");
    return;
  }
  memcpy(peerInfo.peer_addr, PICOyAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add PicoY");
    return;
  }
  memcpy(peerInfo.peer_addr, PICO3Address, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add Pico3");
    return;
  }
}

void loop() {
  blueMatrice();
  delay(1000);
  blackMatrice();
  delay(1000);
}

void blueMatrice(){
  for(short y = 0; y < 8; y++){
    for(short x = 0; x < 8; x++){
      for(short f = 0; f <4; f++){
        espnow_msg.matrice[x][y].fixtures[f].r = 0;
        espnow_msg.matrice[x][y].fixtures[f].g = 0;
        espnow_msg.matrice[x][y].fixtures[f].b = 255;
      }
    }
  }
  esp_now_send(0, (uint8_t *) &espnow_msg, sizeof(espnow_msg));
}

void blackMatrice(){
  for(short y = 0; y < 8; y++){
    for(short x = 0; x < 8; x++){
      for(short f = 0; f <4; f++){
        espnow_msg.matrice[x][y].fixtures[f].r = 0;
        espnow_msg.matrice[x][y].fixtures[f].g = 0;
        espnow_msg.matrice[x][y].fixtures[f].b = 0;
      }
    }
  }
  esp_now_send(0, (uint8_t *) &espnow_msg, sizeof(espnow_msg));
}

/*
void caseMusicale(int nbJoueurs){
  for(int i = nbJoueurs; i > 0; i--){
    blackAllGrid();
    delay(3000);
    randomOfGrid(i);
    delay(5000);
  }
}

void whiteAllGrid() {
  for (int i = 1; i <= CASES; i++) {
    for (int j = 1; j <= CASES; j++) {
      espnow_msg.col = j;
      espnow_msg.ligne = i;
      espnow_msg.fixture = 1;
      espnow_msg.R = 255;
      espnow_msg.G = 255;
      espnow_msg.B = 255;
      espnow_msg.intensite = 255;
      espnow_msg.fonction = 0;
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(0, (uint8_t *) &espnow_msg, sizeof(espnow_msg));
      delay(MIN_REFRESH_TIME);
    }
  }
}

void randomOfGrid(short nCasesAleatoires) {
  short couleurActuelle = 0;
  short r, g, b = 0;
  bool casesFaites[9][9]; //9 même si on utilise que de 1 à 8
  short x = random(8) + 1;
  short y = random(8) + 1;

  //Reinitialiser le tableau des cases faites
  for(int resetY = 0; resetY < 9; resetY++){
    for(int resetX = 0; resetX < 9; resetX++){
      casesFaites[resetX][resetY] = false;
    }
  }

  // Set N random cases to send
  for (int i = 1; i <= nCasesAleatoires; i++) {
    do{
      x = random(8) + 1;
      y = random(8) + 1;
    } while(casesFaites[x][y] == true);

    switch (couleurActuelle) {
      case 0:
        r = random(255);
        g = random(25);
        b = random(50);
        couleurActuelle++;
        break;
      case 1:
        r = random(25);
        g = random(255);
        b = random(50);
        couleurActuelle++;
        break;
      case 2:
        r = random(25);
        g = random(50);
        b = random(255);
        couleurActuelle = 0;
        break;
    }

    espnow_msg.col = x;
    espnow_msg.ligne = y;
    espnow_msg.fixture = 1;
    espnow_msg.R = 0;
    espnow_msg.G = 0;
    espnow_msg.B = 255;
    espnow_msg.intensite = 255;
    espnow_msg.fonction = 0;

    // Send message via ESP-NOW
    casesFaites[x][y] = true;
    esp_err_t result = esp_now_send(0, (uint8_t *) &espnow_msg, sizeof(espnow_msg));
  }
}

void rgbAllGrid() {
  short r, g, b = 0;
  short couleurActuelle = 0;

  for (int i = 1; i <= CASES; i++) {
    for (int j = 1; j <= CASES; j++) {
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

      espnow_msg.col = j;
      espnow_msg.ligne = i;
      espnow_msg.fixture = 1;
      espnow_msg.R = r;
      espnow_msg.G = g;
      espnow_msg.B = b;
      espnow_msg.intensite = 255;
      espnow_msg.fonction = 0;
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(0, (uint8_t *) &espnow_msg, sizeof(espnow_msg));
      delay(MIN_REFRESH_TIME);
    }
  }
}

void blackAllGrid() {
  for (int i = 1; i <= CASES; i++) {
    for (int j = 1; j <= CASES; j++) {
      espnow_msg.col = j;
      espnow_msg.ligne = i;
      espnow_msg.fixture = 1;
      espnow_msg.R = 0;
      espnow_msg.G = 0;
      espnow_msg.B = 0;
      espnow_msg.intensite = 0;
      espnow_msg.fonction = 0;
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(0, (uint8_t *) &espnow_msg, sizeof(espnow_msg));
      delay(MIN_REFRESH_TIME);
    }
  }
}*/
