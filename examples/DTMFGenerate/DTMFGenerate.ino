// This example demonstrates DTMF tone generation for each digit starting from 0.

#include "dtmfgen.h"

// Create new instance of DTMF generator.
DTMFGenerator dtmf;

unsigned char key = '0';
unsigned char ledPin = 13;

void setup() 
{
  pinMode(ledPin, OUTPUT); 
}

void loop() 
{

  // Generate DTMF tone with 100ms duration.
  digitalWrite(ledPin, HIGH);
  dtmf.generate(key, 100);
  
  digitalWrite(ledPin, LOW);
  delay(1000); 
  
  // Move to next digit.
  key++;

  if(key > '9')
  {
    key = '0';
 }
}
