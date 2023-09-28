#include <Servo.h>
Servo myservo;
 
int pos = 0;    
boolean fire = false;
int direction = 0; //0->straight, 1->right, 2->left
float duration, distance;

#define threshold 400.0
/*-------defining Inputs------*/
#define Left_S 9      // left sensor
#define Right_S 10      // right sensor
#define Forward_S 8 //forward sensor
 
/*-------defining Outputs------*/
#define LM1 3       // left motor
#define LM2 2       // left motor
#define RM1 5       // right motor
#define RM2 4       // right motor
#define pump 6
#define trigPin 13
#define echoPin 12
#define warning 7

const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
 
void setup()
{
  pinMode(Left_S, INPUT);
  pinMode(Right_S, INPUT);
  pinMode(Forward_S, INPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(warning, OUTPUT);
  digitalWrite(warning, LOW);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  

  Serial.begin(9600); // Starting Serial Communication
 
  myservo.attach(11);
  myservo.write(90); 
}

void move(){
  delay(200);

  digitalWrite(trigPin, HIGH);     // send waves for 10 us
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // receive reflected waves
  distance = duration * 0.017;   // convert to distance
  delay(10);

  //show distance
  Serial.println(distance);

  if (distance < threshold){
    digitalWrite(warning, HIGH);

    digitalWrite(LM1, HIGH); //stop
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
    delay(200);
    digitalWrite(LM1, LOW);  //movebackward
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH);
    delay(200);
    digitalWrite(LM1, HIGH); //stop
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
    delay(200);  
    digitalWrite(LM1, HIGH); //turn right
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    delay(200);

    digitalWrite(warning, LOW);
  }
}
 
void put_off_fire()
{
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
    
    digitalWrite(pump, HIGH);

    for (pos = 50; pos <= 130; pos += 1) { 
      myservo.write(pos); 
      delay(10);  
    }
    for (pos = 130; pos >= 50; pos -= 1) { 
      myservo.write(pos); 
      delay(10);
    }
  
    digitalWrite(pump,LOW);
    delay(100);
    myservo.write(90);
  
    // fire=false;
}
 
void loop()
{
   myservo.write(90); //Sweep_Servo();  
 
    if (digitalRead(Left_S) ==1 && digitalRead(Right_S)==1 && digitalRead(Forward_S) ==1) //If Fire not detected all sensors are zero
    {
    //Do not move the robot
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
    fire = false;
    }
    
    else if (digitalRead(Forward_S) ==0) //If Fire is straight ahead
    {
    //Move the robot forward
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    direction = 0;
    fire = true;
    }
    
    else if (digitalRead(Left_S) ==0) //If Fire is to the left
    {
    //Move the robot left
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
    direction = 2;
    fire = true;
    }
    
    else if (digitalRead(Right_S) ==0) //If Fire is to the right
    {
    //Move the robot right
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    direction = 1;
    fire = true;
    }
    
    if(fire == true){
      move();
      put_off_fire();
    }
    delay(500);
}
