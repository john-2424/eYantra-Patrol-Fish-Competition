/*
 * Team Id:          5066
 * Author List :     Srinivas A, Aikansh Prakash Shenoy, 
 *                   Shrikrishna Bhagirath Rajule, Md Tauseeq Zuman
 * File name:        Wireless_Joystick
 * Theme:            Patrol Fish(PF)
 * Functions:        setup, loop, process, buzzing 
 * Global variables: radio, address, switch_pin_j1, switch_pin_j2, X_pin_j1,
 *                   Y_pin_j2, buzzer
*/
 
#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

//radio: creates an object RF24 of the library RF24, input arguments are CE, CSN pin number 
RF24 radio(7,8);

//address: represents the address or pipe through which communication happens
const byte address[6] = "00001";
//switch_pin_j1: represents the joystic 1's switch pin number
const int switch_pin_j1 = 2;
//switch_pin_j2: represents the joystic 2's switch pin number
const int switch_pin_j2 = 4;
//X_pin_j1: represents the joystic 1's x axis pin number
const int X_pin_j1 = 4;
//Y_pin_j1: represents the joystic 2's y axis pin number
const int Y_pin_j2 = 1;
//buzzer: represents the buzzer pin number
const int buzzer = 5;
//time: the total time
unsigned long time;
  
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
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  //Setting up the Joysticks, Buzzer and Serial Monitor
  pinMode(switch_pin_j1, INPUT);
  pinMode(switch_pin_j2, INPUT);
  digitalWrite(switch_pin_j1,HIGH);
  digitalWrite(switch_pin_j2,HIGH);
  pinMode(buzzer,OUTPUT);
  Serial.begin(9600);

  //Running the initial beep of buzzer for 2 seconds indicating the start
  digitalWrite(buzzer,HIGH);
  digitalWrite(buzzer,LOW);
  delay(2000);
  digitalWrite(buzzer,HIGH);
}

void loop() 
{
/*
 * Function Name: loop
 * Input:         None
 * Output:        None
 * Logic:         Sends the processed joystick commands and keeps track of the 
 *                time, from the joystick start, runs repeatedly and contains the main code,
 *                function is called by the sketch
*/

  //data: contains the processed data in string format
  //call the function process to process the read input from the joystics
  String data = process(analogRead(X_pin_j1), analogRead(Y_pin_j2));
  //val: acts as a buffer for the processed data and ready to send
  char val[50];
  data.toCharArray(val, 50);      //convert from string to charcter array format and store it in val 
  Serial.println(data);
  radio.write(&val,sizeof(val));  //Send the processed data
  delay(1000);

  //buzz: represents the buzzer pin state, values are 1 or 0
  int buzz = digitalRead(switch_pin_j1);
  Serial.println(buzz);
  
  //call the function buzzing if buzz value is 0
  if(buzz == 0)
  {
    Serial.print("The Time taken to complete the Run:");
    Serial.print(time/1000);
    Serial.println(" seconds");
    buzzing();
  }
  //Keeping track of the total time (600 seconds)  
  Serial.print("Time: ");
  time = millis();
  //Prints time, since the program started
  Serial.print(time/1000); 
  Serial.println(" seconds");
  //Condition to check if the Total Time is up 
  //The Total time is 602 seconds instead of 600 seconds. The extra 2 seconds accounts for the buzzer time (2 seconds) at the start. 
  if(time>602000 && time<603000)
  {
    buzzing();            
  }
}

String process(int X_val, int Y_val)
{
/*
  * Function Name:  process
  * Input:          X_val,Y_val where X_val is value of X_pin_j1, Y_val is value of Y_pin_j2
  * Output:         String containing the processed data
  * Logic:          Contains a series of if else statements to sort the joystic's input, here the
  *                 X_val is mapped to the different speed mode 0 for stop, 1 for slow speed,
  *                 2 for medium speed and 3 for high speed. Y_val is mapped to the range of -30
  *                 to +30 which defines the offset for the sweep of the servo, the speed mode and
  *                 the offset value is converted into string and concatinated to a single string
  *                 value.           
  * Example Call:   process(500,450);
*/
    
  //value: buffer for adding the processed data to it
  String value= "";
  //i: contains the value of offset for servo sweep, ranges from -30 to +30
  int i;

  // if statements for assigning the offset for sweep values
  if(Y_val>470 && Y_val<550)                //no offset for straight direction
  {
    i = 0;  
  }
  
  if(Y_val<=470)                            //for left offset
  {
    i = map(Y_val, 0, 470, -30, 0);
  }
  
  if(Y_val>=550)                            //for right offset
  {
    i = map(Y_val, 550, 1020, 0, 30);  
  }

  //if else statements for mapping the speed mode values
  if(X_val<550 && X_val>450)
  {
    value = value + '0';                //concatinate the speed mode to buffer
    value = value + String(i);          //concatinate the sweep offset to buffer
    return value;                       //return the processed string
  }
  if(X_val<=450)
  {
    value = value + '1';                //concatinate the speed mode to buffer
    value = value + String(i);          //concatinate the sweep offset to buffer
    return value;                       //return the processed strin
  }
  else if(X_val>=550 && X_val<=900)
  {
    value = value + '2';                //concatinate the speed mode to buffer
    value = value + String(i);          //concatinate the sweep offset to buffer
    return value;                       //return the processed strin
  }
  else if(X_val>900)
  {
    value = value + '3';                //concatinate the speed mode to buffer
    value = value + String(i);          //concatinate the sweep offset to buffer
    return value;                       //return the processed strin
  }
}

void buzzing()
{
/*
 * Function Name:  buzzing
 * Input:          None
 * Output:         None
 * Logic:          Switch on the buzzer for 3 seconds and then switch off
 * Example Call:   buzzing();
*/
   
  //switch on the buzzer
  digitalWrite(buzzer,LOW);
  delay(3000);
  //switch off the buzzer
  digitalWrite(buzzer,HIGH);
}
