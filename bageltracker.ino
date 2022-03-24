:::ESP32 // GPS Module // IoT Project Script::: 
 
#include <WiFi.h>               //required libraries  
#include <AWS_IOT.h>            //list of libraries  
#include <HardwareSerial.h>     //list of libraries  
#include <TinyGPS++.h>          //list of libraries  
 
int green = 21;             //green LED pin# 
int red = 32;               //red LED pin# 
 
#define RXD2 16      //ESP32 RX pin 
#define TXD2 17     //ESP32 TX pin 
 
const char* ssid = "mphone";               //mobile hotspot ssid 
const char* password = "password";    //mobile hotspot password 

#define MQTT_TOPIC "$aws/things/NEO6M_data/shadow/update"           //define topic 
being published to 
#define AWS_HOST "awc6ocnahyhrq-ats.iot.us-east-2.amazonaws.com"    //define AWS host 
location 
#define CLIENT_ID "gps_coordinates"                                 //define client 
ID  
 
AWS_IOT aws;            //shortcut name 
TinyGPSPlus gps;        //shortcut name 
WiFiClient espClient;   //shortcut name 
 
void setup() { 
  Serial.begin(115200);         //set baud rate 
  pinMode(green, OUTPUT);       //set pin mode 
  pinMode(red, OUTPUT);         //set pin mode 
  delay(1000);                  //one second delay 
 
  WiFi.mode(WIFI_STA);          //enable wifi 
  WiFi.begin(ssid, password);   //enter wifi credentials 
  Serial.println("Connecting"); //message while waiting for connection 
 
  while (WiFi.status() != WL_CONNECTED)  //while loop waiting for internet connection 
to be established 
  { 
    delay(200);                         //delay 
    Serial.print(".");                  //delay with printed periods 
  } 
   
  Serial.println("Connection establish to IP address: ");     //message that internet 
connection has been made 
  Serial.println(WiFi.localIP());                             //print IP address 
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);                //second serial 
protocol addressing RX/TX pins 
 
  Serial.println("Connecting to Amazon Web Services...");      //message while 
waiting to establish AWS connection 
  if(aws.connect(AWS_HOST, CLIENT_ID)==0)                     //loop to connect 
    Serial.println("Connected");                               //connection made 
  else  
    Serial.println("Connection failed. Check AWS_HOST & CLIENT_ID");    //connection 
failes 
} 
 
void loop() { 
  while(Serial1.available() > 0)        //while loop for serial connection 
    gps.encode(Serial1.read());         //secondary serial reads gps data 
  if(gps.location.isUpdated() && gps.altitude.isUpdated())      //if gps is working 
  { 
    double latitude = gps.location.lat();       //save data to variable 
    double longitude = gps.location.lng();      //save data to variable 
    if(isnan(latitude) || isnan(longitude)){    //if data not good 
      Serial.println(F("Failed to read GPS"));      //informs user 
      return; 
    } 
    else{ 
      String gps_coordinates = "Latitude: ";    //data is good, beginning stringing 
data together 
      gps_coordinates += String(latitude);      //stringing data together 
      gps_coordinates += " N | Longitude: ";    //stringing data together 
      gps_coordinates += String(longitude);     //stringing data together 
      gps_coordinates += " W";                  //stringing data together 
      char payload[50];                         //string data size 
      gps_coordinates.toCharArray(payload,50);  //format string data 
      Serial.println("Publishing...");          //publishing  
      Serial.println(payload);                  //print gathered data 
      if(aws.publish(MQTT_TOPIC,payload)==0)    //if MQTT client delivers data 
        Serial.println("Publishing payload: SUCCESS");      //let user know success 
      else   
        Serial.println("Publishing Payload: FAILED"):       //let user know failure 
    } 
    delay(1000);                        //one second delay 
    float L1 = gps.location.lat();      //save coordinates in varible 
    float L2 = gps.location.lng();      //save coordinates in varible 
    if(L1 > 29.672000 && L1 < 29.674000 && L2 < -82.342000 && L2 > -82.344000){   
//if AT HOME 
      digitalWrite(green, HIGH);             //Green LED turns ON  
      digitalWrite(red, LOW);                   //Red LED turns OFF 
    } 
    else if(L1 > 29.637000 && L1 < 29.641000 && L2 < -82.341000 && L2 > -82.345000){  
//if AT WORK 
      digitalWrite(green, LOW);             //Green LED turns OFF 
      digitalWrite(red, HIGH);                  //Red LED turns ON 
    } 
    else{                               //if at neither WORK or HOME 
      digitalWrite(green, LOW);         //turn OFF LED's 
      digitalWrite(red, LOW); 
    } 
    Serial.print("The Latitude is: ");          //display latitude information  
    Serial.println(L1, 6);                      //display latitude information  
    Serial.print("The Longitude is: ");         //display latitude information  
    Serial.println(L2, 6);                      //display latitude information  
    delay(30000);                               //30 second delay  
  } 
}
