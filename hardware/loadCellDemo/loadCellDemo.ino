#include "HX711.h"

HX711 cell(3, 2);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println( cell.read() );
}
