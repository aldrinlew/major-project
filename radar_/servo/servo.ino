#include <Servo.h>  // Include the Servo library

Servo servo;  // Create a servo object to control a continuous rotation servo

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  servo.attach(9);     // Attach the servo to pin 9
}

void loop() {
  left();       // Call the function to rotate the servo left
  delay(1000);  // Delay for 1 second
  stopServo();  // Stop the servo
  delay(500);   // Delay for 0.5 seconds
  right();      // Call the function to rotate the servo right
  delay(1000);  // Delay for 1 second
  stopServo();  // Stop the servo
  delay(500);   // Delay for 0.5 seconds
}

void left() {
  servo.write(0);  // Rotate left (counterclockwise at maximum speed)
  Serial.println("Rotating left");
}

void right() {
  servo.write(180);  // Rotate right (clockwise at maximum speed)
  Serial.println("Rotating right");
}

void stopServo() {
  servo.write(90);  // Stop the servo
  Serial.println("Servo stopped");
}
