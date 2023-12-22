// Pin configuration
const int sensorPin = A0; // Connect the OUT pin of the light sensor to analog pin A0

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
}

void loop() {
  // Read the analog value from the light sensor
  int sensorValue = analogRead(sensorPin);

  // Map the sensor value to a range (optional)
  int mappedValue = map(sensorValue, 0, 1023, 0, 100); // Adjust the output range as needed

  // Print the values to the Serial monitor
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print("  Mapped Value: ");
  Serial.println(mappedValue);

  // Add a delay before the next reading
  delay(1000);
}
// mergeeeeeee