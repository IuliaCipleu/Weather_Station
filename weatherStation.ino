#include "DHT.h"

#define DHTPIN 7       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22  // Type of DHT sensor (DHT22)
#define BLUE 10
#define GREEN 11
#define RED 12
//#define YELLOW 13
#define BUZZER 6
const int AO_Pin = 7;  // Connect the AO of MQ-4 sensor with analog channel 0 pin (A0) of Arduino
const int DO_Pin = 8;  // Connect the DO of MQ-4 sensor with digital pin 8 (D8) of Arduino
int threshold;         // Create a variable to store the digital output of the MQ-4 sensor
int methaneGas;        // Create a variable to store the analog output of the MQ-4 sensor
const int sensorLight = A0;
#define DEBUG true

DHT dht(DHTPIN, DHTTYPE);

float temperatureThresholdLow = 19.0;
float temperatureThresholdHigh = 22.0;

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  Serial1.print(command);  // send command to the esp8266
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (Serial1.available()) {
      char c = Serial1.read();  // read next char
      response += c;
    }
  }
  if (response.indexOf("/l0") != -1) {
    Serial.println("Turning LED ON");
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (response.indexOf("/l1") != -1) {
    Serial.println("Turning LED OFF");
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (debug) {
    Serial.print(response);
  }
  return response;
}
float readTemperature() {
  //return millis();
  //delay(2000);  // Wait for 2 seconds between readings

  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("%  Temperature: ");
  Serial.println(temperature);

  if (temperature < temperatureThresholdLow) {
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
  }
  if (temperature >= temperatureThresholdLow && temperature < temperatureThresholdHigh) {
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
  }
  if (temperature >= temperatureThresholdHigh) {
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, HIGH);
  }
  return temperature;
}
float readHumidity() {
  //return millis();
  //delay(2000);  // Wait for 2 seconds between readings

  float humidity = dht.readHumidity();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);

  return humidity;
}

float readMethaneGas() {
  methaneGas = analogRead(AO_Pin);  // Read the analog output measurement sample from the MQ4 sensor's AO pin
  threshold = digitalRead(DO_Pin);  // Read the digital output of MQ-4 sensor's DO pin
  Serial.print("Threshold: ");      // Print out the text "Threshold: "
  Serial.print(threshold);          // Print the threshold reached - with will either print be LOW or HIGH (above or underneath)
  Serial.print(", ");               // print a comma and space

  Serial.print("Methane Concentration: ");  // Print out the text "Methane Concentration: "
  Serial.println(methaneGas);               // Print out the methane value - the analog output - beteewn 0 and 1023

  if (threshold == HIGH) {
    //if (AO_Out >= 200){
    //digitalWrite(YELLOW, HIGH);  // If the threshold is reached, then the LED lights up to indicate a higher concentration
    digitalWrite(BUZZER, HIGH);
  } else {
    //digitalWrite(YELLOW, LOW);  // If the threshold is not reached (stays at 0), the LED stays off
    digitalWrite(BUZZER, LOW);
  }
  //delay(1000);  // Set a 10 second delay
  return methaneGas;
}

float readLight() {
  int sensorValue = analogRead(sensorLight);

  // Map the sensor value to a range (optional)
  int mappedValue = map(sensorValue, 0, 1023, 0, 100);  // Adjust the output range as needed

  // Print the values to the Serial monitor
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print("  Mapped Value: ");
  Serial.println(mappedValue);
  return mappedValue;
}

String getValue(String data, String key) {
  String startKey = key;
  String endKey = "&";
  int startIndex = data.indexOf(startKey);
  if (startIndex == -1) {
    return "";
  }
  int endIndex = data.indexOf(endKey, startIndex);
  if (endIndex == -1) {
    endIndex = data.length();
  }
  return data.substring(startIndex + startKey.length(), endIndex);
}

void checkClientCommands() {
  float temperature, humidity, methaneGas, light = 0.0;
  temperature = readTemperature();
  humidity = readHumidity();
  methaneGas = readMethaneGas();
  light = readLight();
  if (Serial1.available()) {
    if (Serial1.find("+IPD,")) {
      delay(500);
      int connectionId = Serial1.read() - 48;
      String webpage = "<h1>Weather Station App</h1>";
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";

      // Check for commands in the URL parameters
      if (Serial1.find("thresholdLow=")) {
        temperatureThresholdLow = Serial1.parseFloat();
      }

      if (Serial1.find("thresholdHigh=")) {
        temperatureThresholdHigh = Serial1.parseFloat();
      }

      // Add sensor data to the webpage
      webpage += "<h2>Temperature:</h2>";
      webpage += temperature;
      webpage += " C";
      webpage += "<h2>Humidity:</h2>";
      webpage += humidity;
      webpage += "%";
      webpage += "<h2>Methane gas:</h2>";
      webpage += methaneGas;
      webpage += " ppm";
      webpage += "<h2>Luminosity:</h2>";
      webpage += light;
      webpage += " lx";

      // Add temperature threshold to the webpage
      webpage += "<h2>Temperature Lower Threshold:</h2>";
      webpage += temperatureThresholdLow;
      webpage += " C";

      // Add buttons to change temperature threshold
      webpage += "<h2>Change Temperature Lower Threshold:</h2>";
      webpage += "<a href=\"/setThresholdLow?value=20.0\"><button>20.0</button></a>";
      webpage += "<a href=\"/setThresholdLow?value=22.0\"><button>22.0</button></a>";
      webpage += "<a href=\"/setThresholdLow?value=25.0\"><button>25.0</button></a>";

      // Add temperature threshold to the webpage
      webpage += "<h2>Temperature Higher Threshold:</h2>";
      webpage += temperatureThresholdHigh;
      webpage += " C";

      // Add buttons to change temperature threshold
      webpage += "<h2>Change Temperature Higher Threshold:</h2>";
      webpage += "<a href=\"/setThresholdHigh?value=20.0\"><button>20.0</button></a>";
      webpage += "<a href=\"/setThresholdHigh?value=22.0\"><button>22.0</button></a>";
      webpage += "<a href=\"/setThresholdHigh?value=25.0\"><button>25.0</button></a>";

      // Send the webpage to the client
      cipSend += webpage.length();
      cipSend += "\r\n";
      sendData(cipSend, 100, DEBUG);
      sendData(webpage, 150, DEBUG);

      // Close the connection
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId;
      closeCommand += "\r\n";
      sendData(closeCommand, 300, DEBUG);
    }
  }
}
void setup() {
  //Serial.begin(115200);
  Serial1.begin(115200);
  Serial1.setTimeout(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  sendData("AT+RST\r\n", 2000, false);  // reset module
  // Change SSID and password
  sendData("AT+CWJAP=\"WeatherStation\",\"nuPl0uaAzi\"\r\n", 2000, false);
  sendData("AT+CWMODE=2\r\n", 1000, false);  // configure as
  //access point
  sendData("AT+CIFSR\r\n", 1000, DEBUG);   // get ip address
  sendData("AT+CWSAP?\r\n", 2000, DEBUG);  // get SSID info
  //(network name)
  sendData("AT+CIPMUX=1\r\n", 1000, false);  // configure for
  //multiple connections
  sendData("AT+CIPSERVER=1,80\r\n", 1000, false);  // turn on
  //server on port 80

  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(DO_Pin, INPUT);  // Set the D8 pin as a digital input pin
  //pinMode(YELLOW, OUTPUT);  //Set the D13 pin as a digital output pin
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BLUE, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(BUZZER, LOW);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float temperature, humidity, methaneGas, light = 0.0;
  temperature = readTemperature();
  humidity = readHumidity();
  methaneGas = readMethaneGas();
  light = readLight();

  if (Serial1.available()) {
    if (Serial1.find("+IPD,")) {
      delay(500);
      int connectionId = Serial1.read() - 48;  // read()
                                               //function returns
                                               // ASCII decimal value and 0 (the first decimal number) starts at 48

      String request = Serial1.readStringUntil('\r');
      if (request.indexOf("/l0") != -1) {
        Serial.println("Turning LED ON");
        digitalWrite(LED_BUILTIN, HIGH);
      } else if (request.indexOf("/l1") != -1) {
        Serial.println("Turning LED OFF");
        digitalWrite(LED_BUILTIN, LOW);
      } else if (request.indexOf("/setThresholdLow") != -1) {
        String thresholdValueLowStr = getValue(request, "value=");
        if (thresholdValueLowStr != "") {
          temperatureThresholdLow = thresholdValueLowStr.toFloat();
          Serial.print("Temperature lower threshold updated to: ");
          Serial.println(temperatureThresholdLow);
        }
      } else if (request.indexOf("/setThresholdHigh") != -1) {
        String thresholdValueHighStr = getValue(request, "value=");
        if (thresholdValueHighStr != "") {
          temperatureThresholdHigh = thresholdValueHighStr.toFloat();
          Serial.print("Temperature higher threshold updated to: ");
          Serial.println(temperatureThresholdHigh);
        }

        String webpage = "<h1>Weather Station App</h1>";
        String cipSend = "AT+CIPSEND=";
        cipSend += connectionId;
        cipSend += ",";
        if (temperature > 0) {
          webpage += "<h2>Temperature:</h2>";
          webpage += temperature;
          webpage += " C";
        }
        if (humidity > 0) {
          webpage += "<h2>Humidity:</h2>";
          webpage += humidity;
          webpage += "%";
        }
        if (methaneGas > 0) {
          webpage += "<h2>Methane gas:</h2>";
          webpage += methaneGas;
          webpage += " ppm";
        }
        if (light > 0) {
          webpage += "<h2>Luminosity:</h2>";
          webpage += light;
          webpage += " lx";
        }
        webpage += "<h2>Do you want to light on a led?</h2>";
        webpage += "<a href=\"/l0\"><button>ON</button></a>";
        webpage += "<a href=\"/l1\"><button>OFF</button></a>";
        webpage += "<h2>Set Temperature Lower Threshold:</h2>";
        webpage += "<form action=\"/setThresholdLow\" method=\"GET\">";
        webpage += "<label for=\"thresholdLow\">Threshold Lower Value:</label>";
        webpage += "<input type=\"text\" id=\"thresholdLow\" name=\"value\">";
        webpage += "<input type=\"submit\" value=\"Set\">";
        webpage += "</form>";
        webpage += "<h2>Set Temperature Higher Threshold:</h2>";
        webpage += "<form action=\"/setThresholdHigh\" method=\"GET\">";
        webpage += "<label for=\"threshold\">Threshold Higher Value:</label>";
        webpage += "<input type=\"text\" id=\"thresholdHigh\" name=\"value\">";
        webpage += "<input type=\"submit\" value=\"Set\">";
        webpage += "</form>";

        cipSend += webpage.length();
        cipSend += "\r\n";
        sendData(cipSend, 100, DEBUG);
        sendData(webpage, 150, DEBUG);
        String closeCommand = "AT+CIPCLOSE=";
        closeCommand += connectionId;  // append connection id
        closeCommand += "\r\n";
        sendData(closeCommand, 300, DEBUG);
      }
    }
  }
}
