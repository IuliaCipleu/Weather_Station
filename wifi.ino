 #include <SoftwareSerial.h>

SoftwareSerial esp8266(18, 19); // RX, TX

void setup() {
  Serial.begin(9600);
  esp8266.begin(9600);
  pinMode(12, OUTPUT); // RST pin
  digitalWrite(12, HIGH); // Keep ESP8266 out of reset state
}

void loop() {
  // Your main code here

  if (esp8266.available()) {
    Serial.write(esp8266.read());
  }

  if (Serial.available()) {
    esp8266.write(Serial.read());
  }
}
