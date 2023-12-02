#include "DHT.h"

#define DHTPIN 7       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22  // Type of DHT sensor (DHT22)
#define BLUE 10
#define GREEN 11
#define RED 12

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  digitalWrite(BLUE, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);  // Wait for 2 seconds between readings

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.println(temperature);

  if (temperature < 19.0) {
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
  }
  if (temperature >= 19.0 && temperature < 22.0) {
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
  }
  if (temperature >= 22.0) {
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, HIGH);
  }
}
