#include <Arduino.h>
#include "WiFi.h" 
#include "AsyncUDP.h" //Importing necessary libraries to be able to excute our code 

int pinX = 36; // Assigns a pin number to the variable
int pinY = 39; // Assigns a pin number to the variable

unsigned long currentMillis; // Assigns a current value to the timer function
unsigned long previousMillis = 0; //will store the last time an interval was hit
int interval = 1000; // Assigns an interval value to be used in the timer funciton
int port = 7000; // Assigns a port to be broadcast to

const char * ssid = "WaiFiFu"; //WiFi name. It varies depending on the user :)
const char * password = "2nerds&1cat"; //WiFi Password.. THIS IS SECRET DON'T LOOK

AsyncUDP udp; // Creates AsyncUDP object

void setup() 
{   Serial.begin(9600); //The program... begins :D 
    WiFi.mode(WIFI_STA); //Connects to the WiFi
    WiFi.begin(ssid, password); // Checks the WiFi name and password 
    pinMode(pinX, INPUT); //right/left
    pinMode(pinY, INPUT); //up/down
    pinMode(13, INPUT_PULLUP); //initializing digital pin 13 as an input with the internal pull up resistor enabled
}

void movement(String direction, String printing){ //Broadcasts the move function to the pxlserver
    udp.broadcastTo( direction.c_str(), port); //.c_str() is needed to parse the direction input into a char array, which is needed for the broadcastTo funciton to work.
    Serial.print(printing); //Prints the direction we're moving in the serial monitor
}

void movePixel(int horizontal, int vertical){
    if(horizontal == 0){ //When the joystick is pushed completely to the left, the pixel will move left
       movement("moveleft", "moving left");
    } else if(horizontal == 4095){ //When the joystick is pushed completely to the right, the pixel will move right
        movement("moveright", "moving right"); 
    } else if(vertical == 0){ //When the joystick is pushed completely up, the pixel will move up
       movement("moveup", "moving up");
    } else if(vertical == 4095){ //When the joystick is pushed completely down, the pixel will move down
        movement("movedown", "moving down");
    } else {
        udp.broadcastTo("stop",port); //If the joystick is in a neutral position, the pixel will stop
    }
}


void loop(){
    currentMillis = millis();
    if(digitalRead(13) == LOW){ //LOW means pressed, HIGH means not pressed.
            udp.broadcastTo("init 6 6", port); // Initialise a pixel by pressing the joystick
    }
    if(currentMillis - previousMillis > interval){ //By using an interval, the commands will only be read once every interval.
        movePixel(analogRead(pinX), analogRead(pinY)); //This allows the movePixel() function to read the input from the joystick
        previousMillis = currentMillis; //This makes sure that the interval will repeat.
    }
}

