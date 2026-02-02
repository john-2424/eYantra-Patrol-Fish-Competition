/*
 * Team Id:          5066
 * Author List :     Srinivas A, Aikansh Prakash Shenoy, 
 *                   Shrikrishna Bhagirath Rajule, Md Tauseeq Zuman
 * File name:        PatrolFish
 * Theme:            Patrol Fish(PF)
 * Functions:        setup, loop, low, medium, high 
 * Global variables: radio, address, Tail_D, Tail_U, Head
 */
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//radio: creates an object RF24 of the library RF24, input arguments are CE, CSN pin number  
RF24 radio(7, 8);

//address: represents the address or pipe through which communication happens
const byte address[6] = "00001";
//Tail_D: will represent the tail servo(bottom) pwm pin number
Servo Tail_D;
//Tail_U: will represent the tail servo(top) pwm pin number
Servo Tail_U;
//Head: will represent the head servo pwm pin number
Servo Head;

void setup() {
/*
 * Function Name: setup
 * Input:         None
 * Output:        None
 * Logic:         Initial setup of all the devices connected, runs only once,
 *                function is called by the sketch
 */
  //Setting up the NRF24L01
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //Setting up the Head, Tail mechanisms servo and Serial Monitor
  Tail_D.attach(6);
  Tail_U.attach(5);
  Head.attach(3);
  Serial.begin(9600);
}

void loop() {
/*
 * Function Name: loop
 * Input:         None
 * Output:        None
 * Logic:         recieve the command from the joystic and control the servos 
 *                accordingly, runs repeatedly and contains the main code,
 *                function is called by the sketch
 */

  //velocity: will contain the speed mode, ranges from 1 to 3
  int velocity;
  //i: contains the value of offset for servo sweep, ranges from -30 to +30
  int i;
  
  if (radio.available()) { //checking if NRF24L01 is ready to recieve the data
    
    //reciever: buffer for recieving the data
    char reciever[50];
    radio.read(&reciever, sizeof(reciever)); //Recieving the Command from Joystick
    
    //Processing the recieved command
    
    //command: buffer for the recieved command in String format
    String command = String(reciever);
    
    //vel: contains the speed mode in String format
    String vel = command.substring(0,1);
    
    //angle: contains the value of offset for servo sweep in string format
    String angle = command.substring(1);
    
    velocity = vel.toInt();   //converting the speed mode from string to integer format 
    i = angle.toInt();        //converting the offset for servo sweep from string to integer format
    
    //displaying the speed mode and the offset for servo sweep in serial monitor
    Serial.print("velocity: ");
    Serial.println(velocity);
    Serial.print("angle: ");
    Serial.println(i);
  }

  //Switch case for controlling speed and direction of patrol fish
  switch(velocity)
  {
    case 1: low(i); break;    //Calls the function low in case of velocity value 1
    case 2: medium(i); break; //Calls the function medium in case of velocity value 2
    case 3: high(i); break;   //Calls the function high in case of velocity value 3
    default : break;
  }
  
}

void low(int i)
{
    /*
     * Function Name:  low
     * Input:          'i' the value of offset for servo sweep
     * Output:         None
     * Logic:          contains two 'for' loops which help sweep the servo from left to right and 
     *                 again from right to left with the delay of 30ms(low speed) within each iteration of 
     *                 'for' loop, the offset for servo sweep is added to the initial and the final 
     *                 position of the servo, the head servo will also sweep through 90-i degree.
     * Example Call:   low(10);
     */

    //ang : will iterate from 60+i to 120+i in steps of 2
    for(int ang = 60 + i; ang <= 120 + i; ang = ang+2)
    {
      Tail_D.write(ang);
      Tail_U.write(180-ang);
      Head.write(90-i);
      delay(30);
    }

    //ang : will iterate from 120+i to 60+i in steps of 2
    for(int ang = 120 + i; ang >= 60 + i; ang = ang-2)
    {
      Tail_D.write(ang);
      Tail_U.write(180-ang);
      Head.write(90-i);
      delay(30);
    }
}

void medium(int i)
{
    /*
     * Function Name:  medium
     * Input:          'i' the value of offset for servo sweep
     * Output:         None
     * Logic:          contains two 'for' loops which help sweep the servo from left to right and 
     *                 again from right to left with the delay of 10ms(medium speed) within each iteration of 
     *                 'for' loop, the offset for servo sweep is added to the initial and the final 
     *                 position of the servo, the head servo will also sweep through 90-i degree.
     * 
     * Example Call:   medium(10);
     */

    //ang : will iterate from 60+i to 120+i in steps of 2
    for(int ang = 60 + i; ang <= 120 + i; ang = ang+2)
    {
      Tail_D.write(ang);
      Tail_U.write(180-ang);
      Head.write(90-i);
      delay(10);
    }

    //ang : will iterate from 120+i to 60+i in steps of 2
    for(int ang = 120 + i; ang >= 60 + i; ang = ang-2)
    {
      Tail_D.write(ang);
      Tail_U.write(180-ang);
      Head.write(90-i);
      delay(10);
    }
}

void high(int i)
{
    /*
     * Function Name:  high
     * Input:          'i' the value of offset for servo sweep
     * Output:         None
     * Logic:          sweeps the servo through 60 degrees from left to right over time interval of 180ms(high speed)
     *                 and again through 60 degrees from right to left over the time interval of 180ms, the offset 
       *                 for servo sweep is added to the angle of sweep, the head servo will also sweep through 90-i degree.
     * Example Call:   high(10);
     */

    Tail_D.write(60+i);
    Tail_U.write(180-(60+i));
    Head.write(90-i);
    delay(180);
    Tail_D.write(120+i);
    Tail_U.write(180-(120+i));
    Head.write(90-i);
    delay(180);
}


