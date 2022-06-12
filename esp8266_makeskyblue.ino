/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPL27O5CSe3"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "DDK5NO2LIc6ENj1gliN1p3uOGdDUEAGS"
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define rxPin 6
#define txPin 7
SoftwareSerial pp1 =  SoftwareSerial(rxPin, txPin);
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "com.stark.inc";
char pass[] = "Nba7rfql";

BlynkTimer timer;
//=====================================================================
int inByte=0;
byte SendData[] = {0xAA,0x55,0x00,0x00,0x00,0x55};

int read_battVoltage1;
int read_battVoltage2;
int battVoltage;

int read_battAmps1;
int read_battAmps2;
int battAmps;

int read_solarVolts1;
int read_solarVolts2;
int solarVolts;

int read_solarWatts1;
int read_solarWatts2;
int solarWatts;

int read_Temp1;
int read_Temp2;
int Temp;

int read_kwh1;
int read_kwh2;
int kwh;

int read_x1;
int read_x2;
int x;

int read_modeflags;
int read_errorflags;

int checksum;
int packet_checksum;

bool receive_done =1;
//=====================================================================
void setup()
{
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // Debug console
  Serial.begin(9600);
  pp1.begin(9600);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(2500L, myTimerEvent);
}
//-------------------------------------------
void myTimerEvent() // This loop defines what happens when timer is triggered
{
  
//   pp1.flush();
   Serial.println("Sending request"); 
   pp1.write(SendData,sizeof(SendData));
   receive_done = 0;
   
  
   
   
   if (pp1.available()) {     // If anything comes in pp1 (pins 0 & 1)
   inByte = pp1.read();
   
   Serial.print(inByte, HEX);Serial.print(' ');
   
   if(inByte == 0xAA){
    checksum =0;
    Serial.print(inByte, HEX);Serial.print(' ');
    inByte = pp1.read();
    checksum += inByte;
    
    if (inByte == 0xBB){
      
      Serial.print(inByte, HEX);Serial.print(' ');
      read_battVoltage1 = pp1.read();
      checksum += read_battVoltage1;      
      Serial.print(read_battVoltage1, HEX);Serial.print(' ');
      read_battVoltage2 = pp1.read();
      checksum += read_battVoltage2; 
      Serial.print(read_battVoltage2, HEX);Serial.print(' ');
      battVoltage = read_battVoltage1 + read_battVoltage2*256;
      Serial.print("Battery Voltage: ");Serial.println(battVoltage*0.1);

      
      read_battAmps1 = pp1.read();
      checksum += read_battAmps1; 
      Serial.print(read_battAmps1, HEX);Serial.print(' ');
      read_battAmps2 = pp1.read();
      checksum += read_battAmps2; 
      Serial.print(read_battAmps2, HEX);Serial.print(' ');
      battAmps = read_battAmps1 + read_battAmps2*256;
      Serial.print("Battery Amps: ");Serial.println(battAmps*0.1);
      

      read_solarVolts1 = pp1.read();
      checksum += read_solarVolts1; 
      Serial.print(read_solarVolts1, HEX);Serial.print(' ');
      read_solarVolts2 = pp1.read();
      checksum += read_solarVolts2; 
      Serial.print(read_solarVolts2, HEX);Serial.print(' ');
      solarVolts = read_solarVolts1 + read_solarVolts2*256;
      Serial.print("Solar Volts: ");Serial.println(solarVolts*0.1);

      read_solarWatts1 = pp1.read();
      checksum += read_solarWatts1; 
      Serial.print(read_solarWatts1, HEX);Serial.print(' ');
      read_solarWatts2 = pp1.read();
      checksum += read_solarWatts2; 
      Serial.print(read_solarWatts2, HEX);Serial.print(' ');
      solarWatts = read_solarWatts1 + read_solarWatts2*256;
      Serial.print("Solar Watts: ");Serial.println(solarWatts);

      read_Temp1 = pp1.read();
      checksum += read_Temp1; 
      Serial.print(read_Temp1, HEX);Serial.print(' ');
      read_Temp2 = pp1.read();
      checksum += read_Temp2; 
      Serial.print(read_Temp2, HEX);Serial.print(' ');
      Temp = read_Temp1 + read_Temp2*256;
      Serial.print("Temperature: ");Serial.println(Temp*0.1);

      read_kwh1 = pp1.read();
      checksum += read_kwh1; 
      Serial.print(read_kwh1, HEX);Serial.print(' ');
      read_kwh2 = pp1.read();
      checksum += read_kwh2; 
      Serial.print(read_kwh2, HEX);Serial.print(' ');
      kwh = read_kwh1 + read_kwh2*256;
      Serial.print("Accumulated kWh: ");Serial.println(kwh);

      read_x1 = pp1.read();
      checksum += read_x1; 
      Serial.print(read_x1, HEX);Serial.print(' ');
      read_x2 = pp1.read();
      checksum += read_x2; 
      Serial.print(read_x2, HEX);Serial.print(' ');
      x = read_x1 + read_x2*256;
      Serial.print("unknown: ");Serial.println(x);

      read_modeflags = pp1.read();
      checksum += read_modeflags; 
      Serial.print(read_modeflags, HEX);Serial.print(' ');
      Serial.print("Mode: ");Serial.println(read_modeflags);

      read_errorflags = pp1.read();
      checksum += read_errorflags; 
      Serial.print(read_errorflags, HEX);Serial.print(' ');
      Serial.print("Error: ");Serial.println(read_errorflags);

      inByte = pp1.read();      
      checksum += inByte; 
      Serial.print(inByte, HEX);Serial.print(' ');

      packet_checksum = pp1.read();
      Serial.print(packet_checksum, HEX);Serial.print(' ');Serial.println();
      checksum = checksum & 0b11111111;
      Serial.print("Checksum : ");Serial.println(checksum, HEX);

      if (packet_checksum == checksum){
        Serial.println("checksum ok");
        Blynk.virtualWrite(V2, battVoltage*0.1);
        Blynk.virtualWrite(V3, battAmps*0.1);
        Blynk.virtualWrite(V4, solarVolts*0.1);
        Blynk.virtualWrite(V5, solarWatts);
        Blynk.virtualWrite(V6, Temp*0.1);
        Blynk.virtualWrite(V7, kwh);
        Blynk.virtualWrite(V8, read_modeflags);
        Blynk.virtualWrite(V9, read_errorflags);

      }else{

       Serial.println("checksum fail");
       pp1.flush();
      }

   
      Serial.println();
      Serial.println("-------------------------------");
      Serial.println();

//      receive_done = 1;
      

      
    }
   }
   pp1.flush();
  }
}
//-------------------------------------------




void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
void serialFlush(){
  while(pp1.available() > 0) {
    char t = pp1.read();
    Serial.println(t);Serial.print(' ');
  }
}
