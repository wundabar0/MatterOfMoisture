/*
   Matter Humidity Sensor Example with Adafruit Soil Moisture Sensor

   This example shows how to create a soil moisture sensor using the Arduino Matter API 
   and Adafruit's I2C-based soil moisture sensor. The soil moisture readings are published 
   as humidity values to a Matter device, which can be integrated into a Matter ecosystem.

   Hardware:
   - Arduino Nano Matter, SparkFun Thing Plus MGM240P, xG24 Explorer Kit, or xG24 Dev Kit.
   - Adafruit Soil Moisture Sensor (seesaw), connected via I2C: A4 = SDA, A5 = SCL.
   
   Functionality:
   - Reads temperature and soil moisture (capacitive reading) from the Adafruit sensor.
   - Converts the raw capacitive reading to a percentage of soil moisture (0% to 100%).
   - Publishes the converted soil moisture as humidity to the Matter ecosystem.
   - The device must be commissioned to a Matter hub to interact with it.

   Calibration:
   - The sensor is calibrated to report 0% humidity at a capacitance of 345 (dry soil) and 
     100% humidity at a capacitance of 1016 (fully submerged in water).

   This example can serve as a starting point for adding more features, such as temperature 
   monitoring, error handling, and other sensors.

   Author: Tamas Jozsi (Silicon Labs) and Brendan Henrich (Symmetry Electronics)
 */

#include <Matter.h>
#include <MatterHumidity.h>
#include "Adafruit_seesaw.h"

// Create instances for the Adafruit seesaw sensor and Matter humidity sensor
Adafruit_seesaw ss;
MatterHumidity matter_humidity_sensor;

// Calibration values from the soil moisture sensor
const uint16_t CAPREAD_MIN = 345;  // Capacitance at 0% humidity (dry soil)
const uint16_t CAPREAD_MAX = 1016; // Capacitance at 100% humidity (fully submerged)

// Helper function to map capacitance reading to humidity percentage
float getHumidityFromCapacitance(uint16_t capread) {
  // Ensure that the value is constrained within 0 to 100
  return constrain(map(capread, CAPREAD_MIN, CAPREAD_MAX, 0, 100), 0.0f, 100.0f);
}

// Function to wait for the device to be commissioned to the Matter network
void waitForMatterCommissioning() {
  Serial.println("Waiting for Matter device commissioning...");
  while (!Matter.isDeviceCommissioned()) {
    delay(200);  // Polling delay
  }
  Serial.println("Device commissioned successfully.");
}

// Function to wait for Thread network connection
void waitForThreadNetworkConnection() {
  Serial.println("Connecting to Thread network...");
  while (!Matter.isDeviceThreadConnected()) {
    delay(200);  // Polling delay
  }
  Serial.println("Connected to Thread network.");
}

void setup()
{
  // Start serial communication for debugging
  Serial.begin(115200);

  // Initialize Matter and the humidity sensor
  Matter.begin();
  matter_humidity_sensor.begin();

  Serial.println("Matter soil moisture sensor: begin!");

  // Initialize the Adafruit soil moisture sensor on I2C (address 0x36)
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! Adafruit seesaw not found.");
    Serial.println("Please check the wiring (A4=SDA, A5=SCL) and ensure the sensor is powered.");
    while (1) delay(1);  // Halt the program if the sensor isn't found
  } else {
    // Print the sensor version if initialization is successful
    Serial.print("Adafruit seesaw started! Version: ");
    Serial.println(ss.getVersion(), HEX);
  }

  // Check if the Matter device is commissioned
  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned.");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code.");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
  }

  // Wait for the device to be commissioned
  waitForMatterCommissioning();

  // Wait for Thread network connection
  waitForThreadNetworkConnection();

  Serial.println("Waiting for Matter device discovery...");
  // Wait until the humidity sensor device is discovered on the Matter network
  while (!matter_humidity_sensor.is_online()) {
    delay(200);
  }
  Serial.println("Matter device is now online.");
}

void loop()
{
  // Variable to hold the current humidity
  float current_humidity = 0.0f;
  static uint32_t last_action = 0;  // Tracks the last action time for 10-second delay

  // Read the soil moisture and temperature from the sensor
  float tempC = ss.getTemp();             // Read temperature in Celsius
  uint16_t capread = ss.touchRead(0);     // Read the capacitive moisture level

  // Convert the raw capacitive reading to a percentage (0% - 100%)
  current_humidity = getHumidityFromCapacitance(capread);

  // Print the temperature and soil moisture values for debugging
  Serial.println("===== Soil Moisture Sensor Data =====");
  Serial.print("Temperature: "); Serial.print(tempC); Serial.println(" *C");
  Serial.print("Capacitive Reading: "); Serial.println(capread);
  Serial.print("Soil Moisture (converted to Humidity): "); Serial.print(current_humidity); Serial.println(" %");
  Serial.println("=====================================");

  // Publish the humidity value to the Matter network every 10 seconds
  if ((last_action + UPDATE_RATE_MS) < millis()) {
    last_action = millis(); // Update the last action time

    // Publish the humidity value to the Matter network
    matter_humidity_sensor.set_measured_value(current_humidity);
    
    // Log the current humidity to the Serial console
    Serial.printf("*****Published Humidity: %.01f%%\n", current_humidity);
  }
}
