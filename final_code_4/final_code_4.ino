//ph intiallization
#define Vref 3.3  // Nano RP2040 Connect operates at 3.3V 
unsigned long int avgValue;
float pH;
//servo initiallization
#include <Servo.h>  // Include the Servo library
Servo servo;  // Create a servo object to control a continuous rotation servo
//tds initiallization
#define SERIAL Serial
#define tdsPin A2
int sensorValue = 0;
float tdsValue = 0;
float Voltage = 0;
//temperature initiallization
#include <OneWire.h>
#include <DallasTemperature.h>
  // Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 2
  // Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  
  // Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);
//turbidity initiallization
int sensorPin = A1;
float volt;
float ntu;
//ultrasonic initiallization
#define RX_PIN 1  // RX pin of the A02YYUW connected to TX1 (D1) of Arduino Nano RP2040 Connect
#define TX_PIN 0  // TX pin of the A02YYUW connected to RX1 (D0) of Arduino Nano RP2040 Connect
float Distance;

void setup()
{
    //ph setup
    Serial.begin(115200);
    pinMode(A0, INPUT);  // Use A0 for sensor input
    //servo setup
    servo.attach(9);     // Attach the servo to pin 9
    //tds setup
    //temperature setup
    sensors.begin();  // Start up the library
    //turbidity setup
    //ultrasonic setup
     // Start serial communication with the computer
     // Start serial communication with the ultrasonic sensor
    Serial1.begin(9600); // A02YYUW typically uses 9600 baud rate
      // Small delay to ensure sensor initialization
    delay(100);
       
}

void loop()
{
  //ph loop
  {
    float sensorValue;
    int buf[10];
    
    // Read 10 samples from the sensor for smoothing
    for (int i = 0; i < 10; i++)  
    {
        buf[i] = analogRead(A0);  // Read sensor value from A0
        delay(10);
    }

    // Sort the buffer values from small to large
    for (int i = 0; i < 9; i++)
    {
        for (int j = i + 1; j < 10; j++)
        {
            if (buf[i] > buf[j])
            {
                int temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }

    avgValue = 0;
    
    // Take the average of the 6 center values for noise reduction
    for (int i = 2; i < 8; i++)
    {
        avgValue += buf[i];
    }

    sensorValue = avgValue / 6.0;  // Calculate the average sensor value

    // Print the raw analog value for monitoring
    //Serial.print("Raw analog value: ");
    //Serial.println(sensorValue);

    // Adjust the formula based on sensor calibration
    // The raw analog value for pH 7 is around 541, so we adjust the formula accordingly
    float phValue = 7 - 1000 * (sensorValue - 541) * Vref / 59.16 / 1023;  // Adjusted constant from 361 to 541
    
    // Print the calculated pH value
    Serial.print("The PH value is: ");
    Serial.println(phValue, 2);  // Print the pH value with 2 decimal places
    pH=phValue;

    delay(1000);
}
  //servo loop
{
  left();       // Call the function to rotate the servo left
  delay(1000);  // Delay for 1 second
  stopServo();  // Stop the servo
  delay(500);   // Delay for 0.5 seconds
  right();      // Call the function to rotate the servo right
  delay(1000);  // Delay for 1 second
  stopServo();  // Stop the servo
  delay(500);   // Delay for 0.5 seconds
}
  //tds loop
{
  sensorValue = analogRead(tdsPin);
  Voltage = sensorValue*5/1024.0; //Convert analog reading to Voltage
  tdsValue=(133.42*Voltage*Voltage*Voltage - 255.86*Voltage*Voltage + 857.39*Voltage)*0.5; //Convert voltage value to TDS value
  Serial.print("TDS Value = "); 
  Serial.print(tdsValue);
  Serial.println(" ppm");
  delay(1000);
 }
  //temperature loop
 {
    // Send the command to get temperatures
  sensors.requestTemperatures(); 
  
  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print("°C  |  ");
  
  //print the temperature in Fahrenheit
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.println("°F");
  
  delay(500);
 }
 //turbidity loop
 {
  // Initialize the voltage variable
  volt = 0;

  // Sum the voltage readings from the turbidity sensor
  for(int i = 0; i < 800; i++)
  {
      volt += ((float)analogRead(sensorPin) / 1023) * 5;
  }

  // Average the voltage
  volt = volt / 800;

  // Calculate turbidity (NTU) based on voltage
  if(volt < 2.5)
  {
      ntu = 3000;
  }
  else if(volt>4.2)
  {
      ntu = 0;
  }
  else
  {
      ntu = -1120.4 * sq(volt) + 5742.3 * volt - 4353.8;
  }

  // Print the voltage and NTU value to the Serial Monitor
  //Serial.print("Voltage: ");
  //Serial.print(volt);
  //Serial.print(" V");

  Serial.print(" | Turbidity: ");
  Serial.print(ntu);
  Serial.println(" NTU");

  delay(1000); // Delay to make the readings more readable
}
  //ultrasonic loop
{
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
}



//servo declaration
void left() {
  servo.write(0);  // Rotate left (counterclockwise at maximum speed)
}

void right() {
  servo.write(180);  // Rotate right (clockwise at maximum speed)
}

void stopServo() {
  servo.write(90);  // Stop the servo
}
