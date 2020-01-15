//Motor
//skerri
// prueba 2	
class Motor{
	private:
			//MOTOR
		int PUL=13; //Pin para la señal de PULso
		int DIR=12; //define Direction pin.
		int ENABLE = 11; // ENA
		int DUR; //
		float STEPPCYCLE=2000;	// Pasos por vuelta
		float PAS; 				// Cantidad de pasos que se desea que el motor dé
		float ANG=360;	// Ángulo de excursión del motor
		int angularFreq = 10; // En RPM

	}

	int main(void){
		DUR = 20;
		//DUR = 500 ; //1000;
		// CONFIGURACION MOTOR
		pinMode (PUL, OUTPUT);
		pinMode (DIR, OUTPUT);
		pinMode (ENABLE, OUTPUT);
		PAS = STEPPCYCLE*ANG/360;
		
		digitalWrite(ENABLE, HIGH);
		delayMicroseconds(100);
		digitalWrite(DIR,LOW);
		delayMicroseconds(100);
		digitalWrite(DIR,HIGH);

	}

	void sendPulses{
		digitalWrite(PUL,HIGH);
		delayMicroseconds(DUR);  //Este comando está al pedo
		digitalWrite(PUL,LOW);
	}