# A Matter of Moisture: Solar-Powered Soil Sensing with Arduino Matter Nano

_By Brendan Henrich, Senior FAE at Symmetry Electronics_


Ever wondered if your plants secretly judge you for forgetting to water them? Fear not! With the power of the sun and the versatility of the Arduino Matter Nano, you can create a solar-powered moisture sensor that keeps your plants—and your conscience—well-hydrated. Plus, it integrates seamlessly with the Apple Home ecosystem, changing the color of your Nanoleaf Matter light bulb based on soil moisture levels.

## Introduction

Maintaining the right soil moisture is vital for plant health, but constant monitoring can be impractical. This project provides a solution by combining renewable energy and IoT technology. We'll build a solar-powered soil moisture sensor using the Arduino Matter Nano, a versatile development board that integrates with the Apple Home ecosystem. The system uses a Nanoleaf Matter light bulb to visually indicate soil moisture levels, making it easier to care for your plants.

## Getting Started with Arduino Matter Nano

Before diving into the hardware setup, ensure you have the Arduino IDE installed and configured for the Arduino Matter Nano. Arduino provides an excellent getting started guide to help you set up the IDE and connect to the board.

**Key Steps:**
- Install the latest version of the Arduino IDE.
- Add the Arduino Matter Nano board to your IDE.

## Hardware Components

Here's what you'll need for this project:

- **Arduino Matter Nano:** A globally certified 2.4 GHz module supporting IEEE 802.15.4 protocols like Matter and Thread, with a programmable low-power MCU from Silicon Labs.
- **Adafruit STEMMA Soil Moisture Sensor:** An I2C capacitive moisture sensor for accurate soil readings.
- **Solar Power Manager Module (D):** Manages solar panel input and battery charging efficiently.
- **LiPo Battery (1200mAh):** Stores energy collected from the solar panel.
- **RAK Wireless Solar Panel Case (Unify Enclosure Solar IP65):** An enclosure with an integrated solar panel.
- **Nanoleaf Matter Light Bulb (RGB A19):** A smart light bulb that supports Matter and can change colors based on soil moisture levels.

## Connecting the Soil Moisture Sensor

The Adafruit STEMMA Soil Moisture Sensor communicates via I2C, making it easy to connect to the Arduino Matter Nano.

**Wiring Connections:**
- Soil Sensor Vin → Arduino +3V3 OUT
- Soil Sensor GND → Arduino GND
- Soil Sensor SDA → Arduino A4 (SDA)
- Soil Sensor SCL → Arduino A5 (SCL)

For more details on the sensor, check out the [Adafruit STEMMA Soil Sensor guide](https://www.adafruit.com/).

## Powering with Solar Energy

Taking inspiration from our plants, we're using solar power. The Solar Power Manager Module (D) from Waveshare supports solar panels ranging from 6V to 24V and includes a Type-C power adapter input.

**Wiring Connections:**
- LiPo Battery to Battery IN
- Solar Panel from enclosure to Solar IN
- Arduino Nano USB C to 5V OUT via USB C connector

## Programming the Arduino Nano

Start by opening the `matter_humidity_sensor` example in the Arduino IDE, found under Matter examples.

### Adding the Adafruit Seesaw Library

To enable support for the soil moisture sensor, we'll use the Adafruit Seesaw library.

**Installation:**
- Go to **Sketch > Include Library > Manage Libraries**.
- Search for "Adafruit Seesaw" and install it.

**Code Modifications:**
```cpp
#include <Adafruit_seesaw.h>

Adafruit_seesaw ss;

void setup() {
  // Existing setup code...
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! Seesaw not found");
    while(1);
  }
...
}

void loop() {
  uint16_t capread = ss.touchRead(0);
  float humidity = getHumidityFromCapacitance(capread);
  // Send humidity value to Matter stack...
...
}
```
Refer to the Adafruit STEMMA Soil Sensor Arduino Test guide for detailed instructions.
https://learn.adafruit.com/adafruit-stemma-soil-sensor-i2c-capacitive-moisture-sensor/arduino-test

### Calibrating the Moisture Sensor
The soil moisture sensor uses capacitive sensing, outputting capacitance readings that correspond to moisture levels. Calibration is crucial for accurate measurements.
Calibration Steps:
1.	Dry Calibration (Min Value): Expose the sensor to air (dry conditions) and note the reading.
2.	Wet Calibration (Max Value): Submerge the sensor fully in water and note the reading.
Example Calibration Values:
```cpp
const uint16_t CAPREAD_MIN = 345;  // Dry (air)
const uint16_t CAPREAD_MAX = 1016; // Wet (fully submerged)
```
### Mapping to Humidity Percentage
Use a helper function to map the capacitance readings to a percentage:
```cp
float getHumidityFromCapacitance(uint16_t capread) {
  return constrain(map(capread, CAPREAD_MIN, CAPREAD_MAX, 0, 100), 0.0f, 100.0f);
}
```
Adjust the CAPREAD_MIN and CAPREAD_MAX values based on your environment for better accuracy.

## Integrating with Apple Home and Nanoleaf

To make your moisture sensor communicate with the Apple Home ecosystem and control your Nanoleaf Matter light bulb:

1. **Commission the Device**: Follow the user guide to commission the device onto your Matter network.
2. **Onboarding QR Code**: Use the provided QR code for onboarding.

> **Note**: All Arduino Matter Nano boards produce the same QR code. For unique QR codes per device, consider using Silicon Labs' Provisioning tools with Simplicity Studio.

### Nanoleaf Matter Light Bulb

We're using the **Nanoleaf Matter Smart Bulb (RGB A19)**, which supports Matter for seamless integration. These bulbs can be purchased from major hardware stores. For more information, visit the Nanoleaf Matter Integration page.

### Creating Automations

With the device added to Apple Home, you can create automations to change the color of your Nanoleaf Matter light bulb based on soil moisture levels.

**Steps**:
1. **Open Apple Home App**: Navigate to the Automations tab.
2. **Create New Automation**: Select "A Sensor Detects Something".
3. **Choose Your Moisture Sensor**: Set conditions based on moisture levels.
4. **Select Actions**: Choose the Nanoleaf light bulb and set the desired color.

### Plant-Specific Moisture Levels

Customize the moisture thresholds based on your plant's needs:
- **Venus Flytrap (Dionaea muscipula)**: 50%–70%
- **Cactus (Cactaceae)**: 10%–20%
- **Pothos (Epipremnum aureum)**: 40%–60%


## Room for Improvement

While the project successfully meets its primary objectives, there are several areas where enhancements can be made:

### Low Power Modes
- Currently, low power modes are not implemented in this project. The Arduino SDK on Silicon Labs boards supports various low-power modes available on the EFR32 series, as detailed in the `ArduinoLowPower` library.
- However, the Matter stack on Arduino does not support sleep modes at this time.
- For projects where low power consumption is critical, consider using Silicon Labs' full SDK with Simplicity Studio. The Matter stack in this environment fully supports low-power modes.

### Enhanced Power Management
- For future iterations, consider using the AEM10941 from e-peas, which offers better efficiency and more advanced features for energy harvesting.
- Powering the Arduino Matter Nano directly via the VIN pin instead of the USB-C port can reduce power consumption by disabling the serial USB interface.

### Sensor Power Optimization
- The Adafruit Soil Moisture Sensor consumes about 3 mA continuously because it uses a separate MCU to read the sensor and communicate via I2C.
- An improved design would involve controlling the power to the sensor, turning it on only when a reading is needed to conserve energy.

### Hardware Design Enhancements
- Using a smaller enclosure could make the device more compact and less obtrusive.
- Adding an e-ink screen would provide local, low-power visual feedback on soil moisture levels, enhancing usability without significantly increasing power consumption.



## Conclusion

