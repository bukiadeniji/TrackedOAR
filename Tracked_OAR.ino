//Compilation of robotics programs for use on Instructables.  Thanks for everyone that contributed!

//Updated by Buki Adeniji, buki . adeniji @ yahoo . com on September 20th, 2015

//Updates added to the original code from dcordova318 on Instructables.com - thanks for a great description 
//- http://www.instructables.com/id/Tracked-Arduino-Robot/ 

#include <Servo.h> 

int lmF=8; //define OUT1 interface
int lmB=11; //define OUT2 interface 
int speedPinL=9; //enable motor A
int rmF=12; //define OUT3 interface 
int rmB=13; //define OUT4 interface 
int speedPinR=10; //enable motor B

int mSpeed = 250;  // Motor speed: (255 MAX)

int trigPin=7; //ultrasonic trigger 
int echoPin=6; //ultrasonic echo 

int Fdist = 0;      // front distance
int Rdist = 0;      // right distance
int Ldist = 0;      // left distance

int directionn = 0;   //  determine direction of turn

Servo myservo;        // myservo

//Servo Motor alignment - Align servo motor
int AngleFront = 110; // Set servo facing directly ahead
int AngleRight = 65;  // Set servo facing right
int AngleLeft = 155;  // Set servo facing left

int delay_time = 250; // stable steering servo motor

int Fgo = 8;
int Rgo = 6;
int Lgo = 4;
int Bgo = 2;


void setup() 
{
  Serial.begin (115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  pinMode(lmF,OUTPUT);
  pinMode(lmB,OUTPUT);

  pinMode(rmF,OUTPUT);
  pinMode(rmB,OUTPUT);

  pinMode(speedPinL,OUTPUT);
  pinMode(speedPinR,OUTPUT);
  
  myservo.attach(5);    // Define the servo motor output pin5 (PWM)
}

void back(int a)
{
  digitalWrite(speedPinL,HIGH);
  digitalWrite(speedPinR,HIGH);
  digitalWrite(rmB,HIGH);
  digitalWrite(rmF,LOW);
  digitalWrite(lmB,HIGH);
  digitalWrite(lmF,LOW);
  delay(a * 100);
}

void left(int b)
{
  digitalWrite(speedPinL,LOW);
  digitalWrite(speedPinR,HIGH);
  digitalWrite(rmB,HIGH);
  digitalWrite(rmF,LOW);
  digitalWrite(lmB,HIGH);
  digitalWrite(lmF,LOW);
  delay(b * 100);
}
void right(int c)
{
  digitalWrite(speedPinL,HIGH);
  digitalWrite(speedPinR,LOW);
  digitalWrite(rmB,HIGH);
  digitalWrite(rmF,LOW);
  digitalWrite(lmB,HIGH);
  digitalWrite(lmF,LOW);
  delay(c * 100);
}
void advance(int d)
{
  digitalWrite(speedPinL,HIGH);
  digitalWrite(speedPinR,HIGH);
  digitalWrite(rmB,LOW);
  digitalWrite(rmF,HIGH);
  digitalWrite(lmB,LOW);
  digitalWrite(lmF,HIGH);
  delay(d * 100);
}
void halt(int e)
{
  digitalWrite(rmB,LOW);
  digitalWrite(rmF,LOW);
  digitalWrite(lmB,LOW);
  digitalWrite(lmF,LOW);
  delay(e * 100);
}

void detection()
{
  int delay_time = 250; //waiting for servo motor to be stable
  check_dist_Front();  //check distance of object in front of robot
  
  if(Fdist < 15) // if distance to object in front is less than 15cm; stop then reverse
  {
    halt(1);
    back(2);
  }
  
  if(Fdist < 30)
  {
    halt(1);
    check_dist_Left();  //measure left 
    delay(delay_time);
    check_dist_Right(); //measure right
    delay(delay_time);
    myservo.write(AngleFront);
    
    if(Ldist > Rdist)  //compare distance of right and left from objects
    {
      directionn = Lgo; // turn left
    }
    
    if(Ldist <= Rdist)  //compare distance of right and left from objects
    {
      directionn = Rgo; // turn right
    }    
    
    if(Ldist < 15 && Rdist < 15)
    {
      directionn = Bgo;
    }
  }
  else
  {
    directionn = Fgo;  // keep forward
  }
}

void check_dist_Front()
{
  myservo.write(AngleFront);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);    
  digitalWrite(trigPin, LOW);
    
  float duration = pulseIn(echoPin, HIGH);
  float Fdistance = (duration/2) / 29.1;
    
  Serial.print("F distance:");
  Serial.println(Fdistance);
  Fdist = Fdistance;
}
  
void check_dist_Left()
{
  myservo.write(AngleLeft);
  delay(delay_time);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);    
  digitalWrite(trigPin, LOW);
    
  float duration = pulseIn(echoPin, HIGH);
  float Ldistance = (duration/2) / 29.1;
    
  Serial.print("L distance:");
  Serial.println(Ldistance);
  Ldist = Ldistance;
}
void check_dist_Right()
{
  myservo.write(AngleRight);
  delay(delay_time);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);    
  digitalWrite(trigPin, LOW);
    
  float duration = pulseIn(echoPin, HIGH);
  float Rdistance = (duration/2) / 29.1;
    
  Serial.print("R distance:");
  Serial.println(Rdistance);
  Rdist = Rdistance;
}
void testSonic() //function to test the ultrasonic distance sensor
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);    
  digitalWrite(trigPin, LOW);
    
  float duration = pulseIn(echoPin, HIGH);
  float Fdistance = (duration/2) / 29.1;
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  
  if(Fdistance < 10)
  {
    Serial.println("F distance LOW");
    delayMicroseconds(10);
  }
  if (Fdistance > 10 && Fdistance < 20)
  {
    Serial.println("F distance MED");
    delayMicroseconds(10);
  }
  if (Fdistance > 20 && Fdistance < 40)
  {
    Serial.println("F distance HIGH");
    delayMicroseconds(10);
  }
  Serial.print("F distance:");
  Serial.println(Fdistance);
  Fdist = Fdistance;
}

  
void loop()
{
  myservo.write(AngleFront);  //
  detection();        // Range detection and direction finder
  //testSonic();         // Ultrasonic Test function  
  
  if(directionn == 2)  //             
  {
    back(8);                    //  
    left(2);                   //
    Serial.print(" Reverse ");   //
  }
  if(directionn == 6)
  {
    back(1);
    left(6);
    Serial.print(" Right ");
  }
  if(directionn == 4)
  {
    back(1);
    right(6);
    Serial.print(" Left ");
  }
  if(directionn == 8)
  {
    advance(1);
    Serial.println(" Advance ");
  }
}
