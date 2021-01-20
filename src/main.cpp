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
Encoder myEnc(6, 5);
//   avoid using pins with LEDs attached
uint8_t up = 0;
uint8_t down = 0;

int oldState = HIGH;
long oldPosition  = -999;
bool fineAdj = false;
long holdTimer;


void macSetup() {
  while (digitalRead(BUTTON_PIN) == HIGH);
    Keyboard.press(KEY_Z);
  while (digitalRead(BUTTON_PIN) == LOW);
    Keyboard.releaseAll();
  while (digitalRead(BUTTON_PIN) == HIGH);
    Keyboard.press(KEY_SLASH);
  while (digitalRead(BUTTON_PIN) == LOW);
    Keyboard.releaseAll();
  EEPROM.write(0, 0);
}

void handleButton() {
  int newState = digitalRead(BUTTON_PIN);
  // Button logic
  if (newState == LOW) {
    if (millis() - holdTimer > 5000) {
      Serial.println("Held!");

      fineAdj = !fineAdj;
      EEPROM.write(0, 1);
      macSetup();

      holdTimer = millis();
    }

    if (newState != oldState) {
      Serial.println("CHANGE");
      fineAdj ? fineAdj = false : fineAdj = true;
    }
  } else {
    holdTimer = millis();
  }
  oldState = newState;
}

void handleEncoder() {
  long newPosition = myEnc.read();
  // Rotate logic
  if (newPosition - oldPosition == 4) {
    if (fineAdj) {
      Serial.print("FINE ");
      Keyboard.press(HID_KEYBOARD_LEFT_SHIFT);
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Consumer.write(HID_CONSUMER_VOLUME_INCREMENT);
      Keyboard.releaseAll();
    } else {
      Consumer.write(HID_CONSUMER_VOLUME_INCREMENT);
    }
    Serial.println("UP");
    up = 0;
    oldPosition = newPosition;
  } else if (oldPosition - newPosition == 4) {
    if (fineAdj) {
      Serial.print("FINE ");
      Keyboard.press(HID_KEYBOARD_LEFT_SHIFT);
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Consumer.write(HID_CONSUMER_VOLUME_DECREMENT);
      Keyboard.releaseAll();
    } else {
      Consumer.write(HID_CONSUMER_VOLUME_DECREMENT);
    }
    Serial.println("DOWN");
    down = 0;
    oldPosition = newPosition;
  }
}

void setup() {
  Serial.begin(9600);
  Consumer.begin();
  Keyboard.begin();
  EEPROM.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  if (EEPROM.read(0) == 1) {
    macSetup();
  }

  oldPosition = myEnc.read();
  Serial.println("Basic Encoder Test:");
}

void loop() {
  handleButton();
  handleEncoder();
}