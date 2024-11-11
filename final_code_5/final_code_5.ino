// Constants
#define Vref 3.3  // Nano RP2040 Connect operates at 3.3V
#define tdsPin A2
#define ONE_WIRE_BUS 2
#define RX_PIN 1
#define TX_PIN 0

// Global Variables
unsigned long int avgValue;
float pH, tdsValue, Voltage, volt, ntu, Distance;
int sensorValue;

// Libraries
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Objects
Servo servo;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Setup Function
void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  servo.attach(9);
  sensors.begin();
  Serial1.begin(9600); // A02YYUW typically uses 9600 baud rate
  delay(100); // Ensure sensor initialization
}

// Loop Function
void loop() {
  readPH();
  controlServo();
  readTDS();
  readTemperature();
  readTurbidity();
  readUltrasonic();
}

// PH Function
void readPH() {
  int buf[10];
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(A0);
    delay(10);
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++) {
    avgValue += buf[i];
  }
  float sensorValue = avgValue / 6.0;
  pH = 7 - 1000 * (sensorValue - 541) * Vref / 59.16 / 1023;
  Serial.print("The PH value is: ");
  Serial.println(pH, 2);
  delay(1000);
}

// Servo Control Functions
void controlServo() {
  left();
  delay(1000);
  stopServo();
  delay(500);
  right();
  delay(1000);
  stopServo();
  delay(500);
}

void left() {
  servo.write(0);
}

void right() {
  servo.write(180);
}

void stopServo() {
  servo.write(90);
}

// TDS Function
void readTDS() {
  sensorValue = analogRead(tdsPin);
  Voltage = sensorValue * 5 / 1024.0;
  tdsValue = (133.42 * Voltage * Voltage * Voltage - 255.86 * Voltage * Voltage + 857.39 * Voltage) * 0.5;
  Serial.print("TDS Value = ");
  Serial.print(tdsValue);
  Serial.println(" ppm");
  delay(1000);
}

// Temperature Function
void readTemperature() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print("°C  |  ");
  Serial.print((tempC * 9.0) / 5.0 + 32.0);
  Serial.println("°F");
  delay(500);
}

// Turbidity Function
void readTurbidity() {
  volt = 0;
  for(int i = 0; i < 800; i++) {
    volt += ((float)analogRead(A1) / 1023) * 5;
  }
  volt /= 800;
  ntu = (volt < 2.5) ? 3000 : (volt > 4.2) ? 0 : -1120.4 * sq(volt) + 5742.3 * volt - 4353.8;
  Serial.print(" | Turbidity: ");
  Serial.print(ntu);
  Serial.println(" NTU");
  delay(1000);
}

// Ultrasonic Function
void readUltrasonic() {
  if (Serial1.available() >= 4) {
    uint8_t data[4];
    for (int i = 0; i < 4; i++) {
      data[i] = Serial1.read();
    }
    if (data[0] == 0xFF) {
      uint16_t distance = (data[1] << 8) | data[2];
      Distance = (distance < 7500) ? distance : -1;
      Serial.print("Distance: ");
      Serial.print(Distance);
      Serial.println(" mm");
    }
  }
  delay(100);
}
