#include <Arduino.h>

class Encoder(){
	private:
		int encoder0PinA; // muy probablemente esté muy mal hacer esto
		int encoder0PinB;
		int encoder0Pos;

	public:
		void doEncoder(void);
		int getInterruptPin(void);
	}
