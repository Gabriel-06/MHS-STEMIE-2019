#include <Ultrasonic.h> //Make sure you install this library from https://www.arduinolibraries.info/libraries/ultrasonic
#include <FastLED.h> //Make sure you install this library from https://github.com/FastLED/FastLED

/*
If you don't know how to install libraries, please ask Mr Low or follow this guide: https://www.arduino.cc/en/guide/libraries

*/

#define LED_PIN 6 //The pin that LEDs are connected to
#define LED_COUNT 120 //The number of LEDs
#define SPL_PIN A0 //The pin that the microphone's SPL pin is connected to

CRGB leds[LED_COUNT];
Ultrasonic ultrasonic(12,13);

int distance; //Distance variable
int spl; //Sound level variable

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, LED_COUNT); //Start comunication with the LEDs
}

void loop() {
  shift(); //Shift the colours one pixel
  readSensorsAndWrite(); //Read data from the sensors and display to the LEDs
  delay(500); //Wait 500ms
}

void shift(){
  for(int i = LED_COUNT; i == 2; i--){
    leds[i-1] = leds[i]; 
  }
}

void readSensorsAndWrite(){
  distance = ultrasonic.read(); //Read distance from the ultrasonic sensor
  spl = analogRead(SPL_PIN); //Read sound level from the mic
  FastLED.setBrightness(map(distance, 0, 50, 255, 0)); //Map the distance to the brightness
  leds[0] = CHSV(map(spl, 0, 1024, 0, 255), 255, 255); //Map the sound level to the hue (colour)
  FastLED.show(); //Show it
}
