const int AO_Pin=0; // Connect the AO of MQ-4 sensor with analog channel 0 pin (A0) of Arduino
const int DO_Pin=8; // Connect the DO of MQ-4 sensor with digital pin 8 (D8) of Arduino
const int LED_Pin=13; // Connect an LED with D13 pin of Arduino
int threshold; // Create a variable to store the digital output of the MQ-4 sensor
int AO_Out; // Create a variable to store the analog output of the MQ-4 sensor
 
// Set up
void setup() {
Serial.begin(115200);  // Initialize serial monitor using a baud rate of 115200
pinMode(DO_Pin, INPUT); // Set the D8 pin as a digital input pin
pinMode(LED_Pin, OUTPUT); //Set the D13 pin as a digital output pin
}
 
// Main loop
void loop()
{
AO_Out= analogRead(AO_Pin); // Read the analog output measurement sample from the MQ4 sensor's AO pin
threshold = digitalRead(DO_Pin); // Read the digital output of MQ-4 sensor's DO pin
Serial.print("Threshold: "); // Print out the text "Threshold: "
Serial.print(threshold);// Print the threshold reached - with will either print be LOW or HIGH (above or underneath)
Serial.print(", "); // print a comma and space
 
Serial.print("Methane Conentration: "); // Print out the text "Methane Concentration: "
Serial.println(AO_Out); // Print out the methane value - the analog output - beteewn 0 and 1023
 
if (threshold == HIGH){
//if (AO_Out >= 200){
digitalWrite(LED_Pin, HIGH);// If the threshold is reached, then the LED lights up to indicate a higher concentration 
}
else{
digitalWrite(LED_Pin, LOW);// If the threshold is not reached (stays at 0), the LED stays off
}
delay(1000); // Set a 10 second delay
}