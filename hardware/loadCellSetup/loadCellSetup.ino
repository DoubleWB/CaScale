#include "HX711.h"

HX711 scale;
int state;


void setup() {
  state = 1;
  Serial.begin(38400);
}

void loop() {

  float reading;
  int availableNum = 0;

  switch (state) {
    case 1: // Take a couple steps to calibrate the scale
      Serial.print("Begin Setup!\n");
      while (Serial.available() == availableNum) { }
      availableNum = Serial.available();
      scale.begin(3, 2);
      scale.set_scale();
      Serial.print("Scale Set!\n");
      Serial.print("Enter a single character to advance!\n");

      while (Serial.available() == availableNum) { }
      availableNum = Serial.available();
      scale.tare();
      Serial.print("Scale Tared!\n");
      Serial.print("Place a known weight on the scale!\n");
      Serial.print("Enter a single character to advance!\n");

      while (Serial.available() == availableNum) { }
      availableNum = Serial.available();
      reading = scale.get_units(10);
      Serial.print("Scale took a reading:\n");
      Serial.print(reading);
      Serial.print("\n");
      Serial.print("Enter a single character to advance!\n");

      scale.set_scale((1/30.0) * reading);

      state = 2;
      break;
    case 2: //Initialize data stream, an average of 10 readings printed every 2 seconds.
      Serial.print("Reading is:\n");
      Serial.println(scale.get_units(10));
      delay(2000);
      break;
  }
}
