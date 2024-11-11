#define Vref 3.3  // Nano RP2040 Connect operates at 3.3V
unsigned long int avgValue;

void setup()
{
    Serial.begin(9600);
    pinMode(A0, INPUT);  // Use A0 for sensor input
}

void loop()
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
    Serial.print("Raw analog value: ");
    Serial.println(sensorValue);

    // Adjust the formula based on sensor calibration
    // The raw analog value for pH 7 is around 541, so we adjust the formula accordingly
    float phValue = 7 - 1000 * (sensorValue - 541) * Vref / 59.16 / 1023;  // Adjusted constant from 361 to 541
    
    // Print the calculated pH value
    Serial.print("The PH value is: ");
    Serial.println(phValue, 2);  // Print the pH value with 2 decimal places

    delay(1000);
}
