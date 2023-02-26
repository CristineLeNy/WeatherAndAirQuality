//profhacks 2023 
//Cristine Le Ny, Matthew McBurney, Brandon Walsh 


//libraries 
#include <Wire.h>
#include <String.h>
#include "SparkFun_SCD30_Arduino_Library.h"

//sensors
SCD30 airSensor;
const int measurePin = A3;
const int ledPower = 23;
const int trigPin=A0;
const int echoPin=A1;
const int scl = 21;
const int sda = 20;
float duration;

//variables 
//sensor data
float co2[20], temp[20], humidity[20],distance[20],dusty[20],time[20]; 
//counter 
int i=0; 

void setup() 
{
  Serial.begin(115200);
  //dust sensor setup 
  pinMode (ledPower, OUTPUT);

  //ultrasonic sensor setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);
  digitalWrite(trigPin, LOW);

  //check for scd30 connection
  while (airSensor.begin() == false)
  {
    Serial.println("Air sensor not detected. Please check wiring.");
    delay(500);
    Wire.begin(); //for i2c
  }
  delay(2000);

}

void dust()
{
  if (i<20)
  {
  digitalWrite(ledPower, LOW);
  delayMicroseconds(250);
  float voltMeasured=analogRead(measurePin);
  delay (40);
  digitalWrite(ledPower,HIGH);
  float calcVolt=voltMeasured*(3.3/1024);
  dusty[i]=170*calcVolt-.1;
  delay(1000);
  }
}

void ultrasonic ()
{
  if (i<20)
  {
  // Clears the trigPin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance[i] = duration * 0.034 / 2;
  delay(100);
  }
}
void scd30()
{
    if (i<20)
    {
      delay(2000);
      //get co2 reading
      co2[i]=airSensor.getCO2();

      //get temperature reading 
      temp[i]=airSensor.getTemperature();

      //get humidity
      humidity[i]=airSensor.getHumidity();
  }    
}
void printData()
{
  delay(20);
  Serial.println("Time Stamps: ");
  for (int j=0; j<20; j++)
  {
    Serial.print(String(time[j]) + " ");
    delay(10);
  }
  Serial.println();

  delay(20);
  Serial.println("Temperature Data (C): ");
  for (int j=0; j<20; j++)
  {
    Serial.print(String(temp[j]) + " ");
    delay(10);
  }
  Serial.println();

  Serial.println("CO2 Data (ppm): ");
  for (int j=0; j<20; j++)
  {
    Serial.print(String(co2[j]) + " ");
    delay(10);
  }
  Serial.println();

  Serial.println("Humidity Data (%): ");
  for (int j=0; j<20; j++)
  {
    Serial.print(String(humidity[j]) + " ");
    delay(10);
  }
  Serial.println();

  Serial.println("Distance Data (cm): ");
  for (int j=0; j<20; j++)
  {
    Serial.print(String(distance[j]) + " ");
    delay(10);
  }
  Serial.println();

  Serial.println("Dust (ug/m3): ");
  for (int j=0; j<20; j++)
  {
    Serial.print(String(dusty[j]) + " ");
    delay(10);
  }
  Serial.println();
}

void loop() 
{  
  char input = Serial.read();
  if (input== 'p')
  {
    printData();
  }
  
  if (i<20)
  {
    time[i]=millis();
    ultrasonic();
    dust();
    scd30();
    i++;

    delay(2000);
  }
     
}
