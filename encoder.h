#include <Arduino.h>



class Encoder {
	private:
		int encoder0PinA; // muy probablemente esté muy mal hacer esto
		int encoder0PinB;
		int encoder0Pos;

	public:
		Encoder(int encoder0PinA, int encoder0PinB, int encoder0Pos);
		void doEncoder();
		int getInterruptPin();
		int getEncoder0Pos();
		//void defineInterruptPin();
	};
