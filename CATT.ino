//#include <LiquidCrystal.h>
#include <HX711_ADC.h>
#include <EEPROM.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>
#include "Encoder.h"

// SD
const int chipSelect = 53;
String prefix = "log_";
String filename = prefix + String("000") + String(".txt");
String realFilename;

// LCD
const int 	rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8 , 
			d0 = 50, d1 = 51, d2 = 52, d3 = 53;
//LiquidCrystal lcd(rs, en, d0, d1, d2, d3, d4, d5, d6, d7);
LiquidCrystal_I2C lcd(0x27,16,2);  //

//ENCODER
// int encoder0PinA = 19;
// int encoder0PinB = 18;
// int encoder0Pos = 0;


//int ledPin = A0;
//volatile byte state = LOW;

//LOOP LENTO
int Delta = 60;
int Delta_millis;
int millis_Ant;
int millis_Now;


//MOTOR
int PUL=13; //Pin para la señal de PULso
int DIR=12; //define Direction pin.
int ENABLE = 11; // ENA
int DUR; //
float STEPPCYCLE=2000;	// Pasos por vuelta
float PAS; 				// Cantidad de pasos que se desea que el motor dé
//float ANG=0.5;			
float ANG=360;	// Ángulo de excursión del motor
int angularFreq = 10; // En RPM
//int CONTADOR=0;

//HX711 constructor (dout pin, sck pin):
const int dout = 2, sckpin = 3;
HX711_ADC LoadCell(2, 3);
const int eepromAdress = 0;
float calValue; // calibration value
long t;
Encoder encoder();

void setup() {
	lcd.init();
	lcd.backlight();
	
// LCD
	//lcd.begin(16, 2);
  	lcd.print("Medicion de");
  	lcd.setCursor(0, 1);
  	lcd.print("Torque");
	lcd.display();

	// CELDA DE CARGA
	calValue = 20800.0;
	Serial.begin(9600);
	Serial.println(filename);
	delay(10);
	Serial.println("Iniciando celda de carga...");
	LoadCell.begin();
	long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
	LoadCell.start(stabilisingtime);
	if(LoadCell.getTareTimeoutFlag()) {
		Serial.println("Tare timeout, check MCU>HX711 wiring and pin designations");
		}
	else {
		LoadCell.tare();
		LoadCell.setCalFactor(calValue); // set calibration value (float)
		//LoadCell.setGain(gain); // 
		Serial.println("Startup + tare is complete");
		}

	if (!SD.begin(chipSelect)) {
		lcd.clear();
    	lcd.print("La SD fallo");
    	lcd.setCursor(0, 1);
  		lcd.print("Revisar conexiones");

    	delay(8000);
    	lcd.clear();
    	digitalWrite(A0, HIGH);
  		}
  	else {
  		Serial.println("Inicio correcto");
  		realFilename = checkIfSDExists();
  	}
	
			
	// ENCODER
	// pinMode(encoder0PinA, INPUT);
	// pinMode(encoder0PinB, INPUT);
	int attachPin = encoder.getInterruptPin()
	attachInterrupt(digitalPinToInterrupt(attachPin), encoder.doEncoder(), CHANGE);  // encoder pin on interrupt 0 - pin 2

	

	// 	MOTOR 
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
	
	
void loop() {
	//Serial.println("Rapido");
	millis_Now = millis();
  	Delta_millis = millis_Now - millis_Ant;

	/////////////////////////////////////////////////
	//////////////// LOOP RÁPIDO ////////////////////
	///////////////////////////////////////////////////
	///////////////// CELDA DE CARGA //////////////////
	LoadCell.update();
	//  LoadCell.setGain(gain);

	//get smoothed value from data set
//	if (millis() > t + 250) {
		float torque = LoadCell.getData();
		// lcd.clear();
		// lcd.print("Torque [Nm] ");
		// lcd.print(torque);
		t = millis();

	unsigned long start = millis();


	//////////////// LOOP LENTO /////////////////////
	if(Delta_millis >= Delta)
		{
		//Serial.println("Lento");
		millis_Ant = millis_Now;


		//////////////// MOTOR //////////////////////////
		digitalWrite(PUL,HIGH);
	    delayMicroseconds(DUR);  //Este comando está al pedo
	    digitalWrite(PUL,LOW);
		unsigned long end = millis();
		unsigned long delta = end - start;
		//float omega = ANG/(delta/1000.*60.);
		
		//if(Delta_millis >= Delta*5)
		lcd.clear();
		lcd.print("Torque [Nm] ");
		lcd.print(torque);
		lcd.setCursor(0,1);
		// lcd.print("[RPM]:");
		// lcd.print(omega);
		lcd.print("Desp: ");

		float desplAng;
		int zeroPos = encoder.getEncoder0Pos();
		desplAng = zeroPos/1000.*360.;
		lcd.print(desplAng);
		
		// SD Write
		writeSD(torque, desplAng);
    }// Loop lento End
    
}

// void doEncoder() {
  //state = !state;
  /* If pinA and pinB are both high or both low, it is spinning
     forward. If they're different, it's going backward.

     For more information on speeding up this process, see
     [Reference/PortManipulation], specifically the PIND register.
  */
	  // if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
	  // 	encoder0Pos++;
	  // } else {
	  // encoder0Pos--;
	  // }

  //Serial.println (encoder0Pos, DEC);
//}

String checkIfSDExists(void){
	bool sdCardFull = false; 
	byte num = 0;
	String strNum;
	String newfilename;
	newfilename = filename;
    while(SD.exists(newfilename) ){
        Serial.println("existo");
        if( num == 999 ){          // If the SD card is full
            sdCardFull = true;
            }
	        num++;
	        newfilename = prefix + getPadded(num) + String(".txt");
	        Serial.println(newfilename);
    }
    return newfilename;
}


void writeSD(float torque, float desplAng){
	String dataString = "";
	dataString += String(desplAng);
	dataString += ",";
	dataString += String(torque);

	File dataFile = SD.open(realFilename, FILE_WRITE);
	// if the file is available, write to it:
	if (dataFile) {
		dataFile.println(dataString);
		dataFile.close();
		// print to the serial port too:
		Serial.println(dataString);
}}


String getPadded(int num) {
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