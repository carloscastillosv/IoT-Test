#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "secrets.h"

// put function declarations here:
int myFunction(int, int);

// Constant Definition
// const char* ssid = "TIGO-D347";
// const char* password = "4NJ667301187";

#define AWS_IOT_PUBLISH_TOPIC   "testTempMonitor"
#define AWS_IOT_SUBSCRIBE_TOPIC "testTempMonitor"

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["humidity"] = 10;
  doc["temperature"] = 20;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

void setup()
{
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting to Wifi . . .");
  }

  Serial.println("Connected to the Wifi Network");
  Serial.println("Wifi Connected");
  Serial.println("IP Adress");
  Serial.println(WiFi.localIP());
  
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
   // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);

  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}

void loop()
{
  // put your main code here, to run repeatedly:
  // Serial.print("x\t");
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(1000);
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(1000);

  publishMessage();
  client.loop();
  delay(10000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}