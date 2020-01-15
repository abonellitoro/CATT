#include "encoder.h"

Encoder::Encoder(int encoder0PinA = 19,
				int encoder0PinB = 18,
				int encoder0Pos = 0){

}



void Encoder::doEncoder(void) {
  //state = !state;
  /* If pinA and pinB are both high or both low, it is spinning
     forward. If they're different, it's going backward.

     For more information on speeding up this process, see
     [Reference/PortManipulation], specifically the PIND register.
  */
	  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
	  	encoder0Pos++;
	  } else {
	  encoder0Pos--;
	  }

  //Serial.println (encoder0Pos, DEC);
}

int Encoder::getInterruptPin(void){
	return encoder0PinB;
}