// Pin definitions
#define RX_PIN 1  // RX pin of the A02YYUW connected to TX1 (D1) of Arduino Nano RP2040 Connect
#define TX_PIN 0  // TX pin of the A02YYUW connected to RX1 (D0) of Arduino Nano RP2040 Connect
float Distance = 0;

void setup() {
  // Start serial communication with the computer
  Serial.begin(115200);
  
  // Start serial communication with the ultrasonic sensor
  Serial1.begin(9600); // A02YYUW typically uses 9600 baud rate

  // Small delay to ensure sensor initialization
  delay(100);
}

void loop() {
  // Request data from the ultrasonic sensor
  if (Serial1.available() >= 4) {
    uint8_t data[4];
    for (int i = 0; i < 4; i++) {
      data[i] = Serial1.read();
    }
    
    if (data[0] == 0xFF) { // Check for valid data frame
      uint16_t distance = (data[1] << 8) | data[2]; // Combine the high and low bytes
      if (distance < 7500) { // Range of the sensor in mm
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" mm");
        Distance = distance;
      } else {
        Serial.println("Out of range");
      }
    }
  }

  delay(100); // Small delay to avoid flooding the sensor with requests
}
