#include <Arduino.h>
#include "Encoder.h"

Encoder::Encoder(int encoder0PinA,	int encoder0PinB, int encoder0Pos){
	// this->encoder0PinA=encoder0PinA;
	// this->encoder0PinB=encoder0PinB;
	// this->encoder0Pos=encoder0Pos;
}

void Encoder::doEncoder() {
	  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
	  	encoder0Pos++;
	  } else {
	  encoder0Pos--;
	  }

  //Serial.println (encoder0Pos, DEC);
}

int Encoder::getInterruptPin(){
	return encoder0PinB;
}

// void Encoder::defineInterruptPin(){
// 		attachInterrupt(digitalPinToInterrupt(18), doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2

// }

int Encoder::getEncoder0Pos(){
	return encoder0Pos;
}