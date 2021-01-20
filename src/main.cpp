#include <Arduino.h>

/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>
#include <HID-Project.h>
#include <EEPROM.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
#define BUTTON_PIN 7
#define WINDOWS 0
#define MACOS 1


Encoder myEnc(6, 5);
//   avoid using pins with LEDs attached
int myOS;


int oldState = HIGH;
long oldPosition;
bool fineAdj = false;
long holdTimer;


void osSetup() {
  while (digitalRead(BUTTON_PIN) == LOW);
  oldPosition = myEnc.read();
  int newPosition = oldPosition;
  delay(10);
  while (digitalRead(BUTTON_PIN) == HIGH) {
    newPosition = myEnc.read();
  }
  Serial.print(newPosition);
  Serial.print(" : ");
  Serial.println(oldPosition);

  if (newPosition > oldPosition) myOS = WINDOWS;
  else {
    myOS = MACOS;
    while (digitalRead(BUTTON_PIN) == LOW);
    while (digitalRead(BUTTON_PIN) == HIGH);
      Keyboard.press(KEY_Z);
    while (digitalRead(BUTTON_PIN) == LOW);
      Keyboard.releaseAll();
    while (digitalRead(BUTTON_PIN) == HIGH);
      Keyboard.press(KEY_SLASH);
    while (digitalRead(BUTTON_PIN) == LOW);
      Keyboard.releaseAll();
  }

  EEPROM.write(0, myOS);
  oldPosition = newPosition;
}

void handleButton() {
  int newState = digitalRead(BUTTON_PIN);
  // Button logic
  if (newState == LOW) {
    if (newState != oldState) {
    Serial.println("Pressed");
      if (myOS == MACOS) {
        Serial.println("CHANGE");
        fineAdj ? fineAdj = false : fineAdj = true;
      }
      else Consumer.write(HID_CONSUMER_MUTE);      
    }
    
    // If button held for '5' seconds
    if (millis() - holdTimer > 3000) {
      Serial.println("Held!");

      if (myOS == WINDOWS) Consumer.write(HID_CONSUMER_MUTE);

      fineAdj = !fineAdj;
      osSetup();

      holdTimer = millis();
    }
  } else holdTimer = millis();
  oldState = newState;
}

void handleEncoder() {
  long newPosition = myEnc.read();
  // Rotate logic else
  if (myOS == MACOS) {
    if (newPosition - oldPosition == 4) {
      if (fineAdj) {
        Serial.print("FINE ");
        Keyboard.press(HID_KEYBOARD_LEFT_SHIFT);
        Keyboard.press(HID_KEYBOARD_LEFT_ALT);
        Consumer.write(HID_CONSUMER_VOLUME_INCREMENT);
        Keyboard.releaseAll();
      }
      else Consumer.write(HID_CONSUMER_VOLUME_INCREMENT); 

      Serial.println("UP");
      oldPosition = newPosition;
    } else if (oldPosition - newPosition == 4) {
      if (fineAdj) {
        Serial.print("FINE ");
        Keyboard.press(HID_KEYBOARD_LEFT_SHIFT);
        Keyboard.press(HID_KEYBOARD_LEFT_ALT);
        Consumer.write(HID_CONSUMER_VOLUME_DECREMENT);
        Keyboard.releaseAll();
      }
      else Consumer.write(HID_CONSUMER_VOLUME_DECREMENT);

      Serial.println("DOWN");
      oldPosition = newPosition;
    }
  }
  else {
    if (newPosition - oldPosition == 4) {
      Serial.println("VOLUME UP");
      Consumer.write(HID_CONSUMER_VOLUME_INCREMENT);
      oldPosition = newPosition;
    } else if (oldPosition - newPosition == 4) {
      Serial.println("VOLUME DOWN");
      Consumer.write(HID_CONSUMER_VOLUME_DECREMENT);
      oldPosition = newPosition;
    }
  }
}

void setup() {
  Serial.begin(9600);
  Consumer.begin();
  Keyboard.begin();
  EEPROM.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  myOS = EEPROM.read(0);
  if (myOS != MACOS && WINDOWS) osSetup();

  oldPosition = myEnc.read();
  Serial.println("Basic Encoder Test:");
}

void loop() {
  handleButton();
  handleEncoder();
}