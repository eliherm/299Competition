#include "QSerial.h"
#include <Servo.h>

#define RANGING_SENSOR_PIN 3
#define RANGE_THRESHOLD 605
#define LTHRESH 940
#define CTHRESH 940
#define RTHRESH 940
#define IRL A2
#define IRC A0
#define IRR A1
#define txpin -1
#define rxpin 11
#define GRIP_SENSE A5
#define GRIP_THRESH 1000
#define GRIP_PIN 8
#define TILT_PIN 9
#define PAN_PIN 10

QSerial myIRserial;
Servo gripServo;
Servo tiltServo;
Servo panServo;

float LeftIR,MiddleIR,RightIR;
int MLeft = 4;
int ELeft = 5;
int ERight = 6;
int MRight = 7;
int leftPin = 0;
int centrePin = 0;
int rightPin = 0;
int start = 1;

void setup() {
    int path[2] = { 1, 2 };

    pinMode(rxpin, INPUT);
    pinMode (A0, INPUT);
    pinMode (A1, INPUT);
    pinMode (A2, INPUT);
    pinMode(MLeft, OUTPUT);
    pinMode(ELeft, OUTPUT);
    pinMode(ELeft, OUTPUT);
    pinMode(MLeft, OUTPUT);

    myIRserial.attach(rxpin, txpin);
    gripServo.attach(GRIP_PIN);
    tiltServo.attach(TILT_PIN);
    panServo.attach(PAN_PIN);
    Serial.begin(9600);
}

void loop() {
    int path[5];
    int pLength;
    startPos();

    while (start) {
     if (1) {
       path[0] = 0;
       path[1] = 0;
       path[2] = 0;
       path[3] = 0;
       path[4] = 0;
       pLength = 5;
       start = 0;
     }
    }

    int traveled = 0;
    int LeftSpeed = 100;
    int RightSpeed = 100;
    while (traveled < pLength) {
        LeftSpeed = 100;
        RightSpeed = 100;
        leftPin = analogRead(IRL);
        centrePin = analogRead(IRC);
        rightPin = analogRead(IRR);

        // If at an intersection perform task based on path
        if (checkIntersection()) {
            pathFollow(path, traveled);
            traveled++;
        }
        if ((leftPin > LTHRESH) && (centrePin < CTHRESH)) {
            RightSpeed += 20;
            LeftSpeed -= 20;
        } else if ((rightPin > RTHRESH) && (centrePin < CTHRESH)) {
            LeftSpeed += 20;
            RightSpeed -=20;
        }

        analogWrite(ELeft, LeftSpeed);
        analogWrite(ERight, RightSpeed);
        digitalWrite(MLeft, 1);
        digitalWrite(MRight, 1);
        startPos();

        if (rangingStop()) {
            stopRobot();
            pickUp();
            turnAround();
            delay(5000);
        }
    }
}

void pathFollow(int path[], int intersection) {
    Serial.println(path[intersection]);
    if (path[intersection] == 0) {
        delay(500);
    } else if(path[intersection] == 1) {
        turnLeft();
    } else if(path[intersection] == 2) {
        turnRight();
    } else if (path[intersection] == 3) {
        // stopRobot();
    }
}

void turnLeft() {
    analogWrite(ELeft, 128);
    analogWrite(ERight, 128);
    digitalWrite(MLeft, 1);
    digitalWrite(MRight, 1);
    delay(500);

    digitalWrite(MLeft, 0);
    digitalWrite(MRight, 1);
    delay(500);

    leftPin = analogRead(IRL);
    centrePin = analogRead(IRC);
    rightPin = analogRead(IRR);

    while (centrePin <= CTHRESH) {
        leftPin = analogRead(IRL);
        centrePin = analogRead(IRC);
        rightPin = analogRead(IRR);
        delay(1);
    }

    analogWrite(ELeft, 0);
    analogWrite(ERight, 0);
}

void turnRight() {
    analogWrite(ELeft, 128);
    analogWrite(ERight, 128);
    digitalWrite(MLeft, 1);
    digitalWrite(MRight, 1);
    delay(500);

    digitalWrite(MLeft, 1);
    digitalWrite(MRight, 0);
    delay(500);

    leftPin = analogRead(IRL);
    centrePin = analogRead(IRC);
    rightPin = analogRead(IRR);

    while (centrePin <= CTHRESH) {
        leftPin = analogRead(IRL);
        centrePin = analogRead(IRC);
        rightPin = analogRead(IRR);
        delay(1);
    }

    analogWrite(ELeft, 0);
    analogWrite(ERight, 0);
}

int checkIntersection() {
    leftPin = analogRead(IRL);
    centrePin = analogRead(IRC);
    rightPin = analogRead(IRR);

    if ((leftPin >= LTHRESH) && (centrePin >= CTHRESH) && (rightPin >= RTHRESH)) {
        Serial.println("intersection");
        return 1;
    } else {
        return 0;
    }
}

char beaconReceiver() {
    char val;
    while (1) {
        val = myIRserial.receive(200);
        Serial.println(val);

        if(val == '0') {
            return val;
        } else if (val == '1') {
            return val;
        } else if (val == '2') {
            return val;
        }
        delay(100);
    }
}

void turnAround() {
    digitalWrite(MLeft, 0);
    digitalWrite(MRight, 0);
    analogWrite(ELeft, 128);
    analogWrite(ERight, 128);
    delay(2000);

    digitalWrite(MLeft, 1);
    digitalWrite(MRight, 0);
    analogWrite(ELeft, 128);
    analogWrite(ERight, 128);
    delay(1000);

    leftPin = analogRead(IRL);
    centrePin = analogRead(IRC);
    rightPin = analogRead(IRR);

    while (centrePin <= CTHRESH) {
        leftPin = analogRead(IRL);
        centrePin = analogRead(IRC);
        rightPin = analogRead(IRR);
        delay(1);
    }
    analogWrite(ELeft, 0);
    analogWrite(ERight, 0);
}

void startPos() {
    //starting position of the arm, pan - 90, vertical - 160, fully open - 40
    panServo.write(90);
    tiltServo.write(160);
    gripServo.write(40);
    delay(500);
}

void walkingPos() {
    Serial.println("walking around now");
    panServo.write(90);
    tiltServo.write(160);
}

int rangingStop() {
    int range = analogRead(RANGING_SENSOR_PIN);
    Serial.println(range);
    if (range > RANGE_THRESHOLD) {
        return 1;
    } else {
        return 0;
    }
}

void stopRobot() {
    analogWrite(ELeft, 0);
    analogWrite(ERight, 0);
    digitalWrite(MLeft, 1);
    digitalWrite(MRight, 1);
}

void closeGripper() {
    int finalGrip;
    int press = analogRead(GRIP_SENSE);

    for (int i = 40; i < 180; i++) {
        press = analogRead(GRIP_SENSE);
        Serial.println(press);
        gripServo.write(i);
        finalGrip = i;
        delay(50);
    }

    // if closing on something
    if (press >= GRIP_THRESH) {
        Serial.println("closed");
        int angle = gripServo.read(); // angle gripper is at
        Serial.println(angle);
    }
    delay(1000);
}

void dropOff() { // have ball, bringing home
    Serial.println("Dropping off");
    tiltServo.write(75);
    delay(1000); // this delay is necessary
    gripServo.write(40);
    walkingPos();
}

void pickUp() {
    Serial.println("Picking up");
    startPos();
    tiltServo.write(75); // horizontal = 75 for us
    closeGripper();
    walkingPos();
}
