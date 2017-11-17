#include <math.h>

// color swirl! connect an RGB LED to the PWM pins as indicated
// in the #defines
// public domain, enjoy!
 
#define REDPIN D0
#define GREENPIN D1
#define BLUEPIN D2

#define FADESPEED 5     // make this higher to slow down
int fade_delay = 3;
unsigned long fade_time = millis();

String mode = "hiccup";
String color = "0F0F0F";
int brightness = 100; // percentage value used for calculating brightness
int fadedirection = -1;
int i=0;
double fMultiplier = 0;
double debugvalue;


void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  Particle.function("ledhiccup",ledHiccup);
  Particle.function("ledcycle",ledCycle);
  Particle.function("ledsetvalue",ledSetValue);
  Particle.function("ledsetcolor",ledSetColor);
  Particle.function("ledflash",ledFlash);
  Particle.function("ledfade",ledFade);
  Particle.function("ledsinefade",ledSineFade);
  Particle.function("ledbreathe",ledBreathe);
  
  Particle.variable("LEDMode", mode);
  //Particle.variable("LEDColor", color);
  Particle.variable("Brightness", brightness);
  Particle.variable("fMultiplier", fMultiplier);
  Particle.variable("Debug Value", debugvalue);
  Particle.variable("Fade Dir",fadedirection);
  
}

void loop() {
    if(mode.equalsIgnoreCase("flash")) {
        ledFlash(color);
    } else if (mode.equalsIgnoreCase("cycle")) {
        ledCycle("ignored");
    } else if (mode.equalsIgnoreCase("fade")) {
        ledFade(color);
    } else if (mode.equalsIgnoreCase("sine")) {
        ledSineFade(color);
    } else if (mode.equalsIgnoreCase("breathe")) {
        ledBreathe(color);
    } else if (mode.equalsIgnoreCase("hiccup")) {
        ledHiccup(color);
    }
}

int ledCycle(String fspeed) {
   int r, g, b;
   mode = "cycle";
   color = "CYCLE";
   
   //analogWrite(REDPIN, 0);
   //analogWrite(GREENPIN, 0);
   //analogWrite(BLUEPIN, 0);
 
  // fade from blue to violet
  for (r = 0; r < 256; r++) { 
    analogWrite(REDPIN, r);
    delay(FADESPEED);
  } 
  // fade from violet to red
  for (b = 255; b > 0; b--) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
  // fade from red to yellow
  for (g = 0; g < 256; g++) { 
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
  } 
  // fade from yellow to green
  for (r = 255; r > 0; r--) { 
    analogWrite(REDPIN, r);
    delay(FADESPEED);
  } 
  // fade from green to teal
  for (b = 0; b < 256; b++) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
  // fade from teal to blue
  for (g = 255; g > 0; g--) { 
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
  } 
  
  return EXIT_SUCCESS;
 
}

int ledSetValue(String hexValue) {
    char rHex[2], gHex[2], bHex[2];
    char hexChar[6];
    //int r,g,b;
    
    if (hexValue.length() != 6) {
        //error - need 6-digit hex value
        return EXIT_FAILURE;
    }
    
    mode = "solid";
    color = hexValue;
    
    int r = getR(hexValue);
    int g = getG(hexValue);
    int b = getB(hexValue);
    
    analogWrite(REDPIN, r);
    analogWrite(GREENPIN, g);
    analogWrite(BLUEPIN, b);
    
    return EXIT_SUCCESS;
}

