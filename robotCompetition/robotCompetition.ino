#define  LTHRESH 875
#define  CTHRESH 875
#define  RTHRESH 875
float LeftIR,MiddleIR,RightIR;
int MLeft = 4;
int ELeft = 5;
int ERight = 6; 
int MRight = 7;

void setup() {
  int path[2] = {1,2};
  pinMode (A0, INPUT);
  pinMode (A1, INPUT);
  pinMode (A2, INPUT);
  pinMode(MLeft, OUTPUT);
  pinMode(ELeft, OUTPUT);
  pinMode(ELeft, OUTPUT);
  pinMode(MLeft, OUTPUT);
  
}

void loop() {
  int pLength = 3;
  int path[3] = {1,2,1}; 
  int traveled = 0;
  int checkIntersection = 1;
  int LeftSpeed = 128;
  int RightSpeed = 128;
  while(traveled < pLength){
    MiddleIR = analogRead(A1);
    RightIR = analogRead(A0);
    LeftIR = analogRead(A2);
    //If at an intersection perform task based on path
    if(checkIntersection){
      pathFollow(path, traveled);
      traveled++;
    }
    analogWrite(ELeft, LeftSpeed);
    analogWrite(ERight, RightSpeed);
    digitalWrite(MLeft, 1);
    digitalWrite(MRight, 1);
    delay(2000);
  }
}

void pathFollow(int path[], int intersection){
  if(path[intersection] == 1){
    turnLeft(90);
  }else if(path[intersection] == 2){
    turnRight(90);
  }
}

void turnLeft(int angle){
  analogWrite(ELeft, 128);
  analogWrite(ERight, 128);
  digitalWrite(MLeft, 0);
  digitalWrite(MRight, 1);
  if(angle == 90){
    delay(750);
  }else if(angle == 180){
    delay(1425);
  }
  analogWrite(ELeft, 0);
  analogWrite(ERight, 0);
}

void turnRight(int angle){
  analogWrite(ELeft, 128);
  analogWrite(ERight, 128);
  digitalWrite(MLeft, 1);
  digitalWrite(MRight, 0);
  if(angle == 90){
    delay(750);
  }else if(angle == 180){
    delay(1425);
  }
  analogWrite(ELeft, 0);
  analogWrite(ERight, 0);
}
