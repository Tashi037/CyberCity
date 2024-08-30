#include <CyberCitySharedFunctionality.h>
#include <ArduinoJson.h>
/***************************************************
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#define TRIG_PIN 13
#define ECHO_PIN 12

int tl1Green = 15;
int tl1Red = 16;
int tl1Yellow = 17;
int tl2Green = 27;  // Fix
int tl2Red = 13;    // Fix
int tl2Yellow = 12; // Fix

#include "sensitiveInformation.h"

CyberCitySharedFunctionality cyberCity;

void lightsOn()
{
  digitalWrite(tl1Red, HIGH);
  digitalWrite(tl2Green, HIGH);
  delay(5000);
  digitalWrite(tl2Green, LOW);
  digitalWrite(tl2Yellow, HIGH);
  delay(1000);
  digitalWrite(tl2Yellow, LOW);

  digitalWrite(tl2Red, HIGH);
  digitalWrite(tl1Green, HIGH);
  delay(5000);
  digitalWrite(tl1Green, LOW);
  digitalWrite(tl1Yellow, HIGH);
  delay(1000);
  digitalWrite(tl1Yellow, LOW);
}

void lightsOff()
{
  digitalWrite(tl1Red, LOW);
  digitalWrite(tl1Yellow, LOW);
  digitalWrite(tl1Green, LOW);
}

void sonarSensorData()
{

  float duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * .0343) / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.print("Payload from server:");
  String dataToPost = String(distance);
  String payload = cyberCity.dataTransfer(dataToPost, apiKeyValue, sensorName, sensorLocation, 300, serverName, true, true);
  Serial.print("Payload from server:");
  Serial.println(payload);
  DynamicJsonDocument doc(1024);
  //  Serial.println(deserializeJson(doc, payload));
  DeserializationError error = deserializeJson(doc, payload);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char *command = doc["command"];
  Serial.print("Command: ");
  Serial.print(command);
  delay(500);
  if (String(command) == "On")
  {
    Serial.println("normal operation:)");
    lightsOn();
    // outputCommand = "Fan On";
  }
  else
  {
    // outputCommand = "Fan Off";
    Serial.println("Traffic light chaos");
    lightsOff();
  }
}

void setup()
{
  /*
   */

  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial)
  {
    delay(10);
  }
  delay(1000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println();
  Serial.print("Connected to the Internet");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(LED_BUILTIN, OUTPUT);

  // RTC
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
  }

  // The following line can be uncommented if the time needs to be reset.
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  rtc.start();

  // EINK
  display.begin();
  display.clearBuffer();

  cyberCity.logEvent("System Initialisation...");

  // Module Specific Code

  // put your setup code here, to run once:
  pinMode(tl1Green, OUTPUT);
  pinMode(tl1Red, OUTPUT);
  pinMode(tl1Yellow, OUTPUT);
  pinMode(tl2Green, OUTPUT);
  pinMode(tl2Red, OUTPUT);
  pinMode(tl2Yellow, OUTPUT);
  lightsOff();
  Serial.begin(9600);
}

void loop()
{

  // put your main code here, to run repeatedly:
  // int sensorData = red, green, yellow;
  sonarSensorData(); // this function runs both sonarSensorData and Lights on and Off
}
