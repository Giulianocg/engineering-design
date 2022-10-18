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
const int in1 = 5;
const int in2 = 4;
const int in3 = 0;
const int in4 = 2 ;
//pins on motor driver connected to micro controller
int prevposition = 1;
//must be updated before position is. Starts assuming blinds in open position
int position = 2;
//0 is cold side, 1 is open, 2 is warm side.
int targettemp = 0;
int temp = 30;
//as determind by temperature sensor
bool light = true;
// variable for if the sun is out as determined by light sensor
int mode = 1;
// 0 = automatic, 1 = by target temp, 2 = manual
const int revsbtwnstates = 2; 
// revolutions of motor needed between states, ie from cold to open and open to warm

AccelStepper motor(AccelStepper::HALF4WIRE, in1, in3, in2, in4);
DHTNEW tempsensor(14);

//-----------------------------------------------------------------------------------------------------------------------------

//Define our functions::
// Converts resistance (in ohms) to temperature (in deg C)
const float R1 = 4.7;  // Fixed resistor next to thermistor. 0 if not required.
const float R2 = 2;  // Fixed resistor on other side of divider
const float Vcc = 3.3;
const float BETA = 1000;
const float NOM_RESIST = 1000;
const float NOM_TEMP = 1000;
float resistanceToCelsius(float resistance) {
  // Simplified Steinhart-Hart
  float temp = log(resistance / NOM_RESIST) / BETA;
  temp += 1.0 / (NOM_TEMP + 273.15);
  temp = 1 / temp;
  temp -= 273.15; // convert to C
  return temp;
}


void setup(){
  //----------------------------------------------------------------
  motor.setAcceleration(100);
  motor.setMaxSpeed(500);
  //-------------------------------------------------------------------
  //pinmodes A0 is LDR and .. is Thermistor
  pinMode(14, INPUT);
  //pinMode(, input); // fill this in ++++

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
  //float voltage = analogRead(A0); //--
  float voltage = 4; //--
  voltage = voltage / 1024;
  float totalResistance = ((Vcc * R2) / voltage) - R2;
  float resistance = (totalResistance - R1) * 1000;
  T_inside = digitalRead(14); //resistanceToCelsius(resistance);
  //Light = analogRead();

  //Send an HTTP POST request every 20 seconds
  delay(20000);
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
      
      if (Auto == 1){ //Automatic mode
        if (Light > 40){ //100 is just an example i dint know what the cutoff is
          If (T_inside > T_target){
            // close on reflective ****
          }
          else if (T_inside < T_target){
            // open ****
          }
          else {
            continue;
          }
        }
        else if (Light <= 40){
          // close blinds on absorbant ****
        }

      }
      else if (Auto == 0){ //manual mode
        if (Open = 1){
          //open ****
        }
        else if (Open = 0){
          if (Side = 0){ //reflect
            //closed on reflect ****
          }
          else if (Side = 1){ //Absorb
            // closed on absorb ****
          }          
        }
      }


      //-------------------------------------------------------------------------------------------------------------------------------------
    }
    Serial.println();



    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

