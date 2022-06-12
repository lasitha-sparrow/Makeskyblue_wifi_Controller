
#define RXD2 17
#define TXD2 16

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
void setup() {
 Serial.begin(9600);
  
 Serial1.begin(9600);


}

void myTimerEvent() // This loop defines what happens when timer is triggered
{
  
//   Serial1.flush();
   //Serial.println("Sending request"); 
   Serial.write(SendData,sizeof(SendData));
   receive_done = 0;
   
  
   
   
   if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
   inByte = Serial1.read();
   
   Serial.print(inByte, HEX);Serial.print(' ');
   
   if(inByte == 0xAA){
    checksum =0;
    Serial.print(inByte, HEX);Serial.print(' ');
    inByte = Serial1.read();
    checksum += inByte;
    
    if (inByte == 0xBB){
      
      Serial.print(inByte, HEX);Serial.print(' ');
      read_battVoltage1 = Serial1.read();
      checksum += read_battVoltage1;      
      Serial.print(read_battVoltage1, HEX);Serial.print(' ');
      read_battVoltage2 = Serial1.read();
      checksum += read_battVoltage2; 
      Serial.print(read_battVoltage2, HEX);Serial.print(' ');
      battVoltage = read_battVoltage1 + read_battVoltage2*256;
      Serial.print("Battery Voltage: ");Serial.println(battVoltage*0.1);

      
      read_battAmps1 = Serial1.read();
      checksum += read_battAmps1; 
      Serial.print(read_battAmps1, HEX);Serial.print(' ');
      read_battAmps2 = Serial1.read();
      checksum += read_battAmps2; 
      Serial.print(read_battAmps2, HEX);Serial.print(' ');
      battAmps = read_battAmps1 + read_battAmps2*256;
      Serial.print("Battery Amps: ");Serial.println(battAmps*0.1);
      

      read_solarVolts1 = Serial1.read();
      checksum += read_solarVolts1; 
      Serial.print(read_solarVolts1, HEX);Serial.print(' ');
      read_solarVolts2 = Serial1.read();
      checksum += read_solarVolts2; 
      Serial.print(read_solarVolts2, HEX);Serial.print(' ');
      solarVolts = read_solarVolts1 + read_solarVolts2*256;
      Serial.print("Solar Volts: ");Serial.println(solarVolts*0.1);

      read_solarWatts1 = Serial1.read();
      checksum += read_solarWatts1; 
      Serial.print(read_solarWatts1, HEX);Serial.print(' ');
      read_solarWatts2 = Serial1.read();
      checksum += read_solarWatts2; 
      Serial.print(read_solarWatts2, HEX);Serial.print(' ');
      solarWatts = read_solarWatts1 + read_solarWatts2*256;
      Serial.print("Solar Watts: ");Serial.println(solarWatts);

      read_Temp1 = Serial1.read();
      checksum += read_Temp1; 
      Serial.print(read_Temp1, HEX);Serial.print(' ');
      read_Temp2 = Serial1.read();
      checksum += read_Temp2; 
      Serial.print(read_Temp2, HEX);Serial.print(' ');
      Temp = read_Temp1 + read_Temp2*256;
      Serial.print("Temperature: ");Serial.println(Temp*0.1);

      read_kwh1 = Serial1.read();
      checksum += read_kwh1; 
      Serial.print(read_kwh1, HEX);Serial.print(' ');
      read_kwh2 = Serial1.read();
      checksum += read_kwh2; 
      Serial.print(read_kwh2, HEX);Serial.print(' ');
      kwh = read_kwh1 + read_kwh2*256;
      Serial.print("Accumulated kWh: ");Serial.println(kwh);

      read_x1 = Serial1.read();
      checksum += read_x1; 
      Serial.print(read_x1, HEX);Serial.print(' ');
      read_x2 = Serial1.read();
      checksum += read_x2; 
      Serial.print(read_x2, HEX);Serial.print(' ');
      x = read_x1 + read_x2*256;
      Serial.print("unknown: ");Serial.println(x);

      read_modeflags = Serial1.read();
      checksum += read_modeflags; 
      Serial.print(read_modeflags, HEX);Serial.print(' ');
      Serial.print("Mode: ");Serial.println(read_modeflags);

      read_errorflags = Serial1.read();
      checksum += read_errorflags; 
      Serial.print(read_errorflags, HEX);Serial.print(' ');
      Serial.print("Error: ");Serial.println(read_errorflags);

      inByte = Serial1.read();      
      checksum += inByte; 
      Serial.print(inByte, HEX);Serial.print(' ');

      packet_checksum = Serial1.read();
      Serial.print(packet_checksum, HEX);Serial.print(' ');Serial.println();
      checksum = checksum & 0b11111111;
      Serial.print("Checksum : ");Serial.println(checksum, HEX);

      if (packet_checksum == checksum){
        Serial.println("checksum ok");
        Serial.println(battVoltage*0.1);
        Serial.println(battAmps*0.1);
        Serial.println(solarVolts*0.1);
        Serial.println(solarWatts);
        Serial.println(Temp*0.1);
        Serial.println(kwh);
        Serial.println(read_modeflags);
        Serial.println(read_errorflags);

      }else{

       Serial.println("checksum fail");
       Serial1.flush();
      }

   
      Serial.println();
      Serial.println("-------------------------------");
      Serial.println();

//      receive_done = 1;
      

      
    }
   }
   Serial1.flush();
  }
}

void loop() {
  myTimerEvent();
  delay(2000);
}
void serialFlush(){
  while(Serial1.available() > 0) {
    char t = Serial1.read();
    Serial.println(t);Serial.print(' ');
  }
}
