#include <CircularBuffer.h> //Make sure you install this library from https://github.com/rlogiacco/CircularBuffer
#include <Ultrasonic.h> //Make sure you install this library from https://github.com/ErickSimoes/Ultrasonic
#include <FastLED.h> //Make sure you install this library from https://github.com/FastLED/FastLED

/*
If you don't know how to install libraries, please ask Mr Low or follow this guide: https://www.arduino.cc/en/guide/libraries

This code uses a circular buffer, to store the hues and shift them along the display. Read up on this concept here:
https://en.wikipedia.org/wiki/Circular_buffer

*/

CircularBuffer<int,50> hue; //Set up circular buffer

#define LED_PIN 6 //The pin that LEDs are connected to
#define LED_COUNT 120 //The number of LEDs
#define SPL_PIN A0 //The pin that the microphone's SPL pin is connected to

CRGB leds[LED_COUNT]; //Set up leds[] array
Ultrasonic ultrasonic(12,13); //Trig(er) 12, Echo 13

int distance; //Distance variable
int spl; //Sound level variable

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, LED_COUNT); //Start comunication with the LEDs
}

void loop(){
  distance = ultrasonic.read(); //Read distance from the ultrasonic sensor
  spl = analogRead(SPL_PIN); //Read sound level from the mic
  FastLED.setBrightness(map(distance, 0, 50, 255, 0)); //Map the distance to the brightness
  hue.unshift(map(spl, 0, 1024, 0, 359)); //Shift values in buffer 1 to the right
  for(int i = 0; i<50; i++){
    leds[i] = hue[i]; //Set leds[] to buffer values
  }
  FastLED.show(); //Show it
  
  delay(500); //Wait 500ms
}
