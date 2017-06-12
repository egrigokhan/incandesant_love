/*
 * MORSE CODE DECODER USING LDR
 * 
 * created 12.06.2017
 * by Gökhan Eğri
 * 
 * *************************************PIN SET*******************************************                
 * LED -Vcc-> 12
 *     -GND-> GND
 *     
 * LDR --> +5V
 *     --> A3
 *     
 * 100K RESISTOR --> GND
 *               --> A3
 *               
 * LCD* -GND-> GND 
 *      -Vcc-> +5V
 *      -SDA-> A4
 *      -SCL-> A5
 *      
 *      *The LCD module used in this project was pre-equipped with an I2C backpack the   
 *      pin structure of which is given above. If you are working with a regular display, 
 *      please connect it in the 16-pin formation as seen in                             
 * ***************************************************************************************
 * 
 * 
 */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//PINS
int sensorPin  = A3;    // the pin for LDR
int ledPin     = 12;    // the pin for the indicator LED

//VARIABLES
int sensorValue  = 0;    // variable to store the value coming from the sensor
int binVal       = 0;    // the current binary value

String result  = "";    // the resulting natural language String
String temp    = "";    // the dash-dot representation of individual inputs

//CONSTANTS
int MORSE_TIME_LONG    = 20;    // below-delimiter for DASH ~MAY VARY
int MORSE_TIME_SHORT   = 10;    // above-delimiter for DOT ~MAY VARY
int MORSE_TIME_DELETE  = 50;    // below-delimiter for CLEAR - keeping the light on for a long time clears the entire screen ~MAY VARY

int LIGHT_THRESHOLD  = 1000;    // > LIGHT_THRESHOLD --> 1 || < LIGHT_THRESHOLD --> 0 ~MAY VARY 

/*
 * 0x27 --> THE ADDRESS OF THE I2C MODULE ~MAY VARY
 * 20   --> COLUMN COUNT ~MAY VARY
 * 4    --> ROW COUNT ~MAY VARY
 * 
 * ALTER ACCORDINGLY
 */
LiquidCrystal_I2C lcd(0x27, 20, 4);

//The Morse[dot-dash] representations of the English alphabet with additional commands ~can be expanded at will
String morseABC[] =       {".-",   //A
                           "-...", //B
                           "-.-.", //C
                           "-..",  //D
                           ".",    //E
                           "..-.", //F
                           "--.",  //G
                           "....", //H
                           "..",   //I
                           ".---", //J
                           "-.-",  //K
                           ".-..", //L
                           "--",   //M
                           "-.",   //N
                           "---",  //O
                           ".--.", //P
                           "--.-", //Q
                           ".-.",  //R
                           "...",  //S
                           "-",    //T
                           "..-",  //U
                           "...-", //V
                           ".--",  //W
                           "-..-", //X
                           "-.--", //Y
                           "--..", //Z
                           "....." //' '
                           "..--"  //'<-'
                          };

//the English alphabet
char regularABC[] =       {'A', 
                           'B', 
                           'C', 
                           'D', 
                           'E', 
                           'F', 
                           'G', 
                           'H', 
                           'I', 
                           'J', 
                           'K', 
                           'L', 
                           'M', 
                           'N', 
                           'O', 
                           'P', 
                           'Q', 
                           'R', 
                           'S', 
                           'T', 
                           'U', 
                           'V', 
                           'W', 
                           'X', 
                           'Y', 
                           'Z',
                           ' '
                          };

void setup() {
  
  Serial.begin(250000);  // serial-port set-up ~baud rate MAY BE CHANGED, however then the MORSE_TIME* constants above MUST ALSO CHANGE
  
  pinMode(ledPin, OUTPUT); 
  
  lcd.begin();      // starting LCD
  lcd.backlight();  // turning on backlight
  
} // END OF SETUP

void loop() {

  // Initial LDR read
  sensorValue = analogRead(sensorPin);

  //Converting to 0/1 based on light intensity
  binVal = convertToBin(sensorValue);

  //Serial.println(binVal);  // ~for debugging purposes

  durationLoop(binVal); // calling durationLoop(int) method with current bitVal

} // END OF LOOP

/*
 * Converts given sensor reading to binary with respect to the LIGHT_THRESHOLD declared above.
 * 
 * @param val Sensory reading value to be converted (int)
 * @return 0 || 1 (int)
 */
int convertToBin( int val ) {
  
  if( val > LIGHT_THRESHOLD ) {
    digitalWrite(ledPin, HIGH);  // INDICATOR LIGHT --> ON
    return 1;
  }

  else {
    digitalWrite(ledPin, LOW);  // INDICATOR LIGHT --> OFF
    return 0;
  }
}

/*
 * Counts the amount of time spent at a given binary state(0-1).
 * 
 * @param binState 0 || 1 (int)
 */
void durationLoop( int binState ) {

  //Reading sensor value from LDR
  sensorValue = analogRead(sensorPin);

  //Converting sensorValue to binary
  binVal = convertToBin(sensorValue);

  //Initializing time to 0
  int time = 0;

  //While the LDR holds its state
  while( binVal == binState ) {

    time += 1;  // incrementing time by 1

    //If MORSE_TIME_LONG exceeded when 0, interpret as end of current pattern ie. charEnd
    if(binState == 0 && time >=  MORSE_TIME_LONG) {
      break;
    }

    
    //Reading sensor value from LDR
    sensorValue = analogRead(sensorPin);

    //Converting sensorValue to binary
    binVal = convertToBin(sensorValue);
    
    delay(50);  // Mandatory delay for normalizing time data ~MAY VARY
  }
  
  //Serial.println(time);  // for debugging purposes
  
  morsify(binState, time); // morsify the obtained pattern(DOT, DASH or CHAREND)
}

/*
 * Transforms dash-dot patterns into natural language Strings and updates the LCD.
 * 
 * @param binState 0 || 1 (int) 
 * @param time Time spent at the supplied binary state (int)
 */
void morsify(int binState, int time) {

  if(binState == 1) {

    //LONG --> DASH
    if( time >=  MORSE_TIME_LONG && time <= MORSE_TIME_DELETE ) {
      temp += '-';
    }

    //SHORT --> DOT
    else if( time <= MORSE_TIME_SHORT ) {
      temp += '.';
    }

    //TOO LONG --> CLEAR
    else if( time > MORSE_TIME_DELETE ) {
      result = "";
      temp = "";
    }
  }

  //At 0 state for LONG, interpret as CHAREND and evaluate the pattern at hand(temp)
  else if(binState == 0 && time >= MORSE_TIME_LONG) {

    /*
     * 28 --> LENGTH OF MORSEABC ARRAY, CHANGE IF YOU ADD EXTRA COMMANDS
     */
    for(int i = 0; i < 28; i++) {
      if(temp.equals(morseABC[i])) {

        //BACKSPACE - delete last character
        if(i == 27) {
          result = result.substring(0, result.length() - 2);
          break;
        }

        //SPACE - single whitespace
        else if(i == 26) {
          result = result + " ";
        }
        
        result += regularABC[i];
        break;
      }
    }

    //Serial.println(temp); // for debugging purposes

    //Clearing LCD
    lcd.clear();

    //Updating with new result String
    for(int i = 0; i < result.length(); i++) {
      lcd.print(result[i]);
    }

    //Emptying temp for the next iteration
    temp = ""; 
  }
}

