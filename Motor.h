#include <Arduino.h>

class Motor
{
	public:
		Motor(int PULSEpin, int DIRpin, int ENABLEpin,int duration);
		int getDuration();
		void sendPulses();
	private:
		int PULSEpin; //Pin para la señal de PULso
		int DIRpin; //define Direction pin.
		int ENABLEpin; // ENA
		int duration; //
};