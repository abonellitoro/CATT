#include <Arduino.h>

class SDManager{
	public:
		SDManager(int chipSelect);
		bool begin();
		String checkIfSDExists();
		void writeSD(float torque, float desplAng);

	private:
		String getPadded(int num);
		String prefix;
		String filename;
		String realFilename;
		const int chipSelect;
	};