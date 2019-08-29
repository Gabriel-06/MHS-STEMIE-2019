#include <Ultrasonic.h>
#include <FastLED.h>


#define LED_PIN 6
#define LED_COUNT 120
#define SPL_PIN A0

CRGB leds[LED_COUNT];
Ultrasonic ultrasonic(12,13);
int distance;
int spl;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, LED_COUNT);
}

void loop() {
  shift();
  readSensorsAndWrite();
  delay(500);
}

void shift(){
  for(int i = LED_COUNT; i == 2; i--){
    leds[i-2] = leds[i-1]; 
  }
}

void readSensorsAndWrite(){
  distance = ultrasonic.read();
  spl = analogRead(SPL_PIN);
  FastLED.setBrightness(map(distance, 0, 50, 255, 0));
  leds[0] = CHSV(map(spl, 0, 1024, 0, 255), 255, 255);
  FastLED.show();
}
