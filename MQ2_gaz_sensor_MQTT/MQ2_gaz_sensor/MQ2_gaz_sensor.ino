//Include librairies
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

//Define SSID and PWD WiFi 
#define SSID "Galaxy S20+ VB"
#define PASSWORD "djhx3915"

//Define MQTT server and init variable for MQTT
#define MQTT_SERVER "192.168.99.90"
WiFiClient espClient;
PubSubClient client(espClient);

//Init Arduino pin
int redLed = D1;
int pinBuzzer = D2;
int pinMQ = A0;
// Threshold value of gaz sensor
int sensorThres = 250;
float analogSensor;

void setup_wifi(){
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Print the IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      // client.subscribe("room/lamp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqtt_client(){
  
  if(!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");
    
  client.publish("gaz_sensor", String(analogSensor).c_str()); 
  // publish light reading every 5 seconds 
    //if(millis() > (time + 5000)) { 
      //time = millis();
      //}
}

void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinMQ, INPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);
}

void loop() {
  analogSensor = analogRead(pinMQ);
  //analogSensorChar = analogSensor;

  mqtt_client();
  
  Serial.print("Gaz sensor value : ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    Serial.println("Smoke detected");
    digitalWrite(redLed, HIGH);
    tone(pinBuzzer, 2850, 500);
    delay(1500);
    tone(pinBuzzer, 2850, 500);
    delay(1500);
    tone(pinBuzzer, 2850, 500);
  }
  else
  {
    digitalWrite(redLed, LOW);
    noTone(pinBuzzer);
  }
  delay(2000);
}
