#include <FastLED.h>

#define DATA_PIN 25

#define NUM_STRIPS 32
#define NUM_LEDS_PER_STRIP 36
#define TOTAL_LED 1200
#define DECALLAGE 6

//NUMLED
int num_led_full_cote = TOTAL_LED / 4;
int num_led_cote = num_led_full_cote - DECALLAGE * 2;

CRGB total_led_array[TOTAL_LED];
CRGBSet total_led(total_led_array, TOTAL_LED);

//COTE 1
CRGBSet cote1full(total_led(0, 299));
CRGBSet cote1(total_led(6, 293));

//COTE 2
CRGBSet cote2full(total_led(300, 599));
CRGBSet cote2(total_led(306, 593));

//COTE 3
CRGBSet cote3full(total_led(600, 899));
CRGBSet cote3(total_led(606, 893));

//COTE 4
CRGBSet cote4full(total_led(900, 1199));
CRGBSet cote4(total_led(906, 1193));

typedef struct carreau {
  byte col;
  byte ligne;
  byte fixture;
  byte R;
  byte G;
  byte B;
  byte intensite;
  byte function;
} carreau;

struct CRGB * fixtures[][] ={}; 

void setup() {
  //Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(total_led_array, TOTAL_LED);
  FastLED.clear(); //Clear existing values
  FastLED.setBrightness(25);

  
}

void loop() {
  
  
  int posx = 2;
  int posy = 3;
  int pos =  map(posx, 0, 8, 0, 300);
  
  fill_solid(*carre, 1200, CRGB::Green);
    FastLED.show();
 /* afficherCoteAvecDelimitation(cote1, CRGB::Blue);
  afficherCoteAvecDelimitation(cote2, CRGB::Green);
  afficherCoteAvecDelimitation(cote3, CRGB::Red);
  afficherCoteAvecDelimitation(cote4, CRGB::White);*/
  FastLED.delay(1000);
}

void afficherCote(int cote, CRGB color) {
  /*for (int i = (TOTAL_LED / 4) * cote + DECALLAGE; i < (TOTAL_LED / 4) * cote + (TOTAL_LED / 4) - DECALLAGE; i++) {
    leds[i] = color;
    }
    FastLED.show();*/
}

void afficherCoteAvecDelimitation(CRGBSet cote, CRGB color) {
  fill_solid(cote, num_led_cote, color);
  /*for(int i = 0; i < num_full_cote; i++){
    if(i % NUM_LEDS_PER_STRIP == 0){
      cote[i] = CRGB::White;
    }
  }*/
  FastLED.show();
}
