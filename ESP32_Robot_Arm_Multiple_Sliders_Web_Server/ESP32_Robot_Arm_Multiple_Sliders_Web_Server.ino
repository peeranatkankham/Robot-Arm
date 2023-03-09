#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <Servo.h>

static const int servoPin[7] = {13,12,14,27,26,25,33};

// Replace with your network credentials
const char* ssid = "Peeranat";
const char* password = "peeranatk88";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
// Create a WebSocket object

AsyncWebSocket ws("/ws");
// Set LED GPIO

String message = "";
String sliderValue1 = "150";
String sliderValue2 = "180";
String sliderValue3 = "70";
String sliderValue4 = "90";
String sliderValue5 = "90";
String sliderValue6 = "90";
String sliderValue7 = "90";

int dutyCycle1 = 150;
int dutyCycle2 = 180;
int dutyCycle3 = 70;
int dutyCycle4 = 90;
int dutyCycle5 = 90;
int dutyCycle6 = 90;
int dutyCycle7 = 90;

int pinData[7];
unsigned long last_time = 0;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;

float servo1Smoothed;
float servo1Prev = 150;
float servo2Smoothed;
float servo2Prev = 180;
float servo3Smoothed;
float servo3Prev = 70;
float servo4Smoothed;
float servo4Prev = 90;
float servo5Smoothed;
float servo5Prev = 90;
float servo6Smoothed;
float servo6Prev = 90;
float servo7Smoothed;
float servo7Prev = 90;


//Json Variable to Hold Slider Values
JSONVar sliderValues;

//Get Slider Values
String getSliderValues(){
  sliderValues["sliderValue1"] = String(sliderValue1);
  sliderValues["sliderValue2"] = String(sliderValue2);
  sliderValues["sliderValue3"] = String(sliderValue3);
  sliderValues["sliderValue4"] = String(sliderValue4);
  sliderValues["sliderValue5"] = String(sliderValue5);
  sliderValues["sliderValue6"] = String(sliderValue6);
  sliderValues["sliderValue7"] = String(sliderValue7);

  String jsonString = JSON.stringify(sliderValues);
  return jsonString;
}

// Initialize SPIFFS
void initFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else{
   Serial.println("SPIFFS mounted successfully");
  }
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void notifyClients(String sliderValues) {
  ws.textAll(sliderValues);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    message = (char*)data;
    if (message.indexOf("1s") >= 0) {
      sliderValue1 = message.substring(2);
      dutyCycle1 = sliderValue1.toInt();
      Serial.println(dutyCycle1);
      Serial.print(getSliderValues());
      notifyClients(getSliderValues());
    }
    if (message.indexOf("2s") >= 0) {
      sliderValue2 = message.substring(2);
      dutyCycle2 = sliderValue2.toInt();
      Serial.println(dutyCycle2);
      Serial.print(getSliderValues());
      notifyClients(getSliderValues());
    }    
    if (message.indexOf("3s") >= 0) {
      sliderValue3 = message.substring(2);
      dutyCycle3 = sliderValue3.toInt();
      Serial.println(dutyCycle3);
      Serial.print(getSliderValues());
      notifyClients(getSliderValues());
    }
    if (message.indexOf("4s") >= 0) {
      sliderValue4 = message.substring(2);
      dutyCycle4 = sliderValue4.toInt();
      Serial.println(dutyCycle4);
      Serial.print(getSliderValues());
      notifyClients(getSliderValues());
    }
    if (message.indexOf("5s") >= 0) {
      sliderValue5 = message.substring(2);
      dutyCycle5 = sliderValue5.toInt();
      Serial.println(dutyCycle5);
      Serial.print(getSliderValues());
      notifyClients(getSliderValues());
    }    
    if (message.indexOf("6s") >= 0) {
      sliderValue6 = message.substring(2);
      dutyCycle6 = sliderValue6.toInt();
      Serial.println(dutyCycle6);
      Serial.print(getSliderValues());
      notifyClients(getSliderValues());
    }
    if (message.indexOf("7s") >= 0) {
      sliderValue7 = message.substring(2);
      dutyCycle7 = sliderValue7.toInt();
      Serial.println(dutyCycle7);
      Serial.print(getSliderValues());
      notifyClients(getSliderValues());
    }
    if (strcmp((char*)data, "getValues") == 0) {
      notifyClients(getSliderValues());
    }
    if (strcmp((char*)data, "reset") == 0) {
      sliderValue1 = "150";
      sliderValue2 = "180";
      sliderValue3 = "70";
      sliderValue4 = "90";
      sliderValue5 = "90";
      sliderValue6 = "90";
      sliderValue7 = "90";
      dutyCycle1 = 150;
      dutyCycle2 = 180;
      dutyCycle3 = 70;
      dutyCycle4 = 90;
      dutyCycle5 = 90;
      dutyCycle6 = 90;
      dutyCycle7 = 90;
      Serial.print(getSliderValues());
      notifyClients(getSliderValues());
    }
  }
}
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}


void setup() {
  Serial.begin(115200);
  servo1.attach(servoPin[0]);
  servo2.attach(servoPin[1]);
  servo3.attach(servoPin[2]);
  servo4.attach(servoPin[3]);
  servo5.attach(servoPin[4]);
  servo6.attach(servoPin[5]);
  servo7.attach(servoPin[6]);

  servo1.write(150);
  servo2.write(180);
  servo3.write(70);
  servo4.write(90);
  servo5.write(90);
  servo6.write(90);
  servo7.write(90);

  
  initFS();
  initWiFi();
  initWebSocket();
  
  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  server.serveStatic("/", SPIFFS, "/");

  // Start server
  
  server.begin();
  
}

void loop() {

  if( millis() - last_time > 5) {
    last_time = millis();
    servo1Smoothed = (dutyCycle1 * 0.02) + (servo1Prev * 0.98);
    servo2Smoothed = (dutyCycle2 * 0.02) + (servo2Prev * 0.98);
    servo3Smoothed = (dutyCycle3 * 0.02) + (servo3Prev * 0.98);
    servo4Smoothed = (dutyCycle4 * 0.02) + (servo4Prev * 0.98);
    servo5Smoothed = (dutyCycle5 * 0.02) + (servo5Prev * 0.98);
    servo6Smoothed = (dutyCycle6 * 0.02) + (servo6Prev * 0.98);
    servo7Smoothed = (dutyCycle7 * 0.02) + (servo7Prev * 0.98);
    
  
    servo1Prev = servo1Smoothed;
    servo2Prev = servo2Smoothed;
    servo3Prev = servo3Smoothed;
    servo4Prev = servo4Smoothed;
    servo5Prev = servo5Smoothed;
    servo6Prev = servo6Smoothed;
    servo7Prev = servo7Smoothed;
    
    servo1.write(servo1Smoothed);
    servo2.write(servo2Smoothed); 
    servo3.write(servo3Smoothed); 
    servo4.write(servo4Smoothed); 
    servo5.write(servo5Smoothed); 
    servo6.write(servo6Smoothed); 
    servo7.write(servo7Smoothed);  
  }
  ws.cleanupClients();
}
