
#include <ESP8266WiFi.h> 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
int red_led_pin    = 2;
int green_led_pin  = 4;
int blue_led_pin  = 5;
float RS_gas = 0;
float ratio = 0;
float sensorValue = 0;
float sensor_volt = 0;
float R0=150;                           //sensor resitance value as calibrated
float nppm;

double ppm;
const char *ssid = "*****";              //WiFi SSID
const char *password = "*****";       //WiFi password

         //aws topic 

#if !(ARDUINOJSON_VERSION_MAJOR == 6 and ARDUINOJSON_VERSION_MINOR == 5)
  #error "Install ArduinoJson v6.5.0-beta"
#endif



void setup() 
{
  pinMode(red_led_pin,OUTPUT);
  pinMode(green_led_pin,OUTPUT);
   pinMode(blue_led_pin,OUTPUT);
   
  
  pinMode(A0,INPUT);
  Serial.begin(115200);
   
   WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(100);
      Serial.print(".");
      RGB_color(HIGH,HIGH,LOW); //blue
    delay(50);
    }

    
    Serial.printf("mq7 connect=%d\n", res);

   
}

void loop() 
{
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(500);
      Serial.print(".");
     RGB_color(HIGH,HIGH,LOW); //BLUE
    delay(50);
   }


if(ppm>=2.5)
{
  RGB_color(LOW,HIGH,HIGH); //glow red color when ppm value is above or equal to 50ppm 
  delay(100);
  Serial.print("red");

  nppm=0;
  for(int i=0;i<100;i++)
   {
   sensorValue = analogRead(A0);      //reading analog value from MQ7 sensor
   sensor_volt = sensorValue/1024*5.0;
   RS_gas = (5.08-sensor_volt)/sensor_volt;
   ratio = RS_gas/R0; 
   float x = 1538.46 * ratio;
   ppm = pow(x,-1.709);       // formula for conversion to ppm value
    nppm=nppm+ppm;
   }
   ppm=nppm/100;             //calculating average of 100 ppm values
   Serial.print("PPM: ");
   Serial.println(ppm);
   delay(100);
    Serial.print("red");    
}
else{
while(1)
{ 
  nppm=0;
  for(int i=0;i<100;i++)
   {
   sensorValue = analogRead(A0);      //reading analog value from MQ7 sensor
   sensor_volt = sensorValue/1024*5.0;
   RS_gas = (5.08-sensor_volt)/sensor_volt;
   ratio = RS_gas/R0; 
   float x = 1538.46 * ratio;
   ppm = pow(x,-1.709);       // formula for conversion to ppm value
    nppm=nppm+ppm;
   }
   ppm=nppm/100;             //calculating average of 100 ppm values
   Serial.print("PPM: ");
   Serial.println(ppm);
   delay(100);
       

if(ppm<=2.5)
{
 RGB_color(HIGH,LOW,HIGH);  //glow green color when ppm value is below 50ppm 
 delay(100);
  Serial.print("green");
   
}

 else if(ppm<1)
 {
  RGB_color(HIGH,HIGH,LOW); //glow red color when ppm value is above or equal to 50ppm 
  delay(100);
  Serial.print("blue");
 }      
 
  delay(10); //Wait 1 second 
 
    Serial.println();
    if(ppm>=2.5)
   {
    break;
   }
 
  }
  else {
    Serial.println("Not connected...");
    delay(20);
  }

  delay(4000);
   
}
}
}


void RGB_color(int red_led_value,int green_led_value,int blue_led_value)
{
  digitalWrite(red_led_pin,red_led_value);
    digitalWrite(green_led_pin,green_led_value);
    digitalWrite(blue_led_pin,blue_led_value);
     
}
