// Control RGB LEDs wired through Particle Photon
// Author: O. Dettman
 
#define REDPIN D0
#define GREENPIN D1
#define BLUEPIN D2

#define FADESPEED 5     // make this higher to slow down

String mode = "flash";
String color = "0F0F0F";
int brightness = 0;

void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  Particle.function("ledcycle",ledCycle);
  Particle.function("ledsetvalue",ledSetValue);
  Particle.function("ledsetcolor",ledSetColor);
  Particle.function("ledflash",ledFlash);
  Particle.function("ledfade",ledFade);
  Particle.variable("LEDMode", mode);
  Particle.variable("LEDColor", color);
}

void loop() {
    if(mode.equalsIgnoreCase("flash")) {
        ledFlash(color);
    } else if (mode.equalsIgnoreCase("cycle")) {
        ledCycle("ignored");
    } else if (mode.equalsIgnoreCase("fade")) {
        ledFade(color);
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
  
  return 1;
 
}

int ledSetValue(String hexValue) {
    char rHex[2], gHex[2], bHex[2];
    char hexChar[6];
    //int r,g,b;
    
    if (hexValue.length() != 6) {
        //error - need 6-digit hex value
        return 0;
    }
    
    mode = "solid";
    color = hexValue;
    
    int r = getR(hexValue);
    int g = getG(hexValue);
    int b = getB(hexValue);
    
    analogWrite(REDPIN, r);
    analogWrite(GREENPIN, g);
    analogWrite(BLUEPIN, b);
    
    return 1;
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

        return 0;
    }
    return 1;
}

int ledFlash(String hexValue) {
    char rHex[2], gHex[2], bHex[2];
    char hexChar[6];
    mode = "flash";
    color = hexValue;

    if (hexValue.length() != 6) {
        //error - need 6-digit hex value
        return 0;
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

    
    return 1;

}

int ledFade(String hexValue) {
    char rHex[2], gHex[2], bHex[2];
    char hexChar[6];
    mode = "fade";
    color = hexValue;

    if (hexValue.length() != 6) {
        //error - need 6-digit hex value
        return 0;
    }
    
    int r = getR(hexValue);
    int g = getG(hexValue);
    int b = getB(hexValue);
    
    if (r > brightness) {
        r = r-brightness;
    } else {
        r=0;
    }

    if (g > brightness) {
        g = g-brightness;
    } else {
        g=0;
    }

    if (b > brightness) {
        b = b-brightness;
    } else {
        b=0;
    }
    
    analogWrite(REDPIN, r);
    analogWrite(GREENPIN, g);
    analogWrite(BLUEPIN, b);
    
    delay(FADESPEED);
    
    if (brightness++ > 255) brightness = 0;
    
    
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