int ledSetColor(String color) {
    mode = "solid";
    
    if (color.equalsIgnoreCase("red")) {
        color = "FF0000";
        analogWrite(REDPIN, 255);
        analogWrite(GREENPIN, 0);
        analogWrite(BLUEPIN, 0);
    } else if (color.equalsIgnoreCase("green")) {
        color = "00FF00";
        analogWrite(REDPIN, 0);
        analogWrite(GREENPIN, 255);
        analogWrite(BLUEPIN, 0);
    } else if (color.equalsIgnoreCase("blue")) {
        color = "0000FF";
        analogWrite(REDPIN, 0);
        analogWrite(GREENPIN, 0);
        analogWrite(BLUEPIN, 255);
    } else if (color.equalsIgnoreCase("yellow")) {
        color = "FFFF00";
        analogWrite(REDPIN, 255);
        analogWrite(GREENPIN, 255);
        analogWrite(BLUEPIN, 0);
    } else if (color.equalsIgnoreCase("orange")) {
        color = "FF8000";
        analogWrite(REDPIN, 255);
        analogWrite(GREENPIN, 128);
        analogWrite(BLUEPIN, 0);
    } else if (color.equalsIgnoreCase("cyan")) {
        color = "00FFFF";
        analogWrite(REDPIN, 0);
        analogWrite(GREENPIN, 255);
        analogWrite(BLUEPIN, 255);
    } else if (color.equalsIgnoreCase("purple")) {
        color = "8000FF";
        analogWrite(REDPIN, 128);
        analogWrite(GREENPIN, 0);
        analogWrite(BLUEPIN, 255);
    } else if (color.equalsIgnoreCase("pink")) {
        color = "FF00FF";
        analogWrite(REDPIN, 255);
        analogWrite(GREENPIN, 0);
        analogWrite(BLUEPIN, 255);
    } else if (color.equalsIgnoreCase("white")) {
        color = "FFFFFF";
        analogWrite(REDPIN, 255);
        analogWrite(GREENPIN, 255);
        analogWrite(BLUEPIN, 255);
    } else if (color.equalsIgnoreCase("off")) {
        color = "000000";
        analogWrite(REDPIN, 0);
        analogWrite(GREENPIN, 0);
        analogWrite(BLUEPIN, 0);
    } else {
        color = "000000";
        analogWrite(REDPIN, 0);
        analogWrite(GREENPIN, 0);
        analogWrite(BLUEPIN, 0);

        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int ledFlash(String hexValue) { // This needs to be rewritten to be non-blocking.
    char rHex[2], gHex[2], bHex[2];
    char hexChar[6];
    mode = "flash";
    color = hexValue;

    if (hexValue.length() != 6) {
        //error - need 6-digit hex value
        return EXIT_FAILURE;
    }
    
    int r = getR(hexValue);
    int g = getG(hexValue);
    int b = getB(hexValue);

    analogWrite(REDPIN, r);
    analogWrite(GREENPIN, g);
    analogWrite(BLUEPIN, b);
    delay(200);
    analogWrite(REDPIN, 0);
    analogWrite(GREENPIN, 0);
    analogWrite(BLUEPIN, 0);
    delay(500);

    
    return EXIT_SUCCESS;

}

int ledFade(String hexValue) {
    char rHex[2], gHex[2], bHex[2];
    char hexChar[6];
    mode = "fade";
    color = hexValue;

    if (hexValue.length() != 6) {
        //error - need 6-digit hex value
        return EXIT_FAILURE;
    }
    if( (fade_time + fade_delay) < millis() ){ // This is a better way to control the delay of the fade. It's non-blocking and should be used by all the methods!
        int r = getR(hexValue);
        int g = getG(hexValue);
        int b = getB(hexValue);
    
        if (brightness >= 100) {
            fadedirection = -1;
        } else if (brightness <= 0) {
            fadedirection = 1;
        }
        brightness = brightness + fadedirection;
        
        r = r * (brightness/100.0);
        g = g * (brightness/100.0);
        b = b * (brightness/100.0);
        
        analogWrite(REDPIN, r);
        analogWrite(GREENPIN, g);
        analogWrite(BLUEPIN, b);
        fade_time = millis();
    }
    
    // if (brightness++ > 255) brightness = 0;
    return EXIT_SUCCESS;
    
}

int ledSineFade(String hexValue) {
    char rHex[2], gHex[2], bHex[2];
    char hexChar[6];
    mode = "sine";
    color = hexValue;
    double fr, fg, fb, x;

    if (hexValue.length() != 6) {
        //error - need 6-digit hex value
        return EXIT_FAILURE;
    }
    
    if( (fade_time + fade_delay) < millis() ){ // This is a better way to control the delay of the fade. It's non-blocking and should be used by all the methods!
        int r = getR(hexValue);
        int g = getG(hexValue);
        int b = getB(hexValue);
    
        if (brightness >=  1000) {
            fadedirection = -1;
        } else if (brightness <= 0) {
            fadedirection = 1;
        }
        brightness += fadedirection;
        x = brightness / 1000.0; // Convert to percentage
        x = x * 2.0 * M_PI; // Convert to radians
    
    // fMultiplier is based on a sine wave that has been shifted up one (to get a value between zero and one)
    //  and pi/2 (90 degrees) to the right. A full wave still happens between zero and 2pi.
        fMultiplier = (sin(x - M_PI/2.0) + 1.0) /2.0;
    
        fr = fMultiplier * r;
        fg = fMultiplier * g;
        fb = fMultiplier * b;

    
        analogWrite(REDPIN, (int)fr);
        analogWrite(GREENPIN, (int)fg);
        analogWrite(BLUEPIN, (int)fb);
        fade_time = millis();    
    }
    
    return EXIT_SUCCESS;
    
}

int ledBreathe(String hexValue) {
    /* Credit for algorithm to Luca Soltoggio - 2015 (NonBlockingBreathingLed 0.1)
        12 May 2015
        http://www.arduinoelettronica.com/
        https://arduinoelectronics.wordpress.com/
        http://minibianpi.wodpress.com/
 
        Use a exp + sin function to recreate a
        non-blocking breathing led effect
 
        Released under GPL v.2 license
        
        Modified on 20171114 by Oliver Dettman to accomodate for RGB LED
          by changing the amplitude of the function to 1 (was 255)
          and using it as a multiplier for each element of the RGB values
    */
    char rHex[2], gHex[2], bHex[2];
    char hexChar[6];
    mode = "breathe";
    color = hexValue;
    double fr, fg, fb;

    if (hexValue.length() != 6) {
        //error - need 6-digit hex value
        return EXIT_FAILURE;
    }
    
    if( (fade_time + fade_delay) < millis() ){ // This is a better way to control the delay of the fade. It's non-blocking and should be used by all the methods!
        int r = getR(hexValue);
        int g = getG(hexValue);
        int b = getB(hexValue);
    
        fade_time = millis();
    
        fMultiplier = (exp(sin(i/2000.0*M_PI*10)) - 0.36787944)*0.425; 
        // this is the math function recreating the effect
        
        fr = fMultiplier * r;
        fg = fMultiplier * g;
        fb = fMultiplier * b;
        
        analogWrite(REDPIN, fr);
        analogWrite(GREENPIN, fg);
        analogWrite(BLUEPIN, fb);
        
        i=i+1;
    }
    
    return EXIT_SUCCESS;
    
    
}

int ledHiccup(String hexValue) {

    char rHex[2], gHex[2], bHex[2];
    char hexChar[6];
    mode = "hiccup";
    color = hexValue;
    double fr, fg, fb;

    if (hexValue.length() != 6) {
        //error - need 6-digit hex value
        return EXIT_FAILURE;
    }
    
    if( (fade_time + fade_delay) < millis() ){ // This is a better way to control the delay of the fade. It's non-blocking and should be used by all the methods!
        int r = getR(hexValue);
        int g = getG(hexValue);
        int b = getB(hexValue);

        fMultiplier = (sin(i/200.0) + cos(3.0*(i/200.0)) + 1.879) / 3.758;  //(exp(sin(i/2000.0*M_PI*10)) - 0.36787944)*0.425; 

        fr = fMultiplier * r;
        fg = fMultiplier * g;
        fb = fMultiplier * b;
        
        analogWrite(REDPIN, fr);
        analogWrite(GREENPIN, fg);
        analogWrite(BLUEPIN, fb);
        
        fade_time = millis();
        i=i+1;
    }
    
    return EXIT_SUCCESS;
    
    
}


int getR(String hexValue) {
    char hex[2];
    char hexChar[6];
    
    // convert String to char[]
    hexValue.toCharArray(hexChar, hexValue.length()+1);
    
    // red component is the first pair of hex digits
    hex[0] = hexChar[0];
    hex[1] = hexChar[1];
    
    int rVal = (int)strtol(hex, NULL, 16);
    return rVal;
}

int getG(String hexValue) {
    char hex[2];
    char hexChar[6];
    
    // convert String to char[]
    hexValue.toCharArray(hexChar, hexValue.length()+1);
    
    // green component is the second pair of hex digits
    hex[0] = hexChar[2];
    hex[1] = hexChar[3];
    
    int rVal = (int)strtol(hex, NULL, 16);
    return rVal;
}

int getB(String hexValue) {
    char hex[2];
    char hexChar[6];
    
    // convert String to char[]
    hexValue.toCharArray(hexChar, hexValue.length()+1);
    
    // blue component is the first pair of hex digits
    hex[0] = hexChar[4];
    hex[1] = hexChar[5];
    
    int rVal = (int)strtol(hex, NULL, 16);
    return rVal;
}


