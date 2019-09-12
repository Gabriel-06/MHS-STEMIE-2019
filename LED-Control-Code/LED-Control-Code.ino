#include <CircularBuffer.h> //Make sure you install this library from https://github.com/rlogiacco/CircularBuffer
#include <Ultrasonic.h> //Make sure you install this library from https://github.com/ErickSimoes/Ultrasonic
#include <FastLED.h> //Make sure you install this library from https://github.com/FastLED/FastLED

/*
If you don't know how to install libraries, please ask Mr Low or follow this guide: https://www.arduino.cc/en/guide/libraries

This code uses a circular buffer, to store the hues and shift them along the display. Read up on this concept here:
https://en.wikipedia.org/wiki/Circular_buffer

*/

CircularBuffer<int,120> hue; //Set up circular buffer
CircularBuffer<int,5> bright; //Set up circular buffer

#define LED_PIN 6 //The pin that LEDs are connected to
#define LED_COUNT 120 //The number of LEDs
#define SPL_PIN A0 //The pin that the microphone's SPL pin is connected to

CRGB leds[LED_COUNT]; //Set up leds[] array
Ultrasonic ultrasonic(12,13); //Trig(er) 12, Echo 13

int distance; //Distance variable
int spl; //Sound level variable
int brightness;
int brightnessRead;
int hue2;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, LED_COUNT); //Start comunication with the LEDs
  Serial.begin(9600);//Debug
}

void loop(){
  distance = ultrasonic.read(); //Read distance from the ultrasonic sensor
  spl = analogRead(SPL_PIN); //Read sound level from the mic
  brightnessRead = map(distance, 0, 327, 0, 255); //Map the distance to the brightness
  bright.unshift(brightnessRead);
  brightness = (bright[0]+bright[1]+bright[2]+bright[3]+bright[4])/5;
  FastLED.setBrightness(brightness); 
  Serial.println(spl);//Debug
  hue2 = map(spl, 0, 1024, 0, 359); //Map the colour to the spl
  hue.unshift(hue2); //Shift values in buffer 1 to the right
  Serial.println(hue2);//Debug
  for(int i = 0; i<120; i++){
    leds[i] = CHSV(hue[i], 255, 255); //Set leds[] to buffer values
  }
  FastLED.show(); //Show it
  
  delay(100); //Wait 500ms
}
