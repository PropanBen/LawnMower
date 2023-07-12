#include <Ps3Controller.h>
// Mac Adresse ESP B8:D6:1A:34:6A:06

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

#define PMWC 27
#define Cin1 25
#define Cin2 26

#define PMWD 27
#define Din1 12
#define Din2 14

#define PMWE 25
#define Ein1 32
#define Ein2 32



#define MotorSpeed 255
#define MowSpeed 100

bool motorCForward = false;


void setupMotorControl() {
  pinMode(PMWA, OUTPUT);
  pinMode(Ain1, OUTPUT);
  pinMode(Ain2, OUTPUT);
  
  pinMode(PMWB, OUTPUT);
  pinMode(Bin1, OUTPUT);
  pinMode(Bin2, OUTPUT);
  
  pinMode(PMWC, OUTPUT);
  pinMode(Cin1, OUTPUT);
  pinMode(Cin2, OUTPUT);

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
  analogWrite(PMWB,MotorSpeed);
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
  digitalWrite(Cin1, HIGH);
  digitalWrite(Cin2, LOW);
  analogWrite(PMWC, MowSpeed);
}

// Function to move motor C backward
void moveMotorCBackward() {
  digitalWrite(Cin1, LOW);
  digitalWrite(Cin2, HIGH);
  analogWrite(PMWC, MowSpeed);
}

// Function to brake motor C
void brakeMotorC() {
  digitalWrite(Cin1, HIGH);
  digitalWrite(Cin2, HIGH);
  analogWrite(PMWC, 0);
}

// Function to move motor D forward
void moveMotorDForward() {
  digitalWrite(Cin1, HIGH);
  digitalWrite(Cin2, LOW);
  analogWrite(PMWC, MowSpeed);
}

// Function to move motor D backward
void moveMotorDBackward() {
  digitalWrite(Din1, LOW);
  digitalWrite(Din2, HIGH);
  analogWrite(PMWD, MowSpeed);
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
  analogWrite(PMWE, MowSpeed);
}

// Function to move motor E backward
void moveMotorEBackward() {
  digitalWrite(Ein1, LOW);
  digitalWrite(Ein2, HIGH);
  analogWrite(PMWE, MowSpeed);
}

// Function to brake motor E
void brakeMotorE() {
  digitalWrite(Ein1, HIGH);
  digitalWrite(Ein2, HIGH);
  analogWrite(PMWE, 0);
}

// Function to toggle motor C between forward motion and braking
void toggleMotorCForward() {
  if (motorCForward) {
    moveMotorCForward();
    Serial.println("Schneidmotor an");
    motorCForward = false;
  } else {
    brakeMotorC();
    Serial.println("Schneidmotor aus");
    motorCForward = true;
  }
}

void onConnect(){
    Serial.println("Connected.");
}

void notify()
{

 
   //Circle Button Acclerate
   if( abs(Ps3.event.analog_changed.button.circle) ){
       moveMotorAForward();
       moveMotorBForward();
       moveMotorDForward();
       moveMotorEForward();
   }

  if( Ps3.event.button_up.circle )
  {
       brakeMotorA();
       brakeMotorB();
       brakeMotorD();
       brakeMotorE();
  }

  // Cross Button Reverse
  if( abs(Ps3.event.analog_changed.button.cross) ){

       moveMotorABackward();
       moveMotorBBackward();
       moveMotorDBackward();
       moveMotorEBackward();
   }

  if( Ps3.event.button_up.cross )
  {
       brakeMotorA();
       brakeMotorB();
       brakeMotorD();
       brakeMotorE();
  }

  //Start Mower
  if( Ps3.event.button_up.triangle ){
       toggleMotorCForward();
   }

// Motor Schema
//     A ----- B
//       | C |     
//     D ----- E

 

      //---------------- Analog stick value events ---------------
   if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 10 ){

    if (Ps3.data.analog.stick.lx > 100) {
        moveMotorAForward();
        moveMotorDForward();
        moveMotorBBackward();
        moveMotorEBackward();

    }
    else if (Ps3.data.analog.stick.lx < -100) {
        moveMotorBForward();
        moveMotorEForward();
        moveMotorABackward();
        moveMotorDBackward();
    }
    else 
    {
       brakeMotorA();
       brakeMotorB();
       brakeMotorD();
       brakeMotorE();
    }



       Serial.println(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.println(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
    
   }
}





void setup() {
   
    setupMotorControl();
    Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("B8:D6:1A:34:6A:06");

}




void loop() {
  // put your main code here, to run repeatedly:

  Serial.begin(115200);

}
