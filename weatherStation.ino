#include "DHT.h"

#define DHTPIN 7       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22  // Type of DHT sensor (DHT22)
#define BLUE 10
#define GREEN 11
#define RED 12
#define BUZZER 6
const int AO_Pin = 7;  // Connect the AO of MQ-4 sensor with analog channel 0 pin (A0) of Arduino
const int DO_Pin = 8;  // Connect the DO of MQ-4 sensor with digital pin 8 (D8) of Arduino
int threshold;         // Create a variable to store the digital output of the MQ-4 sensor
int methaneGas;        // Create a variable to store the analog output of the MQ-4 sensor
const int sensorLight = A0;
#define DEBUG true

DHT dht(DHTPIN, DHTTYPE);

float temperatureThreshold = 19.0;      // Initial temperature threshold
float temperatureThresholdHigh = 22.0;  // Initial higher temperature threshold


void setup() {
  Serial1.begin(115200);
  Serial1.setTimeout(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  sendData("AT+RST\r\n", 2000, false);  // reset module
  sendData("AT+CWJAP=\"WeatherStation\",\"nuPl0uaAzi\"\r\n", 2000, false);
  sendData("AT+CWMODE=2\r\n", 1000, false);
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  sendData("AT+CWSAP?\r\n", 2000, DEBUG);
  sendData("AT+CIPMUX=1\r\n", 1000, false);
  sendData("AT+CIPSERVER=1,80\r\n", 1000, false);

  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(DO_Pin, INPUT);
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
      int connectionId = Serial1.read() - 48;

      String request = Serial1.readStringUntil('\r');
      if (request.indexOf("/l0") != -1) {
        Serial.println("Turning LED ON");
        digitalWrite(LED_BUILTIN, HIGH);
      } else if (request.indexOf("/l1") != -1) {
        Serial.println("Turning LED OFF");
        digitalWrite(LED_BUILTIN, LOW);
      } else if (request.indexOf("/setThresholdLow") != -1) {
        String thresholdValueStr = getValue(request, "value=");
        if (thresholdValueStr != "") {
          temperatureThreshold = thresholdValueStr.toFloat();
          if (temperatureThreshold > temperatureThresholdHigh) {
            // Swap the values
            float temp = temperatureThreshold;
            temperatureThreshold = temperatureThresholdHigh;
            temperatureThresholdHigh = temp;
          }
          Serial.print("Temperature threshold updated to: ");
          Serial.println(temperatureThreshold);
        }
      } else if (request.indexOf("/setThresholdHigh") != -1) {
        String thresholdValueHighStr = getValue(request, "value=");
        if (thresholdValueHighStr != "") {
          temperatureThresholdHigh = thresholdValueHighStr.toFloat();
          if (temperatureThreshold > temperatureThresholdHigh) {
            // Swap the values
            float temp = temperatureThreshold;
            temperatureThreshold = temperatureThresholdHigh;
            temperatureThresholdHigh = temp;
          }
          Serial.print("Higher Temperature threshold updated to: ");
          Serial.println(temperatureThresholdHigh);
        }
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

      webpage += "<h2>Current Lower Temperature Threshold:</h2>";
      webpage += temperatureThreshold;
      webpage += " C";
      webpage += "<h2>Current Higher Temperature Threshold:</h2>";
      webpage += temperatureThresholdHigh;
      webpage += " C";

      webpage += "<h2>Set Lower Temperature Threshold:</h2>";
      webpage += "<form action=\"/setThresholdLow\" method=\"GET\">";
      webpage += "<label for=\"threshold\">Threshold Value:</label>";
      webpage += "<input type=\"text\" id=\"threshold\" name=\"value\">";
      webpage += "<input type=\"submit\" value=\"Set\">";
      webpage += "</form>";
      webpage += "<h2>Set Higher Temperature Threshold:</h2>";
      webpage += "<form action=\"/setThresholdHigh\" method=\"GET\">";
      webpage += "<label for=\"thresholdHigh\">Higher Threshold Value:</label>";
      webpage += "<input type=\"text\" id=\"thresholdHigh\" name=\"value\">";
      webpage += "<input type=\"submit\" value=\"Set\">";
      webpage += "</form>";


      cipSend += webpage.length();
      cipSend += "\r\n";
      sendData(cipSend, 100, DEBUG);
      sendData(webpage, 150, DEBUG);
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId;
      closeCommand += "\r\n";
      sendData(closeCommand, 300, DEBUG);
    }
  }
}

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  Serial1.print(command);
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (Serial1.available()) {
      char c = Serial1.read();
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
  float temperature = dht.readTemperature();
  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.println(temperature);

    if (temperature < temperatureThreshold) {
      digitalWrite(BLUE, HIGH);
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, LOW);
    } else if (temperature >= temperatureThreshold && temperature < temperatureThresholdHigh) {
      digitalWrite(BLUE, LOW);
      digitalWrite(GREEN, HIGH);
      digitalWrite(RED, LOW);
    } else {
      digitalWrite(BLUE, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
    }
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

// void checkClientCommands() {
//   float temperature, humidity, methaneGas, light = 0.0;
//   temperature = readTemperature();
//   humidity = readHumidity();
//   methaneGas = readMethaneGas();
//   light = readLight();

//   if (Serial1.available()) {
//     if (Serial1.find("+IPD,")) {
//       delay(500);
//       int connectionId = Serial1.read() - 48;

//       String request = Serial1.readStringUntil('\r');
//       if (request.indexOf("/l0") != -1) {
//         Serial.println("Turning LED ON");
//         digitalWrite(LED_BUILTIN, HIGH);
//       } else if (request.indexOf("/l1") != -1) {
//         Serial.println("Turning LED OFF");
//         digitalWrite(LED_BUILTIN, LOW);
//       } else if (request.indexOf("/setThresholdLow") != -1) {
//         String thresholdValueStr = getValue(request, "value=");
//         if (thresholdValueStr != "") {
//           temperatureThreshold = thresholdValueStr.toFloat();
//           Serial.print("Temperature threshold updated to: ");
//           Serial.println(temperatureThreshold);
//         }
//       }

//       String webpage = "<h1>Weather Station App</h1>";
//       String cipSend = "AT+CIPSEND=";
//       cipSend += connectionId;
//       cipSend += ",";

//       if (temperature > 0) {
//         webpage += "<h2>Temperature:</h2>";
//         webpage += temperature;
//         webpage += " C";
//       }
//       if (humidity > 0) {
//         webpage += "<h2>Humidity:</h2>";
//         webpage += humidity;
//         webpage += "%";
//       }
//       if (methaneGas > 0) {
//         webpage += "<h2>Methane gas:</h2>";
//         webpage += methaneGas;
//         webpage += " ppm";
//       }
//       if (light > 0) {
//         webpage += "<h2>Luminosity:</h2>";
//         webpage += light;
//         webpage += " lx";
//       }

//       webpage += "<h2>Do you want to light on a led?</h2>";
//       webpage += "<a href=\"/l0\"><button>ON</button></a>";
//       webpage += "<a href=\"/l1\"><button>OFF</button></a>";
//       webpage += "<h2>Set Temperature Threshold:</h2>";
//       webpage += "<form action=\"/setThresholdLow\" method=\"GET\">";
//       webpage += "<label for=\"threshold\">Threshold Value:</label>";
//       webpage += "<input type=\"text\" id=\"threshold\" name=\"value\">";
//       webpage += "<input type=\"submit\" value=\"Set\">";
//       webpage += "</form>";

//       cipSend += webpage.length();
//       cipSend += "\r\n";
//       sendData(cipSend, 100, DEBUG);
//       sendData(webpage, 150, DEBUG);
//       String closeCommand = "AT+CIPCLOSE=";
//       closeCommand += connectionId;
//       closeCommand += "\r\n";
//       sendData(closeCommand, 300, DEBUG);
//     }
//   }
// }
