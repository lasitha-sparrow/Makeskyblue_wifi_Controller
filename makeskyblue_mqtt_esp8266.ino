#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "------";
const char* password = "-------";
const char* mqtt_server = "--------";


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char v2[MSG_BUFFER_SIZE];
char v3[MSG_BUFFER_SIZE];
char v4[MSG_BUFFER_SIZE];
char v5[MSG_BUFFER_SIZE];
char v6[MSG_BUFFER_SIZE];
char v7[MSG_BUFFER_SIZE];
char v8[MSG_BUFFER_SIZE];
char v9[MSG_BUFFER_SIZE];

int value = 0;

#define rxPin 0
#define txPin 2

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
int i;
int read_x1;
int read_x2;
int x;

int read_modeflags;
int read_errorflags;

int checksum;
int packet_checksum;

bool receive_done =1;

void setup_wifi() {

  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

//MakeSkyBlue Code
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(9600);
  Serial.flush();
}

void(* resetFunc) (void) = 0;

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
  //Serial.flush();
   Serial.write(SendData,sizeof(SendData));
   receive_done = 0;
   Serial.flush();
   if (Serial.available()) {     // If anything comes in Serial 
   //Serial.flush();
   inByte = Serial.read();
   
   
   if(inByte == 0xAA){
    checksum =0;
    inByte = Serial.read();
    checksum += inByte;
    
   if (inByte == 0xBB){
      
  
      read_battVoltage1 = Serial.read();
      checksum += read_battVoltage1;      
      read_battVoltage2 = Serial.read();
      checksum += read_battVoltage2; 
      battVoltage = read_battVoltage1 + read_battVoltage2*256;

      
      read_battAmps1 = Serial.read();
      checksum += read_battAmps1; 
      read_battAmps2 = Serial.read();
      checksum += read_battAmps2; 
      battAmps = read_battAmps1 + read_battAmps2*256;
      

      read_solarVolts1 = Serial.read();
      checksum += read_solarVolts1; 
      read_solarVolts2 = Serial.read();
      checksum += read_solarVolts2; 
      solarVolts = read_solarVolts1 + read_solarVolts2*256;

      read_solarWatts1 = Serial.read();
      checksum += read_solarWatts1; 
      read_solarWatts2 = Serial.read();
      checksum += read_solarWatts2; 
      solarWatts = read_solarWatts1 + read_solarWatts2*256;

      read_Temp1 = Serial.read();
      checksum += read_Temp1; 
      read_Temp2 = Serial.read();
      checksum += read_Temp2; 
      Temp = read_Temp1 + read_Temp2*256;

      read_kwh1 = Serial.read();
      checksum += read_kwh1; 
      read_kwh2 = Serial.read();
      checksum += read_kwh2; 
      kwh = read_kwh1 + read_kwh2*256;

      read_x1 = Serial.read();
      checksum += read_x1; 
      read_x2 = Serial.read();
      checksum += read_x2; 
      x = read_x1 + read_x2*256;

      read_modeflags = Serial.read();
      checksum += read_modeflags; 

      read_errorflags = Serial.read();
      checksum += read_errorflags; 

      inByte = Serial.read();      
      checksum += inByte; 

      packet_checksum = Serial.read();
      checksum = checksum & 0b11111111;
 
      if (packet_checksum == checksum){
        sprintf(v2, "%ld", battVoltage*0.1);
        client.publish("v2", v2);

        sprintf(v3, "%ld", battAmps*0.1);
        client.publish("v3", v3);

        sprintf(v4, "%ld", solarVolts*0.1);
        client.publish("v4", v4);

        sprintf(v5, "%ld", solarWatts);
        client.publish("v5", v5);

        sprintf(v6, "%ld", Temp*0.1);
        client.publish("v6", v6);

        sprintf(v7, "%ld", kwh);
        client.publish("v7", v7);

        sprintf(v8, "%ld", read_modeflags);
        client.publish("v8", v8);

        sprintf(v9, "%ld", read_errorflags);
        client.publish("v9", v9);
        Serial.flush();
      }
  }
}
   }}}
