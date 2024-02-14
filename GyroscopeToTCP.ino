//Code for receiving gyroscope data from Arduino Nano 33 IoT and sending it over TCP.
//Code for gyroscope from Arduino documentation https://docs.arduino.cc/tutorials/nano-33-iot/imu-gyroscope/
//Code for sending data over TCP from example code provided in class

#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>

//Setting up variables for sending data over TCP
WiFiClient client;

const char server[] = "192.168.1.88";  //local IP address of receiver device goes here
const int portNum = 7194;  //desired port # goes here. Make sure the receiver is listening on the same port!

//be sure to remove WiFi network details before uploading this code!
const char WIFI_SSID[] = "FiOS-YD974";  //WiFi network name goes here
const char WIFI_PASS[] = "boy6768win7934burn";  //WiFi password goes here


//Setting up variables for gyroscope data
float x, y, z;
int plusThreshold = 30, minusThreshold = -30;


void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Started");

  //retry connection until WiFi is connected successfully
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to SSID: ");
    // Attempt connection to WPA/WPA2 network.
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    delay(3000);
  }
  Serial.println("connected!");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
}


void loop() {
  //connect to client if disconnected, or send TCP message if conected
  if (!client.connected()) {
    Serial.println("connecting");
    client.connect(server, portNum);
    delay(1000);
    return;
  } else {
    calculateCollision();
    delay(50);
  }
}

void calculateCollision() {
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
  }
  if (y > plusThreshold) {
    Serial.println("Sending: Collision front+String(millis())");
    client.println("Collision front_" + String(millis()));
    delay(500);
  }
  if (y < minusThreshold) {
    Serial.println("Sending: Collision back"+String(millis()));
    client.println("Collision back_" + String(millis()));
    delay(500);
  }
  if (x < minusThreshold) {
    Serial.println("Sending: Collision right"+String(millis()));
    client.println("Collision right_" + String(millis()));
    delay(500);
  }
  if (x > plusThreshold) {
    Serial.println("Sending: Collision left_" + String(millis()));
    client.println("Collision left_" +String(millis()));
    delay(500);
  }
}