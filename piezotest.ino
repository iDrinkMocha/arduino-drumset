/*
MIDI On/Off Messages
By Amanda Ghassaei
July 2012
https://www.instructables.com/id/Send-and-Receive-MIDI-with-Arduino/

Modified by Yang Chun Wei for drumset use
March 2019

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

*/
 
 int velocity = 100;//velocity of MIDI notes, must be between 0 and 127
 //higher velocity usually makes MIDI instruments louder
 
 int noteON = 144;//144 = 10010000 in binary, note on command
 int noteOFF = 128;//128 = 10000000 in binary, note off command

 int analogPin[] = {0, 1, 2, 3, 4, 5};

 int readVal[6];
 int velo[6];
 int dupe[6];
 const int thres = 100;
 const int maximum = 300;
 const int bassPin = 9;
 const int hatPin = 8;
 int bassState, hatState = 0;
 boolean bassPressed, hatPressed = false;

void setup() {
  //  Set MIDI baud rate:
  Serial.begin(9600);
  pinMode(bassPin, INPUT);
  pinMode(hatPin, INPUT);
}

void loop() {
  readVal[0] = analogRead(analogPin[0]);
  readVal[1] = analogRead(analogPin[1]);
 // readVal[2] = analogRead(analogPin[2]);
 // readVal[3] = analogRead(analogPin[3]);
  readVal[4] = analogRead(analogPin[4]);
 // readVal[5] = analogRead(analogPin[5]);
 bassState = digitalRead(bassPin);
  //hatState = digitalRead(hatPin);

  //hihat
   if(readVal[0]> thres){
      if(readVal[0] > maximum){
        readVal[0] = maximum;
      }
      velo[0] = map(readVal[0], thres, maximum, 40, 127);
      if(hatPressed == true){
        MIDImessage(noteON, 42, velo[0]);
        delay(10);
        dupe[0] = velo[0];
        MIDImessage(noteOFF, 42, dupe[0]);
      } else{
        //open is 46 but fk i cant get it to work
        MIDImessage(noteON, 42, velo[0]);
        delay(10);
        dupe[0] = velo[0];
        MIDImessage(noteOFF, 42, dupe[0]);
      }
      
  }

  //snare
  if(readVal[1]> thres){
      if(readVal[1] > maximum){
        readVal[1] = maximum;
      }
      velo[1] = map(readVal[1], thres, maximum, 40, 127);
      MIDImessage(noteON, 38, velo[1]);
      delay(10);
      dupe[1] = velo[1];
      MIDImessage(noteOFF, 38, dupe[1]);
  }

  //hi-tom
  if(readVal[2]> thres){
      if(readVal[2] > maximum){
        readVal[2] = maximum;
      }
      velo[2] = map(readVal[2], thres, maximum, 40, 127);
      MIDImessage(noteON, 50, velo[2]);
      delay(10);
      dupe[2] = velo[2];
      MIDImessage(noteOFF, 50, dupe[2]);
  }

  //low-tom
  if(readVal[3]> thres){
      if(readVal[3] > maximum){
        readVal[3] = maximum;
      }
      velo[3] = map(readVal[3], thres, maximum, 40, 127);
      MIDImessage(noteON, 45, velo[3]);
      delay(10);
      dupe[3] = velo[3];
      MIDImessage(noteOFF, 45, dupe[3]);
      //additional delay to prevent multistrike
      delay(70);
  }

  //crash cymbal 49/55(splash)/57
  if(readVal[4]> thres){
      if(readVal[4] > maximum){
        readVal[4] = maximum;
      }
      velo[4] = map(readVal[4], thres, maximum, 40, 127);
      MIDImessage(noteON, 49, velo[4]);
      delay(10);
      dupe[4] = velo[4];
      MIDImessage(noteOFF, 49, dupe[4]);
      //additional delay to prevent multistrike
      delay(70);
  }

  //customisable pad ridecym 51, floor 43
  if(readVal[5]> thres){
      if(readVal[5] > maximum){
        readVal[5] = maximum;
      }
      velo[5] = map(readVal[5], thres, maximum, 40, 127);
      MIDImessage(noteON, 43, velo[5]);
      delay(10);
      dupe[5] = velo[5];
      MIDImessage(noteOFF, 43, dupe[5]);
      //additional delay to prevent multistrike
      delay(70);
  }
  
  /*All button logic are placed below */

  //bass drum logic 35/36 choose sound
  if(bassState == HIGH){
    if(bassPressed == false){
     MIDImessage(noteON, 36, 127);
     delay(10);
     MIDImessage(noteOFF, 36, 127);
     delay(200);
     bassPressed = true;  
    }
  } else{
    bassPressed = false;  
  }
 

  //hi-hat close and open logic
  if(hatState == HIGH){
    MIDImessage(noteON, 42, 110);
     delay(10);
     MIDImessage(noteOFF, 42, 110);
     delay(200);
    if(hatPressed == false){
     
     hatPressed = true;  
    }
  } else{
    hatPressed = false;  
  }
  
}

//send MIDI message
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}
