  #include<SPI.h>
  #include<nRF24L01.h>
  #include<RF24.h>
  
  RF24 radio(7,8);
  
  const byte address[6] = "00001";
  const int switch_pin_j1 = 2;
  const int switch_pin_j2 = 4;
  const int X_pin_j1 = 3;
  const int Y_pin_j2 = 4;
  int i;
  char val[50];
  
void setup() 
{
  // put your setup code here, to run once:
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  pinMode(switch_pin_j1, INPUT);
  digitalWrite(switch_pin_j1, LOW);
  pinMode(switch_pin_j2, INPUT);
  digitalWrite(switch_pin_j2, LOW);
  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  String data = process(analogRead(X_pin_j1), analogRead(Y_pin_j2));
  data.toCharArray(val, 50);
  radio.write(&val,sizeof(val));
  delay(1000);
}

String process(int X_val, int Y_val)
{
  String value= "";
  if(Y_val>470 && Y_val<550)
  {
    i = 0;  
  }
  if(Y_val<=470)
  {
    i = map(Y_val, 0, 470, -30, 0);  
  }
  
  if(Y_val>=550)
  {
    i = map(Y_val, 550, 1020, 0, 30);  
  }
  if(X_val<550)
  {
    value = value + '0';
    value = value + String(i);
    return value;  
  }
  if(X_val>=550 && X_val<=700)
  {
    value = value + '1';
    value = value + String(i);
    return value;
  }

  else if(X_val>700 && X_val<=900)
  {
    
    value = value + '2';
    value = value + String(i);
    return value;
  }

  else if(X_val>900)
  {
    
    value = value + '3';
    value = value + String(i);
    return value;
  }
}
