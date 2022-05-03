//MAC MAITRE 50:02:91:86:88:F0
//PICOx : VERTE : D8:A0:1D:47:70:18
//PICOy : JAUNE : 50:02:91:86:88:A0
//PICO3 : ROUGE  : 50:02:91:86:88:F0

#include <esp_now.h>
#include <WiFi.h>

// Actualisation de 75ms MINIMUM,
// show() à 13hz pour ~2400 LED
#define MIN_REFRESH_TIME 75
#define CASES 8

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t PICOxAddress[] = {0xD8, 0xA0, 0x1D, 0x47, 0x70, 0x18};
uint8_t PICOyAddress[] = {0x50, 0x02, 0x91, 0x86, 0x88, 0xA0};
uint8_t PICO3Address[] = {0x50, 0x02, 0x91, 0x86, 0x88, 0xF0};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  byte col;
  byte ligne;
  byte fixture;
  byte R;
  byte G;
  byte B;
  byte intensite;
  byte fonction;
} struct_message;

// Create a struct_message called myData
struct_message myData;

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
  blackAllGrid();
  randomOfGrid(64);
  delay(500);
}

void whiteAllGrid() {
  for (int i = 1; i <= CASES; i++) {
    for (int j = 1; j <= CASES; j++) {
      myData.col = j;
      myData.ligne = i;
      myData.fixture = 1;
      myData.R = 255;
      myData.G = 255;
      myData.B = 255;
      myData.intensite = 255;
      myData.fonction = 0;
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
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

    myData.col = x;
    myData.ligne = y;
    myData.fixture = 1;
    myData.R = 0;
    myData.G = 204;
    myData.B = 102;
    myData.intensite = 255;
    myData.fonction = 0;

    // Send message via ESP-NOW
    casesFaites[x][y] = true;
    esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
    delay(500);
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

      myData.col = j;
      myData.ligne = i;
      myData.fixture = 1;
      myData.R = r;
      myData.G = g;
      myData.B = b;
      myData.intensite = 255;
      myData.fonction = 0;
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
      delay(MIN_REFRESH_TIME);
    }
  }
}

void blackAllGrid() {
  for (int i = 1; i <= CASES; i++) {
    for (int j = 1; j <= CASES; j++) {
      myData.col = j;
      myData.ligne = i;
      myData.fixture = 1;
      myData.R = 0;
      myData.G = 0;
      myData.B = 0;
      myData.intensite = 0;
      myData.fonction = 0;
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
      delay(MIN_REFRESH_TIME);
    }
  }

}

void afficheTableau() {
  Serial.println(myData.col);
  Serial.println(myData.ligne);
  Serial.println(myData.fixture);
  Serial.println(myData.R);
  Serial.println(myData.G);
  Serial.println(myData.B);
  Serial.println(myData.intensite);
  Serial.println(myData.fonction);
}
