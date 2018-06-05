#include <Wire.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <SD.h>

// Define general constants
const int MEASUREMENT_ARRAY_LEN = 256;

short measurementV[256];
short measurementC[256];
int temp;
int pressure;
boolean hasMeasurement = false;
int radCount = 0;
boolean radFlag = false;

byte measurement_count = 0;
int endDelay = 0; //delay in ms
int clockDelay = 0; //delay in ms

// Define ground command constants
const int GND_CMD_GEIGER_OFF = 0;
const int GND_CMD_GEIGER_ON = 1;

// Constants for UDP connection
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
unsigned int localPort = 8888;      // local port to listen on
EthernetUDP Udp;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,

// SD-card file
File dataFile;

void setup() {
  Serial.begin(9600); // only for printing
  Wire.begin();

  //for (int i = 1; i <= 1; i++) {
  //delay(clockDelay);
  //Wire.beginTransmission(i); // transmit to device #1,2,3
  //Wire.write(1);              // sends one bit, 1 means initialize
  //Wire.endTransmission();    // stop transmitting
  //delay(0); //ms to prevent flooding the first nanos with recieve requests
  // }


  // Setup UDP
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);

  // Setup SD-card
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin()) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  // Setup SD-card
  dataFile = SD.open("datalog.txt", FILE_WRITE);
}



void get_radiation_data() {
  Wire.requestFrom(3, 2); // recieves one int from UNO
  byte readR[2];
  int position = 0;

  while (Wire.available()) { //sends an int as two bytes
    readR[position]=Wire.read();
    position++;
    if (position==2){
      radCount=getInt(readR);
      radFlag=true;
    }
  }
}

/*
   read from nano i
*/
void populate_data_arrays(int i) {
  Serial.print("Reading data from nano");
  Serial.print(i);
  byte c = 100;
  Wire.requestFrom(i, 1);
  while(Wire.available()){    // slave may send less than requeste   
    c=Wire.read();
  }
  byte value = 1; //if we read a 1 the series is ready for transmission
  while (c!=value){
    Serial.println(c);
    delay(2000);
    Wire.requestFrom(i, 1);
    while(Wire.available()){    // slave may send less than requeste   
      c=Wire.read(); 
    }
  }
  Serial.println("cont");

  // Read data when nano is ready
  for (int j = 0; j < 256; j++) { //Recieves a measurmentV and a measurmentC array
    int position = 0;
    byte readV[2];
    byte readC[2];

    Wire.requestFrom(i, 4); // recieves one V and one C measurement 4 is for number of bytes
    while (Wire.available()) { //sends an int as two bytes
      if (position < 2) {
        readV[position] = Wire.read(); //recieves voltage value at j
        position++;
      } else {
        readC[position - 2] = Wire.read(); //recieves current value at j
        position++;
      }
      if (position == 4) {
        measurementV[j] = getInt(readV); //assembles the measurments
        measurementC[j] = getInt(readC);
        char* s;
        sprintf(s,"V=%d,\tC=%d", measurementV[j], measurementC[j]);
        Serial.print(s);
        break;
      }
    }
    if (j == 255) {
      hasMeasurement = true;
    }
  }
}

// function converts an array of two bytes into an int
int getInt (byte B[]) {
  int first = (2 ^ 8) * (int)(B[0]);
  int second = (int)(B[1]);
  //Serial.println("first");
  //Serial.println(first);
  //Serial.println(second);
  return first + second;
}

void get_temp_data() {
  temp += 1;
}

void get_pressure_data() {
  pressure += 1;
}

int handle_ground_command(char command) 
{
  switch (command) {
    case GND_CMD_GEIGER_OFF:
      // turn off geiger tube
      break;
    case GND_CMD_GEIGER_ON:
      // Turn on geiger tube
      break;
  }
}

String format_data_string() {
  String s = "";
  for (int i=0; i<MEASUREMENT_ARRAY_LEN; i++) {
    s += measurementC[i] + "," + measurementV[i];
  }
}

void emmulate_data_arrays() {
  for (int i=0; i<256; i++) {
    measurementV[i] = i;
    measurementC[i] = i;
  }
}

void emmulate_rad_data() {
  radCount = 12;
}

void loop() {
  /*
     Handle ground commands
  */
  /*
  char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // Read content of UDP package into packetBuffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  
    // Handle command
    char command = packetBuffer[0];
    handle_ground_command(command);
  }
  */


  /*
     Read measurement data
  */
  //populate_data_arrays(measurement_count % 2 + 1);
  emmulate_data_arrays();
  // get radiation data
  //get_radiation_data();
  emmulate_rad_data();
  // get temp data
  get_temp_data();
  // get pressure data
  get_pressure_data();

  
  /*
     Format data to string
  */
  String dataString = format_data_string();


  /*
     Write data to SD-card
  */
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  else {// if the file isn't open, pop up an error:
    Serial.println("error opening datalog.txt");
  }


  /*
     Send data to ground
  */


  measurement_count++;
  delay(endDelay);
}
