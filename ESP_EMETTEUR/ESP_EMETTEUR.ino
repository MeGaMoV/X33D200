//MAC MAITRE 50:02:91:86:88:F0
//PICOx : VERTE : D8:A0:1D:47:70:18
//PICOy : JAUNE : 50:02:91:86:88:A0
//PICO3 : ROUGE  : 50:02:91:86:88:F0
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

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
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(9600);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, PICOxAddress, 6);
  //peerInfo.channel = 0;  
  //peerInfo.encrypt = false;  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
    memcpy(peerInfo.peer_addr, PICOyAddress, 6);       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
      memcpy(peerInfo.peer_addr, PICO3Address, 6);       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  //set number of players
  Njoueur=8;
  
  //set every red cases to send
  for (int i = 1; i <= 8; i++){
    for (int j = 1; j <= 8; j++){
  myData.col=i;
  myData.ligne=j;
  myData.fixture=1;
  myData.R=255; 
  myData.G=0; 
  myData.B=0;
  myData.intensite=255;
  myData.fonction=0;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData)); 
  /*
  if (result == ESP_OK) {Serial.println("Sent with success");}
  else {Serial.println("Error sending the data");}
  */
  delay(10);
  }
  }
  
  // Set N random cases Green to send
  for (int i = 1; i <= 8; i++){
  myData.col=random(8)+1;
  myData.ligne=random(8)+1;
  myData.fixture=1;
  myData.R=0; 
  myData.G=255; 
  myData.B=0;
  myData.intensite=255;
  myData.fonction=0;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData)); 
  if (result == ESP_OK) { Serial.println("Sent with success");}
  else { Serial.println("Error sending the data"); }
  delay(100);
  }
  delay(5000);
  
}

//__26/04/2022________________________________________________________________
//MAC MAITRE 50:02:91:86:88:F0
//PICOx : VERTE : D8:A0:1D:47:70:18
//PICOy : JAUNE : 50:02:91:86:88:A0
//PICO3 : ROUGE  : 50:02:91:86:88:F0

#include <esp_now.h>
#include <WiFi.h>

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
  if (esp_now_init() != ESP_OK) {
    return;
  }
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
  Njoueur=8;
  whiteAllGrid();
  delay(400);
  blackAllGrid();
  delay(400);

  //afficheTableau();
  /*
  //set every red cases to send
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      myData.col=i;
      myData.ligne=j;
      myData.fixture=1;
      myData.R=255; 
      myData.G=0; 
      myData.B=0;
      myData.intensite=255;
      myData.fonction=0;
      
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData)); 
      delay(10);
    }
  }
  delay(100);
  afficheTableau();
  
  // Set N random cases Green to send
  for (int i = 0; i < 8; i++){
    myData.col=random(8)+1;
    myData.ligne=random(8)+1;
    myData.fixture=1;
    myData.R=0; 
    myData.G=255; 
    myData.B=0;
    myData.intensite=255;
    myData.fonction=0;
    
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData)); 
    delay(10);
  }
  */
  //delay(5000);
  
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
    delay(75);// actualisation de 75ms MINIMUM, show() à 13 ips pour ~2400 LED
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
      delay(75);

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
