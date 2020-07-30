#include <SD.h>
#include <SPI.h>
#include <virtuabotixRTC.h>

#define NUM_SAMPLES 10

//Duty Cycle
int pinPWM = 13;      // Pin PWM
int AnalogValue;
int DutyCycle;

virtuabotixRTC myRTC(6, 7, 8);

File file;

const int chipSelect = 53; 

//Sensor Arus Masukkan
//int sumCurrents = 0;
//float Is; //
//unsigned char sample_countIs = 0;

//Sensor Arus Induktor 1
int sumCurrentL1 = 0;
float IL1; //
unsigned char sample_countIL1 = 0;

//Sensor Arus Keluaran
//int sumCurrento = 0;
//float Io; //
//unsigned char sample_countIo = 0;

//Sensor Tegangan
//int sumVoltage = 0;
//float V; //Nilai tegangan masukan
//unsigned char sample_countV = 0;

//float R1 = 120000;
//float R2 = 22000;

void setup() {
 Serial.begin(9600); //Setting baudrate at 9600

 pinMode(pinPWM, OUTPUT);  // sets the pin as output
 
 pinMode(chipSelect, OUTPUT);//declaring CS pin as output pin

 if (!SD.begin(chipSelect)) { // Initialize SD card
    Serial.println("Could not initialize SD card."); // if return value is false, something went wrong.
  }
  
  //MODIFIEEEEEEEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
  if (SD.exists("100ohm17.txt")) { // if "file.txt" exists, fill will be deleted
    Serial.println("File exists.");
    if (SD.remove("100ohm17.txt") == true) {
      Serial.println("Successfully removed file.");
    } else {
      Serial.println("Could not removed file.");
    }
  }

  // seconds, minutes, hours, day of the week, day of the month, month, year
  myRTC.setDS1302Time(00, 30, 22, 2, 14, 5, 2019);
  
  //Mengubah Frekuensi clock menjadi 62500 Hz
  TCCR0B = TCCR0B & 0b11111000 | 0x01;
}
 
void loop() {
    myRTC.updateTime();
    
    // MODIFIEEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    DutyCycle = 41;
    AnalogValue = 2.55*DutyCycle;
    analogWrite(pinPWM, AnalogValue); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  
//    while (sample_countV < NUM_SAMPLES) {
//        sumVoltage += analogRead(A0);
//        sample_countV++;
//        delay(10);
//    }

    //Sampel Arus Masukkan
//    while (sample_countIs < NUM_SAMPLES) {
//        sumCurrents += analogRead(A1);
//        sample_countIs++;
//        delay(10);
//    }

    //Sampel Arus Induktor 1
    while (sample_countIL1 < NUM_SAMPLES) {
        sumCurrentL1 += analogRead(A1);
        sample_countIL1++;
        delay(10);
    }

    //Sampel Arus Keluaran
//    while (sample_countIo < NUM_SAMPLES) {
//        sumCurrento += analogRead(A3);
//        sample_countIo++;
//        delay(10);
//    }
    
    //Kalkukasi Tegangan
    //V = ((R1+R2)/R2)*(((float)sumVoltage / (float)NUM_SAMPLES * 5.0)/1023.0);

    //Kalkulasi Arus Masukkan
    //Is = ((((float)sumCurrents / (float)NUM_SAMPLES * 5.0) / 1023.0)-2.5)/0.185;

    //Kalkulasi Arus Induktor 1
    IL1 = ((((float)sumCurrentL1 / (float)NUM_SAMPLES * 5.0) / 1023.0)-2.5)/0.185;

    //Kalkulasi Arus Keluaran
    //Io = ((((float)sumCurrento / (float)NUM_SAMPLES * 5.0) / 1023.0)-2.5)/0.185;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    file = SD.open("100ohm17.txt", FILE_WRITE); // open "file.txt" to write data

    if (file) {

    file.print(myRTC.seconds);
    file.print(",");
    
    //file.print(V);
    //file.print(",");
    //file.print(V*1000);
    //file.print(",");

    //IS A
    //file.print(Is);
    //file.print(",");
    //file.print(Is*1000);
    //file.print(",");

    //IL A1
    file.print(IL1);
    file.print(",");
    file.println(IL1*1000);
    //file.print(",");

    //IO A3
    //file.print(Io);
    //file.print(",");
    //file.println(Io*1000);
    
    file.close(); // close file
    
    Serial.print(myRTC.seconds);
    //Serial.print("| Tegangan : "); // debug output: show written number in serial monitor
    //Serial.print(V*1000);
    //Serial.print("| Is :");
    //Serial.print(Is*1000);
    Serial.print("| IL1 :");
    Serial.println(IL1*1000);
    //Serial.print("| Io :");
    //Serial.println(Io*1000);

    
  } else {
    Serial.println("Could not open file (writing).");
  }
    
    //sample_countV = 0;
    //sumVoltage = 0;  

    //Arus Masukkan
    //sample_countIs = 0; //A1
    //sumCurrents = 0;

    //Arus Induktor 1
    sample_countIL1 = 0; //A2
    sumCurrentL1 = 0;

    //Arus Keluaran
    //sample_countIo = 0;
    //sumCurrento = 0;

  delay(40); //Total Delay 50
 }
