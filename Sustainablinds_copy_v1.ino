//connection vars
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <iostream>
#include <string>

#include <AccelStepper.h>
#include <dhtnew.h>

ESP8266WiFiMulti WiFiMulti;

const char* Wifi_name = "Alonso";                                             //fill this in!!!!
const char* Wifi_pass = "onetwothree"; 
const char* host = "engdesblinds.000webhostapp.com";

// variables to send
bool Status = 0;
int T_inside = 0;
int Light = 0;
//vars to receive
int Time = 0;
int Open = 0;
int Side = 0;
int Auto = 0;
int T_target = 0;
float T_outside = 0;

//-----------------------------------------------------------------------your stuff--------------------------------------

const int revolution = 4096;  
//motor steps for a full revolution
const int in1 = 12; //D6
const int in2 = 13; //D7
const int in3 = 4; //D2
const int in4 = 5; //D1
//pins on motor driver connected to micro controller
int prevposition = 1;
//must be updated before position is. Starts assuming blinds in open position
int position = 1;
//0 is reflective, 1 is open, 2 is absorbent.
const int revsbtwnstates = 2; 
// revolutions of motor needed between states, ie from cold to open and open to warm

AccelStepper motor(AccelStepper::HALF4WIRE, in1, in3, in2, in4);
DHTNEW tempsensor(14);

//-----------------------------------------------------------------------------------------------------------------------------

float temperature() {
  tempsensor.read();
  int temp = tempsensor.getTemperature();
  //Serial.println(tempsensor.getTemperature(), 1);
  return temp;
}


void setup(){
  //----------------------------------------------------------------
  motor.setAcceleration(100);
  motor.setMaxSpeed(500);
  //-------------------------------------------------------------------

  //internet connection
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(Wifi_name, Wifi_pass); 

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP()); //If you dont see this, then you put the wrong password or name
}

void loop(){
  //get the temperature from the resistance
  Serial.println("Ha entrado en el loop"); //-- 
  T_inside = temperature();
  Light = analogRead(A0);

  //Send an HTTP POST request every 10 seconds
  delay(10000);
  if(WiFi.status()== WL_CONNECTED){
    Serial.println("Entrado en el if-statement, check"); //--
    HTTPClient http;
    WiFiClient client;

    String url = "/TX.php?" ;
    String body = "un=1&id=99999&pw='Password123456-'&n1=";
    body += String(15); //-- deberia ser T_inside
    Serial.println(url); //--
    Serial.println(body); //--
    Serial.println(host); //--
    client.println(String("GET ") + url + body + " HTTP/1.1");   //if this doesnt work, try POST
    client.println(String("Host: ") + host);
    client.println("Connection: close");
    //client.print("Content-Length: ");
    //client.println(body.length());
    client.println();
    //client.println(body);
    Serial.println("Acaba de pasar el packet"); //--
    // Wait for the response
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("Client timed out!");
        client.stop();
        return;
      }
    }

    // Read the reply and print it out
    Serial.println("Got response:");
    if (client.available()){
      char c = client.read(); //StringUntil('\r') line should be something like 436##1##1##1##20##3.6, otherwise i think i know what might have gone wrong
      String line(1, c);
      Serial.print(line);

      String arrayyy[6]; //we put the data into each variable
      int icounter = 0;
      String delimiter = "##";
      size_t pos = 0;
      String token;
      while ((pos = line.indexOf(delimiter)) != std::string::npos) {
          token = line.substring(0, pos);
          arrayyy[icounter] = token;
          icounter = icounter + 1;
          line.remove(0, pos + delimiter.length());
      }
      arrayyy[icounter] = line;

      Serial.print("Time: " + arrayyy[0] + "Mode: " + arrayyy[1] + "Side: " + arrayyy[2] + "Auto: " + arrayyy[3] + "T_target: " + arrayyy[4] + "T_outside: " + arrayyy[5]  );
      Time = arrayyy[0].toInt();
      Open = arrayyy[1].toInt();
      Side = arrayyy[2].toInt();
      Auto = arrayyy[3].toInt();
      T_target = arrayyy[4].toInt();
      T_outside = arrayyy[5].toInt(); //with that, we should have all our data
      
      //--------Here we do the actual logic for the blinds--------------------------------------------------------------------------------
      
      if (Auto == 1) { //Automatic mode
        position = automode();

      }
      else if (Auto == 0) { //manual mode
        position = manualmode();
      }
      if (motor.distanceToGo() = 0) {
        motor.moveTo((position-prevposition)*revsbtwnstates*revolution);
        prevposition = position;
      }
    }
    Serial.println();
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  motor.run();
}

int automode() {
  if (Light <= 40) { 
    return 2;
    // close blinds on absorbant ****
  }
  else if (T_inside > T_target) {
    return 0;
    // close on reflective **** 
  }         
  else if (T_inside < T_target) {
    return 1;
    // open ****
  }
  else {
    return prevposition;
  }
}

int manualmode() {
  if (Open = 1) {
    return 1;
    //open ****
  }
  else if (Side = 0) {
    return 0;
    //closed on reflect ****
  }
  else if (Side = 1) { //Absorb
    return 2;
    // closed on absorb ****
  }          
  else {
    return prevposition;
  }
}
