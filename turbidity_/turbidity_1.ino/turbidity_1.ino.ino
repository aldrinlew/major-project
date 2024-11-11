int sensorPin = A1;
float volt;
float ntu;

void setup()
{
  Serial.begin(9600); // Initialize Serial Monitor at 9600 baud rate
}

void loop()
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
    Serial.print("Voltage: ");
    Serial.print(volt);
    Serial.print(" V");

    Serial.print(" | Turbidity: ");
    Serial.print(ntu);
    Serial.println(" NTU");

    delay(1000); // Delay to make the readings more readable
}
