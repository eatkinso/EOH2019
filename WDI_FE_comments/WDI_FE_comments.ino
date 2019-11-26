#include <CapacitiveSensor.h>
#include <MIDI.h>

//include relevant libraries 

MIDI_CREATE_DEFAULT_INSTANCE(); //begin MIDI

CapacitiveSensor s2_4 = CapacitiveSensor(2, 4); //declare the sensor (pin2, pin4) is the pitch
CapacitiveSensor s2_6 = CapacitiveSensor(2, 6); //declare the sensors (pin2, pin6) is volume
int arbitraryThreshold = 100; //this determines how much change in the sensor value will result in a note change
 

long currentNoteInput = 60; 
long lastNoteInput = 0;
int noteStep = 2; // this determines how many half-steps each note-change occurrence is

long currentVolInput = 60;
long lastVolInput = 0;
int volStep = 10; // this determines how large each volume-change interval is

int Note = 60; //arbitrary start note
int highNote = 100; // highest possible note
int lowNote = 60; // lowest possible note

int Vol = 90; //arbitrary start volume
int highVol = 127; //highest possible volume
int lowVol = 50; //lowest possible volume

int lastresetmillis = 0;
int currentmillis = 0;

void setup() {
MIDI.begin(MIDI_CHANNEL_OMNI);
Serial.begin(115200); // baud rate must be 115200 for MIDI

}

void loop() {
  delay(1); //not sure if this is necessary or not tbh, the processing speed already limits it enough
  
  long value_s2_4 = s2_4.capacitiveSensor(100); //read the sensors
  long value_s2_6 = s2_6.capacitiveSensor(100);
  currentNoteInput = value_s2_4; 
  currentVolInput = value_s2_6;





  if (abs(currentNoteInput - lastNoteInput) >= arbitraryThreshold) {
    if (currentNoteInput > lastNoteInput) {
      if (Note <= highNote){
        Note += noteStep;
        if (Note >= highNote){
          Note = highNote;
          }
        } 
      } 
     else if (currentNoteInput < lastNoteInput) {
      if (Note >= lowNote) {
        Note -= noteStep; 
        if (Note <= noteStep){
          Note <= lowNote;
          }
        }
        }
    }
    lastNoteInput = currentNoteInput;
/* if the difference between the current sensor input and the last sensor 
      input is greater than the threshold difference, it checks if the change has been an increase or decrease, 
      then increases the variable "note" by the amount "noteStep" if the change has been an increase, and decreases
      the variable "note" by the amount "noteStep" if the change has been a decrease. The following function 
      that looks similar does the exact same thing, but for increasing/decreasing the volume.*/

   if ((currentmillis - lastresetmillis) >= 30000) {
    Note = 60; 
    lastresetmillis = currentmillis;
    }
    currentmillis = millis(); 
/*we had problems with the note getting higher and never getting lower; this is a quick-and-dirty
 * method of switching it back to the minimum note (60) every 30 seconds so it wouldn't just get 
 * stuck at a high value. probably a better way to do this, it was just a last-minute fix.*/


if (abs(currentVolInput - lastVolInput) >= arbitraryThreshold) {
    if (currentVolInput > lastVolInput) {
      if (Vol <= highVol){
        Vol += volStep;
        if (Vol >= highVol){
          Vol = highVol;
          }
        } 
      }
     else if (currentVolInput < lastVolInput) {
      if (Vol >= lowVol) {
        Vol -= volStep; 
        if (Vol <= lowVol){
          Vol = lowVol;
          }
        }
        }
    }
    lastVolInput = currentVolInput;
    MIDI.sendNoteOn(Note, Vol, 1); //sends a MIDI message through USB
}



/*Elizabeth Atkinson and Varun Sharma 
 * Materials Science and Engineering Dept University of Illinois at Urbana-Champaign 
 * Engineering Open House 2019
 * We hope you enjoy (and learn from!) our work! */
