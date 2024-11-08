#include <avr/sleep.h>

// Pin Definitions for IR Sensors (2 per lane)
const int IR1_A = 2; // Lane A, Sensor 1
const int IR2_A = 3; // Lane A, Sensor 2
const int IR1_B = 4; // Lane B, Sensor 1
const int IR2_B = 5; // Lane B, Sensor 2
const int IR1_C = 6; // Lane C, Sensor 1
const int IR2_C = 7; // Lane C, Sensor 2
const int IR1_D = 8; // Lane D, Sensor 1
const int IR2_D = 9; // Lane D, Sensor 2

// Pin Definitions for Traffic Lights (Green, Yellow, Red)
const int green_A = 10;
const int yellow_A = 11;
const int red_A = 12;
const int green_B = 13;
const int yellow_B = 14;
const int red_B = 15;
const int green_C = 16;
const int yellow_C = 17;
const int red_C = 18;
const int green_D = 19;
const int yellow_D = 20;
const int red_D = 21;

// Timing Definitions (milliseconds)
const int HIGH_TRAFFIC_DURATION = 10000; // 10 seconds
const int LOW_TRAFFIC_DURATION = 5000;   // 5 seconds
const int NO_TRAFFIC_DURATION = 0;       // No green light if no traffic
const int YELLOW_DURATION = 2000;        // 2 seconds

// Variables to store traffic density for each lane
volatile int trafficDensityA = 0;
volatile int trafficDensityB = 0;
volatile int trafficDensityC = 0;
volatile int trafficDensityD = 0;

// Variables for round-robin scheduling
int currentLane = 0;

void setup() {
  // Setup IR sensors as inputs
  pinMode(IR1_A, INPUT);
  pinMode(IR2_A, INPUT);
  pinMode(IR1_B, INPUT);
  pinMode(IR2_B, INPUT);
  pinMode(IR1_C, INPUT);
  pinMode(IR2_C, INPUT);
  pinMode(IR1_D, INPUT);
  pinMode(IR2_D, INPUT);

  // Setup traffic lights as outputs
  pinMode(green_A, OUTPUT);
  pinMode(yellow_A, OUTPUT);
  pinMode(red_A, OUTPUT);
  pinMode(green_B, OUTPUT);
  pinMode(yellow_B, OUTPUT);
  pinMode(red_B, OUTPUT);
  pinMode(green_C, OUTPUT);
  pinMode(yellow_C, OUTPUT);
  pinMode(red_C, OUTPUT);
  pinMode(green_D, OUTPUT);
  pinMode(yellow_D, OUTPUT);
  pinMode(red_D, OUTPUT);

  // Attach interrupts for real-time vehicle detection on each sensor
  attachInterrupt(digitalPinToInterrupt(IR1_A), detectTrafficA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IR2_A), detectTrafficA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IR1_B), detectTrafficB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IR2_B), detectTrafficB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IR1_C), detectTrafficC, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IR2_C), detectTrafficC, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IR1_D), detectTrafficD, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IR2_D), detectTrafficD, CHANGE);
}

void loop() {
  controlTraffic();  // Manage traffic lights based on traffic density

  // Power-saving mode if no traffic is detected
  if (allLanesClear()) {
    enterSleep();    // Put the Arduino to sleep to save power
  }

  powerSavingMode(); // Adjust brightness of lights based on traffic conditions
}

// Interrupt Service Routines (ISR) to handle IR sensor changes for each lane
void detectTrafficA() {
  if (digitalRead(IR1_A) == HIGH && digitalRead(IR2_A) == HIGH) {
    trafficDensityA = 2; // High traffic
  } else if (digitalRead(IR1_A) == HIGH || digitalRead(IR2_A) == HIGH) {
    trafficDensityA = 1; // Low traffic
  } else {
    trafficDensityA = 0; // No traffic
  }
}

void detectTrafficB() {
  if (digitalRead(IR1_B) == HIGH && digitalRead(IR2_B) == HIGH) {
    trafficDensityB = 2; // High traffic
  } else if (digitalRead(IR1_B) == HIGH || digitalRead(IR2_B) == HIGH) {
    trafficDensityB = 1; // Low traffic
  } else {
    trafficDensityB = 0; // No traffic
  }
}

void detectTrafficC() {
  if (digitalRead(IR1_C) == HIGH && digitalRead(IR2_C) == HIGH) {
    trafficDensityC = 2; // High traffic
  } else if (digitalRead(IR1_C) == HIGH || digitalRead(IR2_C) == HIGH) {
    trafficDensityC = 1; // Low traffic
  } else {
    trafficDensityC = 0; // No traffic
  }
}

void detectTrafficD() {
  if (digitalRead(IR1_D) == HIGH && digitalRead(IR2_D) == HIGH) {
    trafficDensityD = 2; // High traffic
  } else if (digitalRead(IR1_D) == HIGH || digitalRead(IR2_D) == HIGH) {
    trafficDensityD = 1; // Low traffic
  } else {
    trafficDensityD = 0; // No traffic
  }
}

// Traffic management based on density with round-robin scheduling
void controlTraffic() {
  int lanes[4] = {trafficDensityA, trafficDensityB, trafficDensityC, trafficDensityD};

  // Iterate through lanes in a round-robin fashion
  for (int i = 0; i < 4; i++) {
    currentLane = (currentLane + 1) % 4; // Switch to next lane
    int density = lanes[currentLane];
    
    if (density == 2) {
      setGreenLight(currentLane, HIGH_TRAFFIC_DURATION);  // High traffic
    } else if (density == 1) {
      setGreenLight(currentLane, LOW_TRAFFIC_DURATION);   // Low traffic
    }
  }
}

// Set green light with given duration for selected lane
void setGreenLight(int lane, int duration) {
  int greenPin, yellowPin, redPin;
  
  // Assign pins based on lane
  switch (lane) {
    case 0: greenPin = green_A; yellowPin = yellow_A; redPin = red_A; break;
    case 1: greenPin = green_B; yellowPin = yellow_B; redPin = red_B; break;
    case 2: greenPin = green_C; yellowPin = yellow_C; redPin = red_C; break;
    case 3: greenPin = green_D; yellowPin = yellow_D; redPin = red_D; break;
  }
  
  // Turn on green light
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);
  delay(duration);

  // Turn on yellow light
  digitalWrite(greenPin, LOW);
  digitalWrite(yellowPin, HIGH);
  delay(YELLOW_DURATION);

  // Turn on red light
  digitalWrite(yellowPin, LOW);
  digitalWrite(redPin, HIGH);
}

// Power-saving mode: dim lights using PWM during low traffic
void powerSavingMode() {
  if (lowTrafficDetected()) {
    analogWrite(green_A, 128);  // Reduce brightness by 50%
    analogWrite(green_B, 128);
    analogWrite(green_C, 128);
    analogWrite(green_D, 128);
  } else {
    analogWrite(green_A, 255);  // Full brightness during high traffic
    analogWrite(green_B, 255);
    analogWrite(green_C, 255);
    analogWrite(green_D, 255);
  }
}

// Check if all lanes have no traffic
bool allLanesClear() {
  return (trafficDensityA == 0 && trafficDensityB == 0 && trafficDensityC == 0 && trafficDensityD == 0);
}

// Check if low traffic is detected on any lane
bool lowTrafficDetected() {
  return (trafficDensityA == 1 || trafficDensityB == 1 || trafficDensityC == 1 || trafficDensityD == 1);
}

// Enter sleep mode to save power when no traffic is detected
void enterSleep() {
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  sleep_mode(); // Arduino enters sleep mode
  sleep_disable(); // Arduino wakes up
}
