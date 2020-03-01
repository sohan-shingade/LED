#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
// 
// This example also shows one easy way to define multiple 
#include <FastLED.h>
#include <Wire.h>  // This is the Ard/RIOduino's I2C library.  

#define LED_PIN 5 //DIO pin used for control of LEDs
#define NUM_LEDS 179 //number of LEDs in LED strip
#define LED_TYPE  WS2811
#define COLOR_ORDER GRB
int LEDmode = 2;
int lastMode = 0;
int x = 0;
int count = 0;
boolean mvrt1Setup = false;
boolean blue = false;


int currpattern = 0; // Index number of which pattern is current
int gHue = 0; // rotating "base color" used by many of the patterns

boolean mvrtsetup = false;

CRGB leds[NUM_LEDS];
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {rainbow, MVRT, confetti, MVRT1, Explode};



void setup()  //Arduino Init loop.
{
  delay(3000);
  Serial.begin(9600); //So we can watch serial monitor.  Use lots of print statments when debugging.
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //Wire.begin(8);  //Start I2C object, assigned to port #8.
  FastLED.setBrightness(70);


}

void loop()
{
  //Wire.onReceive(dataReceived); //Looks for data send over I2C, begins when any is detected, no matter the size. 
 
    gPatterns[currpattern]();
    EVERY_N_MILLISECONDS( 20 ) { (gHue++)%360; } // slowly cycle the "base color" through the rainbow
    EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns 
    //Explode();


    //FastLED.delay(700);
    FastLED.show();
    //Serial.println(currpattern);
    // read the state of the pushbutton value:
 

}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end

  currpattern = (currpattern + 1) % 4;

}

void sinelon()
{
  Serial.println(count);
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  if((count%(NUM_LEDS*2)) >= NUM_LEDS){
    leds[pos] = CRGB(55,0,255);

  }
  else{
        leds[pos] = CRGB(255, 200, 0);

  }
  count++;
}


void confetti() 
{
  // random colored speckles that blink in and fade smoothly

  int pos = random16(NUM_LEDS);
  if(random16(10)%2 == 0){
  leds[pos] = CRGB(55,0,255);
  leds[(pos+1)%NUM_LEDS] = CRGB(55,0,255);
  //leds[(pos+2)%NUM_LEDS] = CRGB(55,0,255);
  }
  else{
    leds[pos] = CRGB(255, 200, 0);
    leds[(pos+1)%NUM_LEDS] = CRGB(255, 200, 0);
    //leds[(pos+2)%NUM_LEDS] = CRGB(255, 200, 0);

  }
   fadeToBlackBy( leds, NUM_LEDS, 100);
   FastLED.delay(50);

}

void Explode(){
  int middlenum = middle(NUM_LEDS);

   if(blue){
   turnOff();
   for(int i = 0; i<=middlenum; i++){
      leds[middlenum-i] = CRGB(0, 0, 255);
      leds[middlenum+i] = CRGB(0, 0, 255);
      FastLED.delay(100);
      FastLED.show();
   }
   FastLED.delay(1000);
   }
   else{
    turnOff();
   for(int i = 0; i<=middlenum; i++){
      leds[middlenum-i] = CRGB(255, 0, 0);
      leds[middlenum+i] = CRGB(255, 0, 0);
      FastLED.delay(100);
      FastLED.show();
   }
   FastLED.delay(1000);
   }
  
}

void MVRT1(){
  int middlenum = middle(NUM_LEDS);

 
   turnOff();
   for(int i = 0; i<=middlenum; i++){
      if(i%4 >= 2){
       leds[middlenum-i] = CRGB(55,0,255);
       leds[middlenum+i] = CRGB(255, 200, 0);
      }
      else{
        leds[middlenum-i] = CRGB(255, 200, 0);
         leds[middlenum+i] = CRGB(55,0,255);
      }
      
      
      FastLED.delay(100);
      FastLED.show();

     
  
  
}
      for(int i = 0; i < 8; i++){
        switchLED();
        FastLED.show();
        FastLED.delay(500);
        
      }
}
int middle(int num){
  if(num % 2 == 0){
    return num/2;
  }
  else{
    return (num-1)/2;
  }
}

void rainbow() 
{

  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
  mvrtsetup = false;
}
//MVRT color reel
void MVRT()
{
  if(!mvrtsetup){
  //boolean color = true;
  for(int i = 0; i < NUM_LEDS; i++)
  {
    //for(int j = 0; j < 5; j++)
    //{

      if(i%10>=5) leds[i] = CRGB(55,0,255);
      else leds[i] = CRGB(255, 200,0);
      //if(color) leds[i + j] = CRGB(85,5,117); //suposed to be purple;
      //else leds[i + j] = CRGB(255, 196, 16);
       //supposed to be gold;
      
    //}
    //color = !color;
  }
  mvrtsetup = true;
  }
  else{
  rotate();
  }
     FastLED.show();
 // lastMode = 2;
}

void rotate()
{
  CRGB temp = leds[0];
  for(int i = 0; i < NUM_LEDS-1; i++)
  {
    
    leds[i] = leds[i + 1];
    
    
  }
  leds[NUM_LEDS - 1] = temp;
      FastLED.show();
      FastLED.delay(100);
}
void switchLED(){
  for(int i = 0; i < NUM_LEDS-1; i++)
  {
    
    if(leds[i] == CRGB(55,0,255)){
      leds[i] = CRGB(255, 200,0);
    }
    else{
      leds[i] = CRGB(55,0,255);
    }
    
    
  }
}

  


void turnOff(){
  FastLED.clear();
  FastLED.show();
}
