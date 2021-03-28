// MPU-6050 Short Example Sketch
//www.elegoo.com
//2016.12.9

#include<Wire.h>
//#include <Adafruit_CircuitPlayground.h>

const int MPU_addr=0x68;    // I2C address of the MPU-6050
static int16_t X,Y,Z;       // variables used for capturing sensor values
const int SW_pin = 12;      // digital pin connected to switch output
const int X_pin = A0;       // analog pin connected to X output of joystick
const int Y_pin = A1;       // analog pin connected to Y output of joystick
const int buzzer = 13;      // analog pin connected to buzzer
int incomingByte = 0;       // variable used for reading incoming data from the serial port 


void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);           // PWR_MGMT_1 register
  Wire.write(0);              // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

// this function matches the values recieved from the mpu-6050 sensor to
// w, s, d or a that is then sent serially to the host 
void mpu()
{ 
  if(X >= 400 || X <= 100){
    if(Y >= -35 && Y <= 30){
      if(Z >= -150 && Z <= 100){
        Serial.print('e');
      }
    }  
  }
  
  if(Z >= -210 && Z <= 5){
    if(Y >= 0 && Y <= 20){
      if(X >= 120 && X <= 260){
        Serial.print('w');
      }
    }
  } 
  
  if(Z >= 30 && Z <= 230){
    if(Y >= 0 && Y <= 20){
      if(X >= 120 && X <= 260){
        Serial.print('s');
      }
    }
  }
  
  if(Y >= -206 && Y <= -100){
    if(Z >= 1 && Z <= 20){
      if(X >= 120 && X <= 260){
        Serial.print('d');
      }
    }
  }
  
  if(Y >= 100  && Y <= 200){
    if(Z >= 1 && Z <= 20){
      if(X >= 120 && X <= 260){
        Serial.print('a');
      }
    }
  }
}

// this function matches the values recieved from the joystick to
// w, s, d or a that is then sent serially to the host 
void js()
{
  if(analogRead(X_pin) == 488){
    if(analogRead(Y_pin) == 0)
      Serial.print('w');
  }
  if(analogRead(X_pin) == 488){
    if(analogRead(Y_pin) == 1023)
      Serial.print('s');
  }
  else if(analogRead(X_pin) == 0){
    if(analogRead(Y_pin) == 514)
      Serial.print('d');
  }
  if(analogRead(X_pin) == 1023){
    if(analogRead(Y_pin) == 514)
      Serial.print('a');
  }
}

// this function activates the buzzer 
// incomingByte recieves serial data from snake.py 
// this data is checked with the letter 'B' and if matched
// buzzer gets activated 
void buzzr()
{
  if(Serial.available() > 0){
    incomingByte = Serial.read();
    if(incomingByte == 'B')
    {
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
    }
  }
}

void loop()
{  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);                    
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  

  X=(Wire.read()<<8|Wire.read())/65.5;  
  Y=(Wire.read()<<8|Wire.read())/65.5;  
  Z=(Wire.read()<<8|Wire.read())/65.5;  
  
  mpu();
  js();
  buzzr();
  delay(100);
}
