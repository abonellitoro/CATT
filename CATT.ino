//#include <LiquidCrystal.h>
#include <HX711_ADC.h>
#include <EEPROM.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
//#include <SD.h>
#include "Encoder.h"
#include "SDManager.h"
#include "Motor.h"

int counter = 0;
int onOffpin = A1;

//LOOP LENTO
int Delta = 32;
int Delta_millis;
int millis_Ant;
int millis_Now;

int Delta_millisLCD;
int millis_AntLCD;
int millis_NowLCD;

// SD (1/3)
const int chipSelect = 53;
// String prefix = "log_";
// String filename = prefix + String("000") + String(".txt");
// String realFilename;
SDManager sd(chipSelect);
// LCD (1/3)
const int 	rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8 , 
			d0 = 50, d1 = 51, d2 = 52, d3 = 53;
//LiquidCrystal lcd(rs, en, d0, d1, d2, d3, d4, d5, d6, d7);
LiquidCrystal_I2C lcd(0x27,16,2);  //


//MOTOR (1/3)
bool DIR = LOW;
int PULSEpin=13; //Pin para la señal de PULso
int DIRpin=12; //define Direction pin.
int ENABLEpin = 11; // ENA

Motor motor(PULSEpin, DIRpin, ENABLEpin, Delta)


//CELDA DE CARGA (1/3)
const int dout = 2, sckpin = 3;
HX711_ADC LoadCell(dout, sckpin);
//const int eepromAdress = 0;
float calValue; // calibration value

//ENCODER
int encoder0PinA = 19;
int encoder0PinB = 18;
int encoder0Pos = 0;
Encoder encoder(encoder0PinA,encoder0PinB, encoder0Pos);

void setup() {

	Serial.begin(9600);
	// LCD
	initializeLCD();
	encoder.init();
	// CELDA DE CARGA
	initializeLoadCell();
	//SD
	sd.initializeSD(lcd, A0);
	// //ENCODER
	attachInterrupt(digitalPinToInterrupt(encoder.getInterruptPin()), paraQueFuncioneLaInterrupcionExterna, CHANGE);  
	// 	MOTOR
	initializeMotor();

}


void loop() {
	if(digitalRead(onOffpin)==HIGH){
		while (counter<=3){
			mainFunction();
			}
	lcd.clear();
	lcd.print("Terminado");
	}
	else{
		//Serial.println("Apagao");
	}
}


void mainFunction(){
	millis_Now = millis();
  	Delta_millis = millis_Now - millis_Ant;
	millis_NowLCD = millis();
  	Delta_millisLCD = millis_NowLCD - millis_AntLCD;

	//////////////// LOOP RÁPIDO //////////////////
	
  	//CELDA DE CARGA (3/3)
	LoadCell.update();
	float torque = LoadCell.getData();
	float desplAng;
	desplAng = encoder0Pos/1000.*360.;
	
	sd.writeSD(torque, desplAng);

	//////////////// LOOP LENTO //////////////////
	if(Delta_millis >= motor.getDuration())
		{
		millis_Ant = millis_Now;

		//MOTOR
		digitalWrite(PULSEpin,HIGH);
	    digitalWrite(PULSEpin,LOW);


		if(Delta_millisLCD >= Delta*8)  {
			millis_AntLCD = millis_NowLCD;
			lcd.clear();
			lcd.print("Torque [Nm] ");
			lcd.print(torque);
			lcd.setCursor(0,1);
			lcd.print("Desp: ");
			lcd.print(desplAng);
		}
		
		// CAMBIO DE SENTIDO
		if (desplAng>20.){
			DIR = HIGH;
			digitalWrite(DIRpin, DIR);
		}
		if (desplAng<-20.){
			bool auxDIR = DIR;
			DIR = LOW;
			digitalWrite(DIRpin,DIR);
			if (auxDIR!= DIR){
				counter++;
			}
		}
    }
    // Loop lento End
}	

void paraQueFuncioneLaInterrupcionExterna(){
	encoder0Pos = encoder.doEncoder();
	// Serial.println(str);
}

void initializeLCD(void)
	{
	lcd.init();
	lcd.backlight();
  	lcd.print("Medicion de");
  	lcd.setCursor(0, 1);
  	lcd.print("Torque");
	lcd.display();
	}

// void initializeMotor(void){
// 	pinMode (PULSEpin, OUTPUT);
// 	pinMode (DIRpin, OUTPUT);
// 	pinMode (ENABLEpin, OUTPUT);
// 	digitalWrite(ENABLEpin, HIGH);
// 	delayMicroseconds(100);
// 	digitalWrite(DIRpin,DIR);
// }

void initializeLoadCell(void){
	calValue = 20800.0;
	/* Serial.begin(9600);
	Serial.println(filename);
	delay(10);
	Serial.println("Iniciando celda de carga...");*/
	LoadCell.begin();
	long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
	LoadCell.start(stabilisingtime);
	if(LoadCell.getTareTimeoutFlag()) {
		//Serial.println("Tare timeout, check MCU>HX711 wiring and pin designations");
	}
	else {
		LoadCell.tare();
		LoadCell.setCalFactor(calValue); // set calibration value (float)
		//LoadCell.setGain(gain); // 
		//Serial.println("Startup + tare is complete");
	}
}

// void initializeSD(){
// 	if (!sd.begin()) {
// 		lcd.clear();
// 		lcd.print("La SD fallo");
// 		lcd.setCursor(0, 1);
// 		lcd.print("Revisar conexiones");

// 		//delay(8000);
// 		lcd.clear();
// 		digitalWrite(A0, HIGH);
// 		}
// 	else {
// 		//Serial.println("Inicio correcto");
// 		realFilename = sd.checkIfSDExists();
// 		lcd.clear();
// 		lcd.print(realFilename);
// 	}

// }