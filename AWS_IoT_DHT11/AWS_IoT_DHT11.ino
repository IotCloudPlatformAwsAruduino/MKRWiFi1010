/*
  AWS IoT WiFi

  This sketch securely connects to an AWS IoT using MQTT over WiFi.
  It uses a private key stored in the ATECC508A and a public
  certificate for SSL/TLS authetication.

  It publishes a message every 5 seconds to arduino/outgoing
  topic and subscribes to messages on the arduino/incoming
  topic.

  The circuit:
  - Arduino MKR WiFi 1010 or MKR1000

  The following tutorial on Arduino Project Hub can be used
  to setup your AWS account and the MKR board:

  https://create.arduino.cc/projecthub/132016/securely-connecting-an-arduino-mkr-wifi-1010-to-aws-iot-core-a9f365

  This example code is in the public domain.
*/
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h> // change to #include <WiFi101.h> for MKR1000
#include <Servo.h>

#include "arduino_secrets.h"

#include "DHT.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

#define LED_1_PIN 5

#include <ArduinoJson.h>
#include "Led.h"

/////// Enter your sensitive data in arduino_secrets.h
const char ssid[]        = SECRET_SSID;
const char pass[]        = SECRET_PASS;
const char broker[]      = SECRET_BROKER;
const char* certificate  = SECRET_CERTIFICATE;

WiFiClient    wifiClient;            // Used for the TCP socket connection
BearSSLClient sslClient(wifiClient); // Used for SSL/TLS connection, integrates with ECC508
MqttClient    mqttClient(sslClient);

unsigned long lastMillis = 0;
// 조도 센서 : 에어컨 전원 on/off 를 감지 
int lightSensor = A1;
int light; // 값 저장

// 서보 모터 
Servo servo; 
int motor = 3; 
int angle = 0; // 초기 각도값 설정

// 습도 온도 
float h;
float t;

Led led1(LED_1_PIN);

void setup() {
  Serial.begin(115200);
  servo.attach(motor); // 서보 모터 초기화
  while (!Serial);

  dht.begin();

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

  // Set a callback to get the current time
  // used to validate the servers certificate
  ArduinoBearSSL.onGetTime(getTime);

  // Set the ECCX08 slot to use for the private key
  // and the accompanying public certificate for it
  sslClient.setEccSlot(0, certificate);

  // Optional, set the client id used for MQTT,
  // each device that is connected to the broker
  // must have a unique client id. The MQTTClient will generate
  // a client id for you based on the millis() value if not set
  //
  // mqttClient.setId("clientId");

  // Set the message callback, this function is
  // called when the MQTTClient receives a message
  mqttClient.onMessage(onMessageReceived);

  
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    connectMQTT();
  }

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();

  // publish a message roughly every 5 seconds.
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    char payload[512];
    getDeviceStatus(payload);
    sendMessage(payload);
  }
}

unsigned long getTime() {
  // get the current time from the WiFi module  
  return WiFi.getTime();
}

void connectWiFi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid);
  Serial.print(" ");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the network");
  Serial.println();
}

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(broker);
  Serial.println(" ");

  while (!mqttClient.connect(broker, 8883)) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe("$aws/things/MyMKRWiFi1010/shadow/update/delta");
}

