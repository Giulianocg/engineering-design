//connection vars
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <iostream>
#include <string>

#include <AccelStepper.h>
#include <dhtnew.h>


const char* Wifi_name = "Giuliano";  //fill this in!!!!
const char* Wifi_pass = "fuckyouman";
const char* host = "engdesblinds.000webhostapp.com";

// variables to send
bool Status = 0;
float T_inside = 0;
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
const int in1 = 12;
const int in2 = 13;
const int in3 = 4;
const int in4 = 5;
//pins on motor driver connected to micro controller
const int poscloseda = 0;
const int posclosedr = 1000;  //make it even for simplicity
const int posopen = posclosedr / 2;
// define the distances

AccelStepper motor(AccelStepper::HALF4WIRE, in1, in3, in2, in4);
DHTNEW tempsensor(14);

//-----------------------------------------------------------------------------------------------------------------------------

void setup() {
  //----------------------------------------------------------------
  motor.setAcceleration(100);
  motor.setMaxSpeed(500);

  //We will assume it starts on closed-absorbant, ans call this 0
  motor.setCurrentPosition(0);

  //-------------------------------------------------------------------

  //internet connection
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(Wifi_name, Wifi_pass);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());  //If you dont see this, then you put the wrong password or name
}

void loop() {
  //get the temperature
  tempsensor.read();
  T_inside = tempsensor.getTemperature();
  //T_inside = 13.9;//change this for now to test cases


  //get light
  Light = analogRead(A0);
  //Light = 100; //change this for now to test cases

  Serial.println(Light);
  Serial.println(T_inside);
  Serial.println("Ha entrado en el loop");  //--

  //Send an HTTP POST request every 20 seconds
  delay(5000);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Entrado en el if-statement, check");  //--
    HTTPClient http;
    WiFiClient client;

    String url = "/TX.php?un=1&id=99999&pw='Password123456-'&n1=";
    url += String(T_inside);
    Serial.println(host);  //--
    Serial.println(url);   //--

    if (http.begin(client, "http://engdesblinds.000webhostapp.com" + url)) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          Serial.println("Acaba de pasar el packet y deberia haberlo recibido");  //--
          String payload = http.getString();
          Serial.println(payload);
          String line = payload;

          String arrayyy[6];  //we put the data into each variable
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

          Serial.println("Time: " + arrayyy[0] + " Mode: " + arrayyy[1] + " Side: " + arrayyy[2] + " Auto: " + arrayyy[3] + " T_target: " + arrayyy[4] + " T_outside: " + arrayyy[5]);
          //motor.setCurrentPosition(0);
          Time = arrayyy[0].toInt();
          Open = arrayyy[1].toInt();
          Side = arrayyy[2].toInt();
          Auto = arrayyy[3].toInt();
          T_target = arrayyy[4].toInt();
          T_outside = arrayyy[5].toFloat();  //with that, we should have all our data

          //--------Here we do the actual logic for the blinds--------------------------------------------------------------------------------

          if (Auto == 1) {  //Automatic mode
            if (Light <= 40) {
              // close blinds on absorbant ****
              Serial.println(" closed, abs. Dark outside");
              motor.moveTo(poscloseda);
              delay(500);
              while ((motor.distanceToGo() != 0)) {  //double brackets? apparently but im not confdent, check afterwards. I doubt it.
                motor.run();
              }
            } else if (T_inside > T_target) {
              // close on reflective ****
              Serial.println(" closed, reflective");
              motor.moveTo(posclosedr);
              delay(500);
              while ((motor.distanceToGo() != 0)) {  //double brackets? apparently but im not confdent, check afterwards.
                motor.run();
              }
            } else if (T_inside < T_target) {
              // open ****
              //Serial.println(" open");
              //Serial.println("1");
              motor.moveTo(posopen);
              //Serial.println("2");
              delay(500);
              //Serial.println("3");
              while (motor.distanceToGo() != 0) {  //double brackets? apparently but im not confdent, check afterwards.
                Serial.println(motor.distanceToGo());
                motor.run();
              }
              Serial.println("4");
            }
          } else if (Open = 1) {
            //open ****
            Serial.println(" open");
            motor.moveTo(posopen);
            delay(500);
            while ((motor.distanceToGo() != 0)) {  //double brackets? apparently but im not confdent, check afterwards.
              motor.run();
            }
          } else if (Side = 0) {  //reflect
            //closed on reflect ****
            Serial.println(" closed, reflective");
            motor.moveTo(posclosedr);
            delay(500);
            while ((motor.distanceToGo() != 0)) {  //double brackets? apparently but im not confdent, check afterwards.
              motor.run();
            }
          } else if (Side = 1) {  //Absorb
            // closed on absorb ****
            Serial.println(" closed, abs");
            motor.moveTo(poscloseda);
            delay(500);
            while ((motor.distanceToGo() != 0)) {  //double brackets? apparently but im not confdent, check afterwards.
              motor.run();
            }
          }
          Serial.println("5");
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      Serial.println("6");
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
    Serial.println("7");
  } else {
    Serial.println("WiFi Disconnected");
  }
  Serial.println("8");
}
