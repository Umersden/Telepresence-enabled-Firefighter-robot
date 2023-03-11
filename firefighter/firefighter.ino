#include <dht.h>
#define dataPin 8 // Defines pin number to which the sensor is connected
dht DHT;
#include <Servo.h>
Servo myservo; 
int pos = 0;    // variable to store the servo position
#define Pumppin 7
#define leftIR 40
#define leftIR2 39
#define MiddleIR 38
#define RightIR2 37
#define RightIR 36

int motor1Pin1 = 42; 
int motor1Pin2 = 43; 
int motor2Pin1 = 44; 
int motor2Pin2 = 45; 
int state;
int enable = 0;
int temp;
int hum;
int distance;
int timer = 0;
boolean fire = false;


void put_off_fire()
{
    delay (500);
 
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    
 ext();
  
  fire=false;
}
void ext()
{
  
  digitalWrite(Pumppin,HIGH);
  delay(100);
  for (pos = 90; pos <= 150; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 150; pos >= 30; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  myservo.write(90);
  digitalWrite(Pumppin,LOW);
}


void forword(){
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
}

void backword(){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}

void turnLeft(){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
}

void turnRight(){
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
}

void Stop(){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    digitalWrite(Pumppin,LOW);
}

void setup() {
Serial.begin(38400);
Serial3.begin(38400);

pinMode(leftIR, INPUT);
pinMode(MiddleIR, INPUT);
pinMode(RightIR, INPUT);

pinMode(motor1Pin1, OUTPUT);
pinMode(motor1Pin2, OUTPUT);
pinMode(motor2Pin1, OUTPUT);
pinMode(motor2Pin2, OUTPUT);
pinMode(Pumppin,OUTPUT);

  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);   

  myservo.attach(10);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial3.available() > 0){     
      state = Serial3.read(); 
      //Serial.println(state);

      if(state==7)
      {enable=1;}
      if(state==8)
      {enable=0;}
  }
   if(enable==1)
   {
    
    // if the state is '1' the DC motor will go forward
   if (state == 1){forword();Serial.println("Go Forward!");}

   // if the state is '2' the motor will Reverse
   else if (state == 2){backword();Serial.println("Reverse!");}
    
   // if the state is '3' the motor will turn left
   else if (state == 3){turnLeft();Serial.println("Turn LEFT");}
    
   // if the state is '4' the motor will turn right
   else if (state == 4){turnRight();Serial.println("Turn RIGHT");}
    
   // if the state is '5' the motor will Stop
   else if (state == 5) {Stop();Serial.println("STOP!");}  

   // if the state is '6' the pump will Start
   else if (state == 6) {ext();Serial.println("Water");}  
   
timer = timer+1;

if(timer>400){
 DHT.read11(dataPin);
 hum = DHT.humidity;
 temp = DHT.temperature;
 
 Serial3.print(temp);        //send distance to MIT App
 Serial3.print(";");
 Serial3.print(hum);         //send distance to MIT App
 Serial3.print(";");
 Serial3.print("1"); //send distance to MIT App 
 Serial3.println(";");    
 timer=0; }
 delay(1);
   }

else if(enable==0)
{distance = analogRead(A1);

  while(digitalRead(RightIR)==1 ||digitalRead(RightIR2)==1)
{
    turnRight();
    delay(100);

}
while(digitalRead(MiddleIR)==1)
{fire = true;
  forword();
  delay(300);
  Serial.print("Middle");
}


while(digitalRead(leftIR)==1 || digitalRead(leftIR2)==1)
{
 turnLeft();
    delay(100);

  Serial.print("Left");
  
}
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  Serial.print("Middle");
  Serial.println();
  } 
  while (fire == true)
     {
      put_off_fire();
     }  
}
