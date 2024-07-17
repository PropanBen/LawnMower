#include <Bluepad32.h>

// Mac Adresse ESP B0:A7:32:D7:80:B8

//Get Mac Adress of ESP32
//include <WiFi.h>
//Serial.println(WiFi.macAddress());

// Motor Schema
//     A ----- B
//       | C |     
//     D ----- E

#define PMWA 22
#define Ain1 21
#define Ain2 19

#define PMWB 18
#define Bin1 5
#define Bin2 4

/*
#define PMWC 23 //27 
#define Cin1 16 // 34
#define Cin2 17 //35
*/

#define RPWM_PIN 23 
#define LPWM_PIN 16 
#define R_EN_PIN 17 
#define L_EN_PIN 2 

#define PMWD 26
#define Din1 12
#define Din2 14

#define PMWE 25
#define Ein1 32
#define Ein2 33

#define MowSpeed 100
#define MotorSpeed 255
bool motorCForward = false;
bool motorCStateChanged = false;

void setupMotorControl() {
  pinMode(PMWA, OUTPUT);
  pinMode(Ain1, OUTPUT);
  pinMode(Ain2, OUTPUT);
  
  pinMode(PMWB, OUTPUT);
  pinMode(Bin1, OUTPUT);
  pinMode(Bin2, OUTPUT);
  
  /*
  pinMode(PMWC, OUTPUT);
  pinMode(Cin1, OUTPUT);
  pinMode(Cin2, OUTPUT);
 */
  pinMode(RPWM_PIN, OUTPUT);
  pinMode(LPWM_PIN, OUTPUT);
  pinMode(R_EN_PIN, OUTPUT);
  pinMode(L_EN_PIN, OUTPUT);

  digitalWrite(R_EN_PIN, HIGH);
  digitalWrite(L_EN_PIN, HIGH);

  pinMode(PMWD, OUTPUT);
  pinMode(Din1, OUTPUT);
  pinMode(Din2, OUTPUT);

  pinMode(PMWE, OUTPUT);
  pinMode(Ein1, OUTPUT);
  pinMode(Ein2, OUTPUT);
}

// Function to move motor A forward
void moveMotorAForward() {
  digitalWrite(Ain1, HIGH);
  digitalWrite(Ain2, LOW);
  analogWrite(PMWA, MotorSpeed);
}

// Function to move motor A backward
void moveMotorABackward() {
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, HIGH);
  analogWrite(PMWA, MotorSpeed);
}

// Function to brake motor A
void brakeMotorA() {
  digitalWrite(Ain1, HIGH);
  digitalWrite(Ain2, HIGH);
  analogWrite(PMWA, 0);
}

// Function to move motor B forward
void moveMotorBForward() {
  digitalWrite(Bin1, HIGH);
  digitalWrite(Bin2, LOW);
  analogWrite(PMWB, MotorSpeed);
}

// Function to move motor B backward
void moveMotorBBackward() {
  digitalWrite(Bin1, LOW);
  digitalWrite(Bin2, HIGH);
  analogWrite(PMWB, MotorSpeed);
}

// Function to brake motor B
void brakeMotorB() {
  digitalWrite(Bin1, HIGH);
  digitalWrite(Bin2, HIGH);
  analogWrite(PMWB, 0);
}

// Function to move motor C forward
void moveMotorCForward() {
  for (int speed = 0; speed <= MowSpeed; speed++) {
    analogWrite(RPWM_PIN, speed);
    analogWrite(LPWM_PIN, 0);
    delay(100); 
  }
}

// Function to move motor C backward
void moveMotorCBackward() {
  for (int speed = 0; speed <= MowSpeed; speed++) {
    analogWrite(RPWM_PIN, 0);
    analogWrite(LPWM_PIN, speed);
    delay(100); 
  }
}

// Function to brake motor C
void brakeMotorC() {
  analogWrite(RPWM_PIN, 0);
  analogWrite(LPWM_PIN, 0);
}

