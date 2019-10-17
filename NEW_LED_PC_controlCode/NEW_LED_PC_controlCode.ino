#include <CircularBuffer.h> //Make sure you install this library from https://github.com/rlogiacco/CircularBuffer
#include <Ultrasonic.h> //Make sure you install this library from https://github.com/ErickSimoes/Ultrasonic
#include <FastLED.h> //Make sure you install this library from https://github.com/FastLED/FastLED
#include <IRremote.h> //Make sure you install this library from http://z3t0.github.io/Arduino-IRremote/
#include <Keyboard.h>

/*
If you don't know how to install libraries, please ask Mr Low or follow this guide: https://www.arduino.cc/en/guide/libraries

This code uses a circular buffer, to store the hues and shift them along the display. Read up on this concept here:
https://en.wikipedia.org/wiki/Circular_buffer
*/

#define LED_PIN 6 //The pin that LEDs are connected to
#define LED_COUNT 120 //The number of LEDs
#define SPL_PIN A0 //The pin that the microphone's SPL pin is connected to
#define IR_PIN 4 //The pin that the IR receiver is connected to

CircularBuffer<int,LED_COUNT> hue; //Set up circular buffer for hue (colour)
CircularBuffer<int,10> bright; //Set up circular buffer for brightness

bool shouldPlay = true;

CRGB leds[LED_COUNT]; //Set up leds[] array
Ultrasonic ultrasonic(12,13); //Trig(er) 12, Echo 13
IRrecv irrecv(4); //Set up IR receiver
decode_results results; //Get decoded results from IR input

int distance; //Distance variable
int spl; //Sound level variable
int brightness;
int brightnessRead;
int hue2;

String volUp = "16754775";
String volDown = "16769055";
String hold = "4294967295";
String prevButton;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, LED_COUNT); //Start comunication with the LEDs
  Serial.begin(9600); //Debug
  irrecv.enableIRIn(); //Begin IR receive process
  //irrecv.blink13(true); //Blink LED on IR input
}

void loop(){
  distance = ultrasonic.read(); //Read distance from the ultrasonic sensor
  spl = analogRead(SPL_PIN); //Read sound level from the mic
  bright.unshift(map(distance, 357, 0, 1, 255)); //Add brighness value to the brightness buffer
  brightness = (bright[0]+bright[1]+bright[2]+bright[3]+bright[4]+bright[5]+bright[6]+bright[8]+bright[9])/10; //averaging of buffer values
  FastLED.setBrightness(brightness);
  hue.unshift(map(spl, 0, 1024, 0, 359)); //Add maped value of spl to the hue buffer
  for(int i = 0; i<120; i++){
    leds[i] = CHSV(hue[i], 255, 255); //Set leds[] to buffer values
  }
  FastLED.show(); //Show it

   if (ultrasonic.read() < 20 and (shouldPlay) ) { //If an object comes close to the rangefinder press space to play video
    Keyboard.press(0x20);
    Keyboard.releaseAll();
    shouldPlay = false;
    }
   else if (ultrasonic.read() > 20 and !(shouldPlay) ) { //If an object leaves the rangefinder press space again to pause video
    Keyboard.press(0x20);
    Keyboard.releaseAll();
    shouldPlay = true;
   }

   if (irrecv.decode(&results)){ //Checks if IR input has been received
        Serial.println(results.value, HEX); //Debug
        Serial.println(String(results.value)); //Debug
        if (String(results.value) == volUp) {
           Keyboard.press(KEY_LEFT_CTRL);
           Keyboard.press(KEY_PAGE_UP);
           Keyboard.releaseAll();
           prevButton = (String(results.value));
        }
        else if (String(results.value) == volDown) {
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press(KEY_PAGE_DOWN);
          Keyboard.releaseAll();
          prevButton = (String(results.value));
        }
        else if (String(results.value) == hold) {
          if (prevButton == volUp) {
           Keyboard.press(KEY_LEFT_CTRL);
           Keyboard.press(KEY_PAGE_UP);
           Keyboard.releaseAll();
         }
        else if (prevButton == volDown) {
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press(KEY_PAGE_DOWN);
          Keyboard.releaseAll();
         }
        }
        irrecv.resume(); //Continue receiving IR input
   }
  delay(100); //Wait 500ms
}
