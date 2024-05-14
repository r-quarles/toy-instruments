//"Toy Instrument"
//For 2024 MT Capstone
//Rae Quarles - 5/9/24
//V 2.4

//2.4 Update:
  //Tidying everything up

//Using code from: https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all
//Listens to Arduino for Serial Messages, then sends an OSC message to Supercollider
import oscP5.*;
import netP5.*;                //OSC libraries
import processing.serial.*;    //Import the Serial Library

OscP5 oscP5;
NetAddress myRemoteLocation;
Serial myPort;                 // Create object from Serial class
String msg;                    // Data received from the serial port
String[] vals;                 // Where msg will be stored once split
int lastTime = 0;              //For millis
int speed = 1000;              //Speed for sending messages (1s)

void setup()
{ // "Uses the first port in this list (number 0). Change this to select the port
  // corresponding to your Arduino board. The last parameter (e.g. 9600) is the
  // speed of the communication. It has to correspond to the value passed to
  // Serial.begin() in your Arduino sketch." Run, then change the below value
  myPort = new Serial(this, "COM9", 9600);  //Then create a new serial listen instance
  myPort.bufferUntil('\n');
  // Start oscP5, listening for incoming messages at port 12000
  oscP5 = new OscP5(this, 7002);
  myRemoteLocation = new NetAddress("127.0.0.1", 57121);
  // Assuming the other computer is running at the same IP and its OSC server is on port 12000
  //myRemoteLocation = new NetAddress("192.168.1.XXX",12000); // Replace with your friend's IP address if necessary
}

void draw() {}                                //We don't need this!

void serialEvent (Serial myPort) {     
  msg = myPort.readStringUntil('\n'); 
  msg = trim(msg);
  vals = split(msg, ":");                    //SPLIT THAT SERIAL STRING!

//CHORD MODE!!!!  
  //If button is pressed:
  if (vals[0].equals("1")) {                 //If the 1st msg is 1, meaning the green button was just pressed
  OscMessage greenOn = new OscMessage("/greenOn");  //Make an OSC message named "photoVals" to send to Supercollider
  greenOn.add(Float.parseFloat(vals[1]));    //Add the 2nd serial value (the fundamental pitch)  
  greenOn.add(Float.parseFloat(vals[2])/100.0);    //Add the 3rd serial value (the amp)
  oscP5.send(greenOn, myRemoteLocation);     //And send the message  
  println("CHORD MODE");                     //Tell us the mode changed
}
  //And then if a car passes by:
  if (vals[2].equals("1")) {                 //If the 2nd msg is 1, meaning were recieving partials for "chord" mode
  OscMessage chord = new OscMessage("/chord"); //Make an OSC message named "chord" to send to Supercollider
  chord.add(Float.parseFloat(vals[0]));      //Add the 1st serial value to the OSC message (the pitch of the partial)
  chord.add(Float.parseFloat(vals[1])/100.0);      //Add the 2nd serial value (the amplitude)
  oscP5.send(chord, myRemoteLocation);       //And send the message
  print("MODE 1: \t");                       //Tell us there was a note
  println(vals[0]);                          //And the freq
  }
  
//DETUNE MODE!!!!  
  //If button is pressed:
  if (vals[0].equals("2")) {                 //If the 1st msg is 2, meaning we're in mode 2
  OscMessage redOn = new OscMessage("/redOn");//Make an OSC message named "redOn" to send to Supercollider
  redOn.add(Float.parseFloat(vals[1]));      //Add the 2nd serial value (the pitch)
  oscP5.send(redOn, myRemoteLocation);       //And send the message  
  println("DETUNE MODE");                    //Tell us the mode changed
  }
  //And then if a car passes by:
  if (vals[2].equals("2")) {                 //If the 3rd msg is 1, meaning were recieving detune amount for "detune" mode
  OscMessage detune = new OscMessage("/detune"); //Make an OSC message named "photoVals" to send to Supercollider
  detune.add(Float.parseFloat(vals[0])/100); //Add the 1st serial value to the OSC message (the pitch of the partial)
  detune.add(Float.parseFloat(vals[1]));     //Add the 2nd serial value (the pitch)
  oscP5.send(detune, myRemoteLocation);      //And send the message
  print("MODE 2: \t");                       //Tell us there was a note
  println(Float.parseFloat(vals[0])/100);    //And add the detune amount
  }
  
//SEQUENCER MODE!!!! 
  //If button is pressed:
  if (vals[0].equals("3")) {                 //If the 1st msg is 3, meaning the yellow button was just pressed
  OscMessage yellowOn = new OscMessage("/yellowOn"); //Make an OSC message named "yellowOn" to send to Supercollider
  oscP5.send(yellowOn, myRemoteLocation);    //And send the message
  println("SEQUENCER MODE");                 //Tell us the mode changed
  }
  //And then if a car passes by:
  if (vals[2].equals("3")) {                 //If the 3rd msg is 3, meaning were recieving notes for "sequencer" mode
  OscMessage seq = new OscMessage("/seq");   //Make an OSC message named "seq" to send to Supercollider
  seq.add(Float.parseFloat(vals[0]));        //Add the 1st serial value to the OSC message (the pitch of the partial)
  seq.add(Float.parseFloat(vals[1]));        //Add the 2nd serial value (the amplitude)
  oscP5.send(seq, myRemoteLocation);         //And send the message
  print("MODE 3: \t");                       //Tell us there was a note
  println(vals[0]);                          //And the freq
  }
// println(msg);                           //Print the messages
}
