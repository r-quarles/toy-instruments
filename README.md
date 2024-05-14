# toy-instruments
*A MT Capstone project by Rae Quarles*

## Introduction
These files work together alongside hardware to sonify a toy car racetrack. Sensors can be placed anywhere on a track to create a trigger-spot, and can be customized to any room with a seperate callibration code. By capitializing on nostalgia and intrigue, this setup entices guests to interact and play the instrument together. 

The system consists of four parts, three input buttons and a center "brain" where the sensors communicate with the microprocessor. For this project I chose to use an Arduino Nano for it's size and accessibility, and was able to perfectly max out all 8 analog and 11 digital pins. The arduino takes in each sensor's data, and sends a unique serial message to Processing each time a car passes over depending on the current "mode" of the system. Processing takes this serial message, splits it, and sends the data as OSC to Supercollider where it triggers a note with the specified parameters. 

### Modes: <br>
> Red - "Detune": <br>
>> This button triggers a drone when pressed at a random note in the third octave. Each time a car passes a sensor it triggers a drone that is slightly detuned from the original pitch, creating a pulsating effect that mimics a LFO. <br>

> Yellow - "Sequence": <br>
>> This button generates a new series of 8 notes in the third octave when pressed, and plays them in order as the car passes over the sensors. The car can be driven back-wards to trigger notes in reverse, or stay stationary to trigger one repeatedly. <br>

> Green - "Chord": <br>
>> This button triggers a random octave of "C" when pressed, and plays a random partial overtop each time the sensors are passed. This creates different chords as the car makes it way around the track. <br>
      
## Process

### Software

This project required the use of three different platforms; Arduino IDE, Processing, and Supercollider. This meant much trial and error while testing, as three platforms meant three places for mistakes. During the course of the semester-long project I learned how to effectively use millis(), as well as how to more fluidly use Java's boolean features when comparing variables. 

When starting, I chose to stay small and focus on narrowing in on the photocell trigger itself. This required quite a bit of callibration with thresholds, which ultimately was done with a seperate patch code I wrote. This patch code would give me the running real value of the analog pins, so I could center the car over them and take the "low light" readings. This, combined with a short line in setup() that would store the ambient light value, allowed me to set up the system in any scenerio. From there, I was able to connect to Supercollider through Processing to create my first mode: CHORD. This mode takes an array of pitches of "C", and randomly picks an index. This note is then played with a 30s duration, while it's multiplied by a just-tuning ratio to create different intervals. These layered over each other create short instances of chords. From here I was able to explore the functionality of the racetrack itself as an instrument through user testing to create the two remaining modes. 

My synth in Supercollider uses a drone and synth I created last year. The drone is the product of a "blip" generator multiplied by two LFNoise generators, which is then splayed across 16 channels and summed. The result is a haunting drone that has a lot of reverb and texture to it, and is easily able to be maniplulated in and out of tune. The key is somewhat more simple, with a PMOscillator that has had harmonics added to it. This key is able to be manipulated with a user-inputted modulation frequency, which in the case of CHORD is able to be randomly chosen with each note. 

### Hardware

The hardware of this project is no doubt the focus. It is the tacticle items that the users come in direct contact with. To make these as useful as possible, I made the buttons modular to the "brain" so they can be attached with a patch cable. This lets the buttons be placed anywhere on the display table, so they can be spread out for many people. This meant they needed to supply their own power, which must be between 3 and 5V to be compatible with the NANO. I used two AA batteries to supply the 3V needed to the digital pins that the button-in jacks connect to. These were also able to power the lights in the arcade buttons I sourced, adding to the design of the system. 

The actual soldering if the circuit gave me much grief, with broken tracks disconnecting some of it. This was able to be remedied by some solder-bridges between the break, saving my circuitboard. Because I had maxed out the pins on the board, I had a lot of components that needed to fit in a very small space on my circuit board. I did have a max size for the board, as the I needed the box to stay reasonabley sized for transport and needed to minimize the amount of radio frequency that could interact with my microprocessor. Through a lot of careful planning, I was able to fit everything I needed onto one protoboard. This I feel is one of the greatest accomplishments of this project, as my planning and design of my hardware has grown through my years of practice at Columbia. Though I did run into some bumps, the resulting work is clean and has few overlaps showing good signal-flow. 

The resulting enclosures would be my second favorite feature of the project. I spent much time perfecting the pattern on them and sealing it, as the final aesthetic of my project felt just as important as it's sound design. The colorful design feels playful, and intrigued many passerby as I was working on it. The buttons and "brain" were clearly defined for users as what they can press and should not, as the final "brain" enclosure lacked much of the interface the buttons had. This combined with it's closed-box design meant it could be plugged into a computer and left alone without worry of cords becoming loose or reset switches being pressed. I ran into a few issues when mounting the USB-C recepticle, as my data-cable was shorter than the enclosure walls. Thankfully, I was able to find an adapter that bridged the gap the morning of Manifest (whew). 

## Conclusions

This project has been the largest one I have ever worked on, and has been increadibly rewarding. The system I have now is able to be expanded upon in a million ways, by adding new modes, sounds, and interactions between the lights and sensors. If I were to continue this project, I would want to add more button-modes and add a visual output from Processing. This Processing screen could be projected, increasing the whimsy around the project. I have learned so much about the process of design through this project, and how to work with my own creative-style to make things that I am proud to showcase. Though it was not the most straight-forward journey, I eventually was able to make a very compelling project that many were able to enjoy. 