void getDeviceStatus(char* payload) { // 디바이스 상태 값 가져와서 payload 설정
  // Read temperature as Celsius (the default)
  t = dht.readTemperature(); // 온도 값 읽어오기
  h = dht.readHumidity(); // 습도 값 읽기 
  
  light = analogRead(lightSensor); // 조도센서 값 읽어오기
  Serial.print("light : ");
  Serial.println(light);
  // Read led status
  const char* led = (led1.getState() == LED_ON)? "ON" : "OFF";
  const char* airCondStatus = (light > 100 )? "ON" : "OFF";

  
  // make payload for the device update topic ($aws/things/MyMKRWiFi1010/shadow/update)
  sprintf(payload,"{\"state\":{\"reported\":{\"temperature\":\"%0.2f\",\"humidity\":\"%0.2f\",\"airCondStatus\":\"%s\",\"LED\":\"%s\"}}}",t,h,airCondStatus,led);
//    sprintf(payload,"{\"state\":{\"reported\":{\"temperature\":\"%0.2f\",\"LED\":\"%s\"}}}",t,led);
  
  if(strcmp(led,"ON")==0){ // 스마트 에어컨 시스템 ON 일때만 
    
  
    if(((h > 40.0 || t > 23.0) && (strcmp(airCondStatus,"OFF") ==0)) ){ //에어컨 켜지는 조건
      
      for(int i=0; i<100; i++){ // 에어컨의 전원이 ON 일 때 까지
        angle = angle + 1 ;
//        if(strcmp(airCondStatus, "ON") == 0 ) // 에어컨이 켜지면 
//          break; // 모터 움직이기 끝
        if(angle >= 100) // 모터 망가짐 방지 
           angle = 100; // 정지 
        servo.write(angle); 
        delay(10);
      }

      for(int i=0; i< 100 ;i++){ // 모터 위치 원상 복구
        angle = angle - 1; 
        if(angle <=0 ){ // 초기 값으로 돌아오면
          angle = 0 ; // 고정
//          break; // 탈출
        }
        servo.write(angle); // servo1을 angle1 값에 맞추어 동작
        delay(10);
      }
      
    }else if((h <= 40.0 && t <= 29.0) && (strcmp(airCondStatus,"ON"))==0 ){ //에어컨 꺼지는 조건

      for(int i=0; i<100; i++){ // 에어컨의 전원이 OFF 일 때 까지
        angle = angle + 1 ;
//        if(strcmp(airCondStatus, "ON") == 0 ) // 에어컨이 켜지면 
//          break; // 모터 움직이기 끝
        if(angle >= 100) // 모터 망가짐 방지 
           angle = 100; // 정지 
        servo.write(angle); 
        delay(10);
      }

      for(int i=0; i< 100 ;i++){ // 모터 위치 원상 복구
        angle = angle - 1; 
        if(angle <=0 ){ // 초기 값으로 돌아오면
          angle = 0 ; // 고정
//          break; // 탈출
        }
        servo.write(angle); // servo1을 angle1 값에 맞추어 동작
        delay(10);
      
        }
    }
    
  }
      
    
  
}

void sendMessage(char* payload) {
  char TOPIC_NAME[]= "$aws/things/MyMKRWiFi1010/shadow/update";
  
  Serial.print("Publishing send message:");
  Serial.println(payload);
  mqttClient.beginMessage(TOPIC_NAME);
  mqttClient.print(payload);
  mqttClient.endMessage();
}


void onMessageReceived(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // store the message received to the buffer
  char buffer[512] ;
  int count=0;
  while (mqttClient.available()) {
     buffer[count++] = (char)mqttClient.read();
  }
  buffer[count]='\0'; // 버퍼의 마지막에 null 캐릭터 삽입
  Serial.println(buffer);
  Serial.println();

  // JSon 형식의 문자열인 buffer를 파싱하여 필요한 값을 얻어옴.
  // 디바이스가 구독한 토픽이 $aws/things/MyMKRWiFi1010/shadow/update/delta 이므로,
  // JSon 문자열 형식은 다음과 같다.
  // {
  //    "version":391,
  //    "timestamp":1572784097,
  //    "state":{
  //        "LED":"ON"
  //    },
  //    "metadata":{
  //        "LED":{
  //          "timestamp":15727840
  //         }
  //    }
  // }
  //
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, buffer);
  JsonObject root = doc.as<JsonObject>();
  JsonObject state = root["state"];
  const char* led = state["LED"];
  Serial.println(led);
  
  char payload[512];
  const char* airCondStatus = (light > 100 )? "ON" : "OFF";

  if (strcmp(led,"ON")==0) {
    led1.on();
    sprintf(payload,"{\"state\":{\"reported\":{\"LED\":\"%s\"}}}","ON");
    sendMessage(payload);
        
    
  } else if (strcmp(led,"OFF")==0) {
    led1.off();
    sprintf(payload,"{\"state\":{\"reported\":{\"LED\":\"%s\"}}}","OFF");
    sendMessage(payload);
  }
 
}
