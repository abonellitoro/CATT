#include <Arduino.h>
#include "LiquidCrystal_I2C.h"

class SDManager{
	public:
		SDManager(int chipSelect);
		bool begin();
		String checkIfSDExists();
		void writeSD(float torque, float desplAng);
		void initializeSD(LiquidCrystal_I2C lcd, int pin);

	private:
		String getPadded(int num);
		String prefix;
		String filename;
		String realFilename;
		int chipSelect;
	};