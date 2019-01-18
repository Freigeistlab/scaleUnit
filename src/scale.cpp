#include "HX711.h"

#include <Ethernet.h>
#include <SPI.h>

#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>

#include <ArduinoJson.h>

WiFiManager wifiManager;

#define DOUT 5
#define CLK 4

const char serverIP[] = "http://192.168.43.25";
const unsigned int port = 5000;
const String unitName = "scale";

HX711 scale(DOUT, CLK);
float calibration_factor = -7050;

const int capacity = JSON_OBJECT_SIZE(2);
StaticJsonBuffer<capacity> jb;

String response;

void setup()
{
  Serial.begin(9600);
  wifiManager.autoConnect("AutoConnectAP");

  scale.set_scale(calibration_factor);
  scale.tare();
}

void sendVal(long val)
{
  HTTPClient http; //Declare object of class HTTPClient

  String requestDestination = String(serverIP) + String(":") + String(port) + String("/") + String(unitName);

  JsonObject& obj = jb.createObject();
  obj["id"] = 0;
  obj["value"] = 10;

  http.begin(requestDestination);               //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8"); //Specify content-type header

  String output = "id=0&value=" + String(val);
  //obj.printTo(output);

  int httpCode = http.POST(output); //Send the request
  String payload = http.getString();     //Get the response payload

  Serial.println(output);

  Serial.println(httpCode); //Print HTTP return code
  Serial.println(payload);  //Print request response payload

  http.end(); //Close connection
}

long readScale()
{
  return scale.get_units();
}

void loop()
{
  sendVal(readScale());
}

