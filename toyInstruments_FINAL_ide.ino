//"Toy Instruments"
//For 2024 MT Capstone
//Rae Quarles - 5/9/24
//V 2.5 FINAL!!

//Using code from: https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all
//This code uses a series of photocells set up in voltage dividers with a 10k resistors
  //Photocell connected to V-, resistor connected to V++, pin connected between them
  //VALUE GOES UP AS LIGHT GOES DOWN (THERE IS MORE RESISTANCE), AND DOWN AS LIGHT INCREASES (LOWER RESISTANCE)
//And sends it to Processing via serial
//Depending on the press of 3 different buttons

//BUTTON MODES: CHANGE THE PATCH DEPENDING ON WHAT COLOR YOU WANT TO BE EACH MODE!
  //MODE 1 - CHORD
    //ButtonPress: 0 = buttonMode, 1 = noteFreq, 2 = amplitude
    //Pass:  0 = noteFreq, 1 = amp, 2 = buttonMode
  //MODE 2 - DETUNE
    //ButtonPress: 0 = buttonMode, 1 = noteFreq
    //Pass: 0 = detune amount (MUST BE /100), 1 = noteFreq, 2 = buttonMode
  //MODE 3 - SEQUENCER
    //ButtonPress: 0 = buttonMode (this doesn't need to be sent to processing :) )
    //Pass: 0 = noteFreq, 1 = amplitude, 2 = buttonMode

//2.5 Update:
  //Have a good presentation :) you'll do great 
  
const int pin1 = A7;                      //These are the pins that are getting the voltage
const int pin2 = A6;
const int pin3 = A5;
const int pin4 = A4;
const int pin5 = A3;
const int pin6 = A2;
const int pin7 = A1;
const int pin8 = A0;
const int butPin1 = 2;                    //These are the pin the buttons are connected to
const int butPin2 = 4;
const int butPin3 = 6;
const int ledPin1 = 11;                    //Pins the LEDs are connected to
const int ledPin2 = 10;
const int ledPin3 = 9;
const int ledPin4 = 8;
const int ledPin5 = 7;
const int ledPin6 = 5;
const int ledPin7 = 3;


int photo1 = 0;                           //This var stores the value of the reading of the first photocell
int photo2 = 0;
int photo3 = 0;
int photo4 = 0;
int photo5 = 0;
int photo6 = 0;
int photo7 = 0;
int photo8 = 0;
int button1 = 0;                          //the current reading of the buttons
int button2 = 0;
int button3 = 0; 

int start1 = 0;                           //Stores the inital photovalue of each cell
int start2 = 0;
int start3 = 0;
int start4 = 0;
int start5 = 0;
int start6 = 0;
int start7 = 0;
int start8 = 0;

int low1 = 882;                           //INPUT THE LOWS FROM CALLIBRATION FILE HERE
int low2 = 863;                           //Stores the value when the car is directly over (lowest light amount and highest resistance)
int low3 = 985;
int low4 = 992;
int low5 = 948;
int low6 = 929;
int low7 = 1001;
int low8 = 982;

int lastPass = 0;                         //Last time a car passed (for millis)
int lastPress = 0;                        //Last time a button was pressed (for millis)
int msgSpeed = 100;  //*CHANGE FOR MSG SPEED* //Speed for sending messages (100ms)
int buttonSpeed = 1000;                   //Speed for check  ing the buttons (1s)
int currentMillis = 0;                    //millis <3                             
int buttonMode = 0;                       //which button mode we're currently on **CHANGE THIS TO MANUALLY CHANGE MODE**

float octaveArray[] = {65.41, 130.81, 261.63, 523.25}; //An array of pitches of C in each octave
float pitchArray[] = {130.81, 146.83, 164.81, 174.61, 196.00, 220, 246.94, 261.63};             //An array of pitches from the C major scale in the third octave
float jtuneArray[] = {1.0666, 1.1250, 1.2000, 1.2500, 1.3333, 1.5000, 1.6666, 1.8750, 2.0000};  //An array of ratios for partials within an octave using Just Tuning
float randOctave = 0.0000;                //Will hold the randomly chosen octave from the array
float randPitch = 0.00;                   //Will hold the randomly chosen pitch from the array
float randPartial = 0.0000;               //Will hold the randomly chosen partial of the pitch from randOctave
int seqNotes[7];                          //Will hold all the sequence notes for mode 3
float amplitude = 0;                      //The amp of the "chord"

