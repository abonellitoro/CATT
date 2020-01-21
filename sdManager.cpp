#include "SDManager.h"
#include <SD.h>

SDManager::SDManager(int chipSelect) {
		prefix = "log_";
		filename = prefix + String("000") + String(".txt");
		chipSelect = chipSelect;
}

bool SDManager::begin(void){
	return SD.begin(chipSelect);
}

String SDManager::checkIfSDExists(void){
	bool sdCardFull = false; 
	byte num = 0;
	String strNum;
	String newfilename;
	newfilename = filename;
    while(SD.exists(newfilename) ){
        if( num == 999 ){          // If the SD card is full
            sdCardFull = true;
            }
	        num++;
	        newfilename = prefix + getPadded(num) + String(".txt");
	        //Serial.println(newfilename);
    		}
    return newfilename;
}


void SDManager::writeSD(float torque, float desplAng){
	String dataString = "";
	dataString += String(desplAng);
	dataString += ",";
	dataString += String(torque);

	File dataFile = SD.open(realFilename, FILE_WRITE);

	// if the file is available, write to it:
	if (dataFile) {
		dataFile.println(dataString);
		dataFile.close();
		//Serial.println(dataString);
}}


String SDManager::getPadded(int num) {
  char buff[4];
  char padded[5];
  
  //sprintf function will convert the long to a string
  sprintf(buff, "%.3u", num); // buff will be "01238"

  padded[0] = buff[0];
  padded[1] = buff[1];
  padded[2] = buff[2];
  padded[3] = buff[3];
  padded[4] = '\0'; // The terminating NULL

  return String(padded);
}