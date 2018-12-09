#include <Bounce.h>
 
// pin definitions
const int digital_pin[] = {  33, 34, 2,    3, 4, 5,    6, 7, 8,    32, 10, 11,    12, 19,21,    22,23,24,    25,26,27,    28,29,30};
const int analog_pin[] = { A1, A2, A3, A4, A6, A12 };
 
// variables for the states of the controls
boolean digital_stored_state[24];
int analog_stored_state[6];
 
// amount of change that constitutes sending a midi message
const int analog_threshold = 6;
const int analog_scale = 8;

 
// Debounce
long debounceDelay = 10;

Bounce digital_debouncer[] = {
  Bounce(digital_pin[0], debounceDelay),
  Bounce(digital_pin[1], debounceDelay),
  Bounce(digital_pin[2], debounceDelay),
  Bounce(digital_pin[3], debounceDelay),
  Bounce(digital_pin[4], debounceDelay),
  Bounce(digital_pin[5], debounceDelay),
  Bounce(digital_pin[6], debounceDelay),
  Bounce(digital_pin[7], debounceDelay),
  Bounce(digital_pin[8], debounceDelay),
  Bounce(digital_pin[9], debounceDelay),
  Bounce(digital_pin[10], debounceDelay),
  Bounce(digital_pin[11], debounceDelay),
  Bounce(digital_pin[12], debounceDelay),
  Bounce(digital_pin[13], debounceDelay),
  Bounce(digital_pin[14], debounceDelay),
  Bounce(digital_pin[15], debounceDelay),
  Bounce(digital_pin[16], debounceDelay),
  Bounce(digital_pin[17], debounceDelay),
  Bounce(digital_pin[18], debounceDelay),
  Bounce(digital_pin[19], debounceDelay),
  Bounce(digital_pin[20], debounceDelay),
  Bounce(digital_pin[21], debounceDelay),
  Bounce(digital_pin[22], debounceDelay),
  Bounce(digital_pin[23], debounceDelay),
};
 
// MIDI settings
int midi_ch = 3;
int midi_vel = 127;
 
const int digital_note[] = { 36,37,38,   39,40,41,   42,43,44,   45,46,47,   48,49,50,   51,52,53,   54,55,56,   57,58,59 };
const int analog_control[] = { 0, 1 ,2 ,3 ,4 ,5};


void setup() {
  //Serial.begin(57600);
  Serial.begin(38400);
 
  // set the pin modes && zero saved states
  int b = 0;
  
  // digital pins
  for (b = 0; b < 24; b++) 
  {
    pinMode(digital_pin[b], INPUT_PULLUP);
    digital_stored_state[b] = false;
    //Serial.print("setup: pin ");
    //Serial.println(b);
  }
  
  // analog pins
  for (b = 0; b < 6; b++) 
  {
    analog_stored_state[b] = 0; 
  }
 
  //Serial.println("----------------");
  //Serial.println("MIDI DJ Controller Test - setup");
  //Serial.println("----------------");
}
 
 
void loop() {
  fcnProcessButtons();
}
 
//Function to process the buttons
void fcnProcessButtons() {
  int b = 0;
  
  // digital pins
  for (b = 0; b < 24;b++)
  {
    digital_debouncer[b].update();
    boolean state = digital_debouncer[b].read();
    if (state != digital_stored_state[b]) {
      if (state == false) {
        usbMIDI.sendNoteOn(digital_note[b], midi_vel, midi_ch);
        //Serial.print("MIDI note on: ");
        //Serial.println(digital_note[b]);
      } else {
        usbMIDI.sendNoteOff(digital_note[b], midi_vel, midi_ch);
        //Serial.print("MIDI note off: ");
        //Serial.println(digital_note[b]);
      }
      digital_stored_state[b] = !digital_stored_state[b];
    }
  }
  
  // analog pins
  for (b = 0; b < 6; b++)
  {
  int analog_state = analogRead(analog_pin[b]);
  //if (analog_state - analog_stored_state[b] >= analog_scale || analog_stored_state[b] - analog_state >= analog_scale) {
  //if (abs(analog_state - analog_stored_state[b] >= analog_threshold)) { // 200130526: correct the typo & use form pointed out by grivvr in the comments
  if (abs(analog_state - analog_stored_state[b]) >= analog_threshold) { // 200130622: correct new typo in previous update pointed out by luke birch in the comments
      int scaled_value = 127 - (analog_state / analog_scale);
      usbMIDI.sendControlChange(analog_control[b], scaled_value, midi_ch);
      /*
      Serial.print("analog value ");
      Serial.print(b);
      Serial.print(": ");
      Serial.print(analog_state);
      Serial.print(" scaled: ");
      Serial.println(scaled_value);*/
      analog_stored_state[b] = analog_state;
    }
  }
}
