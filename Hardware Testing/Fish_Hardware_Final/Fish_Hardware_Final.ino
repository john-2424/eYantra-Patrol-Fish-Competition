#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
Servo servo_1;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  servo_1.attach(3);
}
void loop() {
  int velocity;
  int i;
  if (radio.available()) {
    char reciever[50];
    radio.read(&reciever, sizeof(reciever));
    String command = String(reciever);
    //Serial.println(command);
    String vel = command.substring(0,1);
    String angle = command.substring(1);
    velocity = vel.toInt();
    i = angle.toInt();
    Serial.print("velocity: ");
    Serial.println(velocity);
    Serial.print("angle: ");
    Serial.println(i);
  }
  
  switch(velocity)
  {
    case 1: low(i); break;
    case 2: medium(i); break;
    case 3: high(i); break;
    default : break;
  }
  
}

void low(int i)
{
    for(int ang = 60 + i; ang <= 120 + i; ang = ang+2)
  {
     servo_1.write(ang);
     delay(30);
  }
  for(int ang = 120 + i; ang >= 60 + i; ang = ang-2)
  {
     servo_1.write(ang);
     delay(30);
  }
}

void medium(int i)
{
    for(int ang = 60 + i; ang <= 120 + i; ang = ang+2)
  {
     servo_1.write(ang);
     delay(10);
  }
  for(int ang = 120 + i; ang >= 60 + i; ang = ang-2)
  {
     servo_1.write(ang);
     delay(10);
  }
}

void high(int i)
{
  servo_1.write(60+i);
  delay(180);
  servo_1.write(120+i);
  delay(180);
}