void setup() 
{
Serial.begin(9600);                       //Open up the serial port
                     
pinMode(butPin1, INPUT);                  //Set all our button pins to input
pinMode(butPin2, INPUT);
pinMode(butPin3, INPUT);

pinMode(ledPin1, OUTPUT);                 //Set all the LED pins to output
pinMode(ledPin2, OUTPUT);
pinMode(ledPin3, OUTPUT);
pinMode(ledPin4, OUTPUT);
pinMode(ledPin5, OUTPUT);
pinMode(ledPin6, OUTPUT);
pinMode(ledPin7, OUTPUT);

start1 = analogRead(pin1);                //Set the inital value of the photocells to callibrate later
start2 = analogRead(pin2);
start3 = analogRead(pin3);
start4 = analogRead(pin4);
start5 = analogRead(pin5);
start6 = analogRead(pin6);
start7 = analogRead(pin7);
start8 = analogRead(pin8);
}

void loop(){
currentMillis = millis();                  //Refresh the millis reading

photo1 = analogRead(pin1);                 //Read the value of the photocells
photo2 = analogRead(pin2);  
photo3 = analogRead(pin3);
photo4 = analogRead(pin4);
photo5 = analogRead(pin5);
photo6 = analogRead(pin6);
photo7 = analogRead(pin7);
photo8 = analogRead(pin8);
button1 = digitalRead(butPin1);            //Read the state of the buttons
button2 = digitalRead(butPin2);
button3 = digitalRead(butPin3);     

//CHECK THE BUTTONS FIRST TO SET MODE:
if (currentMillis - lastPress > buttonSpeed) {        //If it's time to check the buttons:
  if (button1 == HIGH){                               //If someone pressed the green button (button1)
    buttonMode = 1;                                   //Update buttonMode to 1
    randOctave = octaveArray[random(4)];              //Pick a random octave each time the button is pressed
    amplitude = map(randOctave, 65.41, 523.25, 150, 80); //Scale that octave value so low notes have a higher amplitude
    Serial.print("1:");                               //Send a serial msg "1" to Processing, where it will trigger a drone in Supercollider
    Serial.print(randOctave); Serial.print(":");      //And then send the pitch
    Serial.println(amplitude);                        //And send the amplitude too!
    lastPress = millis();                             //Update millis
  }
  if (button2 == HIGH) {                              //If someone pressed the yellow button (button2)
    buttonMode = 2;                                   //Update buttonMode to 2
    randPitch = pitchArray[random(8)];                //Pick a random pitch each time the button is pressed
    Serial.print("2:");                               //Send a serial msg "2" to Processing, where it will trigger a drone in Supercollider
    Serial.print(randPitch);                          //And send the pitch to change the drone
    Serial.println(":");                              //We need 3 spots in the array for processing to work lmaoo
    lastPress = millis();                             //Update millis()
  }
  if (button3 == HIGH) {                              //If someone pressed the red button (button3)
    buttonMode = 3;                                   //Update buttonMode to 2
    randOctave = octaveArray[random(4)];              //Pick a random octave each time the button is pressed
    amplitude = map(randOctave, 65.41, 523.25, 150, 80);
    seqNotes[0]= randOctave * jtuneArray[random(9)];  //Generate a random partial off of the random pitch chosen when button was pressed and set it as the first note
    seqNotes[1]= randOctave * jtuneArray[random(9)];
    seqNotes[2]= randOctave * jtuneArray[random(9)];
    seqNotes[3]= randOctave * jtuneArray[random(9)];
    seqNotes[4]= randOctave * jtuneArray[random(9)];
    seqNotes[5]= randOctave * jtuneArray[random(9)];
    seqNotes[6]= randOctave * jtuneArray[random(9)];
    seqNotes[7]= randOctave * jtuneArray[random(9)];
    Serial.println("3:MODE 3:");                      //Send a serial msg "3" to Processing, where it will open the gate of the Synth initalized in Supercollider
    lastPress = millis();                             //Update millis()
  }
}

//CHECK THE PHOTOCELLS AND SEND A MSG FOR SOUND:
if (currentMillis - lastPass > msgSpeed) {            //If it's time to send a message:
  //CHORD MODE:
  if (buttonMode == 1) {                              //And the buttonMode is exactly 1:
    if (photo1 > low1-150) {                          //If the first photocell goes within 150 of the "low" value
        randPartial = randOctave * jtuneArray[random(9)];//Generate a random partial off of the random pitch chosen when green button was pressed
        Serial.print(randPartial); Serial.print(":");   //Print the note generated to serial port
        Serial.print(amplitude);                        //And print the amplitude
        Serial.println(":1");                           //Seperate it and print the buttonMode so Processing know what to do with the note
        digitalWrite(ledPin1, HIGH);                    //Turn on the LED
        digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);
        lastPass = millis();                            //Update millis  
      } if (photo2>low2-150) {
        randPartial = randOctave * jtuneArray[random(9)];//Generate a random partial off of the random pitch chosen when green button was pressed
        Serial.print(randPartial); Serial.print(":");   //Print the note generated to serial port
        Serial.print(amplitude);
        Serial.println(":1");                           //Seperate it and print the buttonMode so Processing know what to do with the note
        digitalWrite(ledPin2, HIGH);                    //Turn on the LED
        digitalWrite(ledPin1, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                           
        lastPass = millis();                            
      } if (photo3>low3-150) {
       randPartial = randOctave * jtuneArray[random(9)];//Generate a random partial off of the random pitch chosen when green button was pressed
        Serial.print(randPartial); Serial.print(":");   //Print the note generated to serial port
        Serial.print(amplitude);
        Serial.println(":1");                           //Seperate it and print the buttonMode so Processing know what to do with the note 
        digitalWrite(ledPin3, HIGH);                    //Turn on the LED
        digitalWrite(ledPin2, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                          
        lastPass = millis();
      } if (photo4>low4-150) {
        randPartial = randOctave * jtuneArray[random(9)];//Generate a random partial off of the random pitch chosen when green button was pressed
        Serial.print(randPartial); Serial.print(":");   //Print the note generated to serial port
        Serial.print(amplitude);
        Serial.println(":1");                           //Seperate it and print the buttonMode so Processing know what to do with the note
        digitalWrite(ledPin4, HIGH);                    //Turn on the LED
        digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                           
        lastPass = millis();
      } if (photo5>low5-150) {
        randPartial = randOctave * jtuneArray[random(9)];//Generate a random partial off of the random pitch chosen when green button was pressed
        Serial.print(randPartial); Serial.print(":");   //Print the note generated to serial port
        Serial.print(amplitude);
        Serial.println(":1");   
        digitalWrite(ledPin5, HIGH);                    //Turn on the LED
        digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                        
        lastPass = millis();
      } if (photo6>low6-150) {
        randPartial = randOctave * jtuneArray[random(9)];//Generate a random partial off of the random pitch chosen when green button was pressed
        Serial.print(randPartial); Serial.print(":");   //Print the note generated to serial port
        Serial.print(amplitude);
        Serial.println(":1"); 
        digitalWrite(ledPin6, HIGH);                    //Turn on the LED
        digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin7, LOW);                          
        lastPass = millis();
      } if (photo7>low7-150) {
        randPartial = randOctave * jtuneArray[random(9)];//Generate a random partial off of the random pitch chosen when green button was pressed
        Serial.print(randPartial); Serial.print(":");   //Print the note generated to serial port
        Serial.print(amplitude);
        Serial.println(":1");   
        digitalWrite(ledPin7, HIGH);                    //Turn on the LED
        digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin1, LOW);                        
        lastPass = millis();
      } if (photo8>low8-150) {
        randPartial = randOctave * jtuneArray[random(9)];//Generate a random partial off of the random pitch chosen when green button was pressed
        Serial.print(randPartial); Serial.print(":");   //Print the note generated to serial port
        Serial.print(amplitude);
        Serial.println(":1");  
        digitalWrite(ledPin1, LOW);                    //Turn on the LED
        digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                         
        lastPass = millis();
      } 
  }
  //DETUNE MODE:
  if (buttonMode == 2){                               //If buttonMode is mode 2:
    if (photo1 > low1-150) {                          //If the first photocell goes within 150 of the "low" value
      photo1 = map(photo1, start1, low1, 0, 250);     //Map the value to between 0 when no car passing, and 2.5 when passing
      Serial.print(photo1);                           //Send it to the serial port to send to supercollider
      Serial.print(":"); Serial.print(randPitch);
      Serial.println(":2");                           //Seperate and send a 2 to tell supercollider we're in mode 2
      digitalWrite(ledPin1, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);
      lastPass = millis();                            //Update millis  
    } if (photo2>low2-150) {
      photo2 = map(photo2, start2, low2, 0, 250); 
      Serial.print(photo2);           
      Serial.print(":"); Serial.print(randPitch);
      Serial.println(":2");
      digitalWrite(ledPin2, HIGH);                    //Turn on the LED
      digitalWrite(ledPin1, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                           
      lastPass = millis();                            
    } if (photo3>low3-150) {
      photo3 = map(photo3, start3, low3, 0, 250);  
      Serial.print(photo3);          
      Serial.print(":"); Serial.print(randPitch);
      Serial.println(":2");
      digitalWrite(ledPin3, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                          
      lastPass = millis();
    } if (photo4>low4-150) {
      photo4 = map(photo4, start4, low4, 0, 250);  
      Serial.print(photo4);        
      Serial.print(":"); Serial.print(randPitch);
      Serial.println(":2");
      digitalWrite(ledPin4, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                           
      lastPass = millis();
    } if (photo5>low5-150) {
      photo5 = map(photo5, start5, low5, 0, 250); 
      Serial.print(photo5);    
      Serial.print(":"); Serial.print(randPitch);
      Serial.println(":2");
      digitalWrite(ledPin5, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                        
      lastPass = millis();
    } if (photo6>low6-150) {
      photo6 = map(photo6, start6, low6, 0, 250);
      Serial.print(photo6);            
      Serial.print(":"); Serial.print(randPitch);
      Serial.println(":2");
      digitalWrite(ledPin6, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin7, LOW);                          
      lastPass = millis();
    } if (photo7>low7-150) {
      photo7 = map(photo7, start7, low7, 0, 250);
      Serial.print(photo7);            
      Serial.print(":"); Serial.print(randPitch);
      Serial.println(":2");
      digitalWrite(ledPin7, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin1, LOW);                        
      lastPass = millis();
    } if (photo8>low8-150) {
      photo8 = map(photo8, start8, low8, 0, 250);
      Serial.print(photo8);            
      Serial.print(":"); Serial.print(randPitch);
      Serial.println(":2");
      digitalWrite(ledPin1, LOW);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                         
      lastPass = millis();
    } 
  }
  //SEQUENCER MODE:
  if (buttonMode == 3) {                              //And the buttonMode is exactly 3 (yellow?):
    if (photo1 > low1-150) {                          //If the first photocell goes within 150 of the "low" value
      Serial.print(seqNotes[0]); Serial.print(":");    //Send the first note to the serial port to send to supercollider
      Serial.print(amplitude);
      Serial.println(":3");                           //Seperate and send a 2 to tell supercollider we're in mode 2
      digitalWrite(ledPin1, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);
      lastPass = millis();                            //Update millis  
    } if (photo2>low2-150) {
      Serial.print(seqNotes[1]); Serial.print(":");
      Serial.print(amplitude);
      Serial.println(":3");
      digitalWrite(ledPin2, HIGH);                    //Turn on the LED
      digitalWrite(ledPin1, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                           
      lastPass = millis();                            
    } if (photo3>low3-150) {
      Serial.print(seqNotes[2]); Serial.print(":");
      Serial.print(amplitude);
      Serial.println(":3"); 
      digitalWrite(ledPin3, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                          
      lastPass = millis();
    } if (photo4>low4-150) {
      Serial.print(seqNotes[3]); Serial.print(":");
      Serial.print(amplitude);
      Serial.println(":3");
      digitalWrite(ledPin4, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                           
      lastPass = millis();
    } if (photo5>low5-150) {
      Serial.print(seqNotes[4]); Serial.print(":");
      Serial.print(amplitude);
      Serial.println(":3");   
      digitalWrite(ledPin5, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                        
      lastPass = millis();
    } if (photo6>low6-150) {
      Serial.print(seqNotes[5]); Serial.print(":");
      Serial.print(amplitude);
      Serial.println(":3"); 
      digitalWrite(ledPin6, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin1, LOW);digitalWrite(ledPin7, LOW);                          
      lastPass = millis();
    } if (photo7>low7-150) {
      Serial.print(seqNotes[6]); Serial.print(":");
      Serial.print(amplitude);
      Serial.println(":3");   
      digitalWrite(ledPin7, HIGH);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin1, LOW);                        
      lastPass = millis();
    } if (photo8>low8-150) {
      Serial.print(seqNotes[7]); Serial.print(":");
      Serial.print(amplitude);
      Serial.println(":3");  
      digitalWrite(ledPin1, LOW);                    //Turn on the LED
      digitalWrite(ledPin2, LOW);digitalWrite(ledPin3, LOW);digitalWrite(ledPin4, LOW);digitalWrite(ledPin5, LOW);digitalWrite(ledPin6, LOW);digitalWrite(ledPin7, LOW);                         
      lastPass = millis();
    }
  }
}
}                                                     //Final bracket for void loop()
