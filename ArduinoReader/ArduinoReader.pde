//Pete's Beats Revolutionized, Cooper Union 2011

#include "pitches.h"

int notesplayed[]={0,0,0,0,0,0,0,0};
boolean incontact;
char serialprinter[]={'A','B','C','D','E','F','G','H'};

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  incontact=false;
  AttemptContact();
}

void loop() {
  if (incontact){ //which means that the Arduino is connected to the game
        boolean pressedany=false;
        //for pin 2
        int sensorValue=0;
        for (int i=0;i<8;i++){
            sensorValue=digitalRead(i+2);
            if (sensorValue==0){ 
              pressedany=true;
              Serial.print(serialprinter[i]);
            }
        }
        /*
        if (!pressedany) {
          Serial.print(" "); 
        }*/
  }
  
  else{ //This allows for a standalone system
        //for pin 2
        int sensorValue = digitalRead(2);
        if (sensorValue==0) {
          notesplayed[0]=NOTE_C3;
        }
        
        //for pin 3
        sensorValue = digitalRead(3);
        if (sensorValue==0) {
          notesplayed[1]=NOTE_D3;
        }
        
        //for pin 4
        sensorValue = digitalRead(4);
        if (sensorValue==0) {
          notesplayed[2]=NOTE_E3;
        }
        
        //for pin 5
        sensorValue = digitalRead(5);
        if (sensorValue==0) {
          notesplayed[3]=NOTE_F3;
        }
        
        //for pin 6
        sensorValue = digitalRead(6);
        if (sensorValue==0) {
          notesplayed[4]=NOTE_G3;
        }
        
        //for pin 7
        sensorValue = digitalRead(7);
        if (sensorValue==0) {
          notesplayed[5]=NOTE_A3;
        }
        
        //for pin 8
        sensorValue = digitalRead(8);
        if (sensorValue==0) {
          notesplayed[6]=NOTE_B3;
        }
        
        //for pin 9
        sensorValue = digitalRead(9);
        if (sensorValue==0) {
          notesplayed[7]=NOTE_C4;
        }
        
        //this section now plays notes on speakers attached to pin 12
        //it can only generate a tone on one pin at a time, 
        //so connect both left and right channels to pin 12
        //send the ground pin of the speakers to ground on the arduino
          for (int i=0;i<8;i++){
            if (notesplayed[i]!=0){
              tone(12,notesplayed[i],1000/4);
              delay(1000/4);
              notesplayed[i]=0;
              break;
            }
          }
          noTone(12);
          
        //i dont understand. u set an automatic order of a scale. 
        //you're not filling up the notesplayed[] based on order of user-playing, you're filling it up automatically. 
        //the notesplayed[] needs to have a variable which increases with every pressed note.
                //I don't understand what this comment means...
  }
}

void AttemptContact(){
	int tries=0;
	while (Serial.available() <=0 && tries < 5){
		Serial.print('I');
		delay(500);
		tries++;
	}
	if (Serial.available()>0){
		char byte=Serial.read();
		if (byte=='I'){
			Serial.flush();
			incontact=true;
		}
	}
}



