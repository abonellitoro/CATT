#include "Encoder.h"

Encoder::Encoder(int encoder0PinA,	int encoder0PinB, int encoder0Pos){
	this->encoder0PinA=encoder0PinA;
	this->encoder0PinB=encoder0PinB;
	this->encoder0Pos=encoder0Pos;

}

void Encoder::init(void){
	pinMode(encoder0PinA, INPUT);
	pinMode(encoder0PinB, INPUT);
}

int Encoder::doEncoder(void) {
	  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
	  	encoder0Pos++;
	  } else {
	  	encoder0Pos--;
	  }
	  // Serial.print("Clase:   ");
	  // Serial.print(encoder0PinA);
	  // Serial.print("; ");
	  // Serial.println(encoder0PinB);
	  return encoder0Pos;

  //Serial.println (encoder0Pos, DEC);
}

int Encoder::getInterruptPin(){
	return encoder0PinB;
}

int Encoder::getEncoder0Pos(){
	return encoder0Pos;
}

// void Encoder::defineInterruptPin(){
// 		attachInterrupt(digitalPinToInterrupt(18), doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2
// }