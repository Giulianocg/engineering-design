//connection vars
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <iostream>
#include <string>

ESP8266WiFiMulti WiFiMulti;

const char* Wifi_name = "";                                             //fill this in!!!!
const char* Wifi_pass = ""; 
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
  //pinmodes A0 is LDR and .. is Thermistor
  pinMode(A0, INPUT);
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
  T_inside = resistanceToCelsius(resistance);
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
      String line = client.read(); //StringUntil('\r') line should be something like _t1436##_b11##_b21##_b31##_n120##_n23.6##, otherwise i think i know what might have gone wrong
      Serial.print(line);

      std::string arrayyy[6]; //we put the data into each variable
      int icounter = 0;
      std::string delimiter = "##";
      size_t pos = 0;
      std::string token;
      while ((pos = line.find(delimiter)) != std::string::npos) {
          token = line.substr(0, pos);
          std::cout << token << std::endl;
          std::cout << icounter << std::endl;
          arrayyy[icounter] = token;
          icounter = icounter + 1;
          line.erase(0, pos + delimiter.length());
      }
      arrayyy[icounter] = line;

      Time = stoi(arrayyy[0]);
      Open = stoi(arrayyy[1]);
      Side = stoi(arrayyy[2]);
      Auto = stoi(arrayyy[3]);
      T_target = stoi(arrayyy[4]);
      T_outside = std::stof(arrayyy[5]); //with that, we should have all our data

      //       Here we do the actual logic for the blinds













    }
    Serial.println();



    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

