//MAC MAITRE 50:02:91:86:88:F0
//PICOx : VERTE : D8:A0:1D:47:70:18
//PICOy : JAUNE : 50:02:91:86:88:A0
//PICO3 : ROUGE  : 50:02:91:86:88:F0

#include <esp_now.h>
#include <WiFi.h>

// Actualisation de 75ms MINIMUM,
// show() à 13hz pour ~2400 LED
#define MIN_REFRESH_TIME 75

byte Njoueur;

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
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add PicoX");
    return;
  }
  memcpy(peerInfo.peer_addr, PICOyAddress, 6);       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add PicoY");
    return;
  }
  memcpy(peerInfo.peer_addr, PICO3Address, 6);       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add Pico3");
    return;
  }
}
 
void loop() {
  //set number of players
  int Njoueur=16;
  int couleurActuelle = 0;
  int r,g,b = 0;
  
  /*whiteAllGrid();
  delay(400);
  blackAllGrid();
  delay(400);*/

    // Set N random cases to send
    for (int i = 0; i < Njoueur; i++){
        switch(couleurActuelle){
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
  
    myData.col=random(8)+1;
    myData.ligne=random(8)+1;
    myData.fixture=1;
    myData.R=r;
    myData.G=g;
    myData.B=b;
    myData.intensite=255;
    myData.fonction=0;

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData)); 
    delay(MIN_REFRESH_TIME);
  }
  blackAllGrid();
  delay(400);
}

void whiteAllGrid(){
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      myData.col=j;
      myData.ligne=i;
      myData.fixture=1;
      myData.R=255; 
      myData.G=0; 
      myData.B=0;
      myData.intensite+=3;
      myData.fonction=0;
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData)); 
      delay(MIN_REFRESH_TIME);
    }
  }

}
void blackAllGrid(){
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      myData.col=j;
      myData.ligne=i;
      myData.fixture=1;
      myData.R=0; 
      myData.G=0; 
      myData.B=0;
      myData.intensite+=3;
      myData.fonction=0;
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