// Function to move motor D forward
void moveMotorDForward() {
  digitalWrite(Din1, HIGH);
  digitalWrite(Din2, LOW);
  analogWrite(PMWD, MotorSpeed);
}

// Function to move motor D backward
void moveMotorDBackward() {
  digitalWrite(Din1, LOW);
  digitalWrite(Din2, HIGH);
  analogWrite(PMWD, MotorSpeed);
}

// Function to brake motor D
void brakeMotorD() {
  digitalWrite(Din1, HIGH);
  digitalWrite(Din2, HIGH);
  analogWrite(PMWD, 0);
}

// Function to move motor E forward
void moveMotorEForward() {
  digitalWrite(Ein1, HIGH);
  digitalWrite(Ein2, LOW);
  analogWrite(PMWE, MotorSpeed);
}

// Function to move motor E backward
void moveMotorEBackward() {
  digitalWrite(Ein1, LOW);
  digitalWrite(Ein2, HIGH);
  analogWrite(PMWE, MotorSpeed);
}

// Function to brake motor E
void brakeMotorE() {
  digitalWrite(Ein1, HIGH);
  digitalWrite(Ein2, HIGH);
  analogWrite(PMWE, 0);
}

// Function to toggle motor C between forward motion and braking
void toggleMotorC() {
  if (motorCForward) {
    brakeMotorC();
    Serial.println("Schneidmotor aus");
  } else {
    moveMotorCForward();
    Serial.println("Schneidmotor an");
  }
  motorCForward = !motorCForward;
  motorCStateChanged = false;
}

ControllerPtr myController = nullptr;

// This callback gets called any time a new gamepad is connected.
void onConnectedController(ControllerPtr ctl) {
  if (myController == nullptr) {
    Serial.println("CALLBACK: Controller is connected");
    ControllerProperties properties = ctl->getProperties();
    Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
    myController = ctl;
  } else {
    Serial.println("CALLBACK: Controller connected, but already in use");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  if (myController == ctl) {
    Serial.println("CALLBACK: Controller disconnected");
    myController = nullptr;
  } else {
    Serial.println("CALLBACK: Controller disconnected, but not the active one");
  }
}

void processGamepad(ControllerPtr ctl) {
  bool movingForwardOrBackward = false;
  bool turning = false;

  if (ctl->a()) {
    movingForwardOrBackward = true;
    Serial.println("Button X");
    moveMotorABackward();
    moveMotorBBackward();
    moveMotorDBackward();
    moveMotorEBackward();
  }
  if (ctl->b()) {
    movingForwardOrBackward = true;
    moveMotorAForward();
    moveMotorBForward();
    moveMotorDForward();
    moveMotorEForward();
  }
  if (ctl->y() && !motorCStateChanged) {
    motorCStateChanged = true;
    toggleMotorC();
  }
  if (ctl->x()) {
    Serial.println("Button Viereck");
    brakeMotorA();
    brakeMotorB();
    brakeMotorD();
    brakeMotorE();
  }
  
  int16_t axisX = ctl->axisX();
  Serial.printf("Left Axis X: %d\n", axisX);

  if (axisX > 500) {
    turning = true;
    Serial.println("Rechts");
    moveMotorAForward();
    moveMotorDForward();
    moveMotorBBackward();
    moveMotorEBackward();
 
  } else if (axisX < -500) {
    turning = true;
    Serial.println("Links");

    moveMotorBForward();
    moveMotorEForward();
    moveMotorABackward();
    moveMotorDBackward();
  }


  if (!movingForwardOrBackward && !turning) {
    brakeMotorA();
    brakeMotorB();
    brakeMotorD();
    brakeMotorE();
  }
}

void setup() {
  Serial.begin(115200);
  setupMotorControl(); 
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);
}

void loop() {
  if (BP32.update() && myController && myController->isConnected() && myController->hasData() && myController->isGamepad()) {
    processGamepad(myController);
  }
  delay(150);
}
