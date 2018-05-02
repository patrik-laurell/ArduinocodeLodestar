int pinV = A1;  //Analog pin measuring voltage
int pinA = A0;  //Analog pin measuring current
int pinGate = 3; // digital pin controlling the gate voltage

//int period = 256; //Period time for a square sequence
int duty[256]; //Array filled with values for how long the square part will be in the pulse.
int settlingTime = 3; //(in ms)
int measurmentPause = 5000; //(in ms)
int tracker = 0; //Keeping track of the current measurment series
int measuredV[256]; //Array storing the measured voltages for a series
int measuredA[256]; //Array storing the measured current for a series
int readTime = 100; // in us
int samples = 300; // number of saples per duty

void setup() {
  Serial.begin(9600);

  for (int j = 0; j < 256; j++) {
    duty[j] = j;
  }
}
void loop() {
  Serial.print("Measurment series ");
  Serial.print(tracker);
  Serial.println(" initiated");

  for (int j = 0; j < 256; j++) {
    analogWrite(pinGate, duty[j]); //Creates square waves with the current duty
    delay(settlingTime); //Circuit is allowed to enter steadystate

    int32_t averageV = 0;
    int32_t averageA = 0;
    
    for (int i = 0; i < samples; i++) {
    averageV = averageV + analogRead(pinV); //Measures V
    averageA = averageA + analogRead(pinA); //Measures A
    delayMicroseconds(readTime); //Prevents interferance when measuring  
    }
    measuredV[j] = averageV/samples;
    measuredA[j] = averageA/samples;
    
    Serial.print(duty[j]);
    Serial.print(" ");
    Serial.print(measuredV[j]);
    Serial.print(" ");
    Serial.println(measuredA[j]);
  }
  //exports measuredV and measuredA with tracker to Arduino Mega
  tracker++;
  delay(measurmentPause); //Delays the next measurment series so not to flood the receiver
}

