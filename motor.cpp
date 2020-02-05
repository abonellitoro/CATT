#include "Motor.h"

Motor::Motor(int PULSEpin, int DIRpin, int ENABLEpin,int duration){
		
		this->duration = duration;
		this->PULSEpin = PULSEpin;
		// this-> DIRpin = DIRpin;
		// this-> ENABLEpin = ENABLEpin;

		pinMode (PULSEpin, OUTPUT);
		pinMode (DIRpin, OUTPUT);
		pinMode (ENABLEpin, OUTPUT);

		digitalWrite(ENABLEpin, LOW);
		delayMicroseconds(100);
		digitalWrite(DIRpin,LOW);
}

int Motor::getDuration(void){
	return duration;
	}

void Motor::sendPulses(void){
		digitalWrite(PULSEpin,HIGH);
		digitalWrite(PULSEpin,LOW);
	}