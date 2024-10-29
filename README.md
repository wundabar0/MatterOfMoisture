A Matter of Moisture: Solar-Powered Soil Sensing with Arduino Matter Nano
By Brendan Henrich, Senior FAE at Symmetry Electronics

Ever wondered if your plants secretly judge you for forgetting to water them? Fear not! With the power of the sun and the versatility of the Arduino Matter Nano, you can create a solar-powered moisture sensor that keeps your plants—and your conscience—well-hydrated. Plus, it integrates seamlessly with the Apple Home ecosystem, changing the color of your Nanoleaf Matter light bulb based on soil moisture levels.

Introduction
Maintaining the right soil moisture is vital for plant health, but constant monitoring can be impractical. This project combines renewable energy and IoT technology to create a solar-powered soil moisture sensor using the Arduino Matter Nano. The system uses a Nanoleaf Matter light bulb to visually indicate soil moisture levels, making it easier to care for your plants.

Getting Started with Arduino Matter Nano
Before diving into the hardware setup, ensure you have the Arduino IDE installed and configured for the Arduino Matter Nano.

Key Steps:

Install the latest version of the Arduino IDE.
Add the Arduino Matter Nano board to your IDE.
Hardware Components
You'll need the following components for this project:

Arduino Matter Nano: A globally certified 2.4 GHz module supporting IEEE 802.15.4 protocols like Matter and Thread.
Adafruit STEMMA Soil Moisture Sensor: An I2C capacitive moisture sensor for accurate soil readings.
Solar Power Manager Module (D): Efficiently manages solar panel input and battery charging.
LiPo Battery (1200mAh): Stores energy collected from the solar panel.
RAK Wireless Solar Panel Case: Enclosure with an integrated solar panel.
Nanoleaf Matter Light Bulb (RGB A19): Smart light bulb that changes colors based on soil moisture levels.
Connecting the Soil Moisture Sensor
The Adafruit STEMMA Soil Moisture Sensor communicates via I2C, making it easy to connect to the Arduino Matter Nano.

Wiring Connections:

Soil Sensor Vin → Arduino +3V3 OUT
Soil Sensor GND → Arduino GND
Soil Sensor SDA → Arduino A4 (SDA)
Soil Sensor SCL → Arduino A5 (SCL)
Powering with Solar Energy
We're using solar power with the Solar Power Manager Module (D), which supports solar panels ranging from 6V to 24V.

Wiring Connections:

LiPo Battery to Battery IN
Solar Panel from enclosure to Solar IN
Arduino Nano USB C to 5V OUT via USB C connector
Programming the Arduino Nano
Open the matter_humidity_sensor example in the Arduino IDE under Matter examples.

Adding the Adafruit Seesaw Library
To enable support for the soil moisture sensor, install the Adafruit Seesaw library:

Go to Sketch > Include Library > Manage Libraries.
Search for "Adafruit Seesaw" and install it.
Code Modifications:

cpp
Copy code
#include <Adafruit_seesaw.h>
Adafruit_seesaw ss;

void setup() {
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! Seesaw not found");
    while(1);
  }
}

void loop() {
  uint16_t capread = ss.touchRead(0);
  float humidity = getHumidityFromCapacitance(capread);
}
Calibrating the Moisture Sensor
The soil moisture sensor uses capacitive sensing, and calibration is crucial for accurate measurements.

Calibration Steps:

Dry Calibration (Min Value): Expose the sensor to air and note the reading.
Wet Calibration (Max Value): Submerge the sensor fully in water and note the reading.
Example Calibration Values:

cpp
Copy code
const uint16_t CAPREAD_MIN = 345;  // Dry (air)
const uint16_t CAPREAD_MAX = 1016; // Wet (fully submerged)

float getHumidityFromCapacitance(uint16_t capread) {
  return constrain(map(capread, CAPREAD_MIN, CAPREAD_MAX, 0, 100), 0.0f, 100.0f);
}
Integrating with Apple Home and Nanoleaf
To make your sensor communicate with the Apple Home ecosystem and control your Nanoleaf Matter light bulb:

Commission the Device: Follow the user guide to commission the device onto your Matter network.
Onboarding QR Code: Use the provided QR code for onboarding.
Creating Automations
With the device added to Apple Home, you can create automations to change the color of your Nanoleaf Matter light bulb based on soil moisture levels.

Steps:

Open the Apple Home App and navigate to the Automations tab.
Select "A Sensor Detects Something" and choose your moisture sensor.
Set actions to change the Nanoleaf light bulb color based on moisture levels.
Room for Improvement
While the project meets its primary objectives, here are some areas for enhancement:

Low Power Modes: Currently, low power modes are not implemented.
Enhanced Power Management: Consider using the AEM10941 from e-peas for improved efficiency.
Sensor Power Optimization: Control the power to the sensor, turning it on only when needed.
Hardware Design Enhancements: A smaller enclosure and adding an e-ink screen for local feedback.
Conclusion
