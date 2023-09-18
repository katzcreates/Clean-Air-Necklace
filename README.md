# The Clean Air Necklace
Files and information for creating the Clean Air Necklace, both with the Design Spark Environmental Sensor Development Kit or with a commercially available particulate matter sensor. To see how the necklace was originally designed and built, check out the [full build video on YouTube](https://youtu.be/niAL5Nqm1jY).

There are two versions of this design that can be found in the 3D Models folder. The original design is V1, which is built to accomodate the Design Spark ESDK, and features slightly harder angles and edges in the necklace. The newer version, V2, features slightly softer edges in the necklace falls and is built to accomodate a commercial PM sensor. The designs can be mixed and matched if designed to use the newer design features with the ESDK, or the older design with the commercial PM sensor.

Both STEP and STL files can be found inside the 3D Models folder. Each STEP file contains all relevant models for that part of the project. Any adjustments to sizing or design changes can be made therein. Otherwise, for simple printing, individual STLs are provided, unsliced. One .3MF file has been provided for the clear top panels which is preset with the settings to create an alternating Hilbert Curve pattern for creating diffusion. Simply import the other top panel STLs into this file once opened to print with the same settings.

For the electronics components, virtually any microcontroller with an ESP32 enabled chip will do, as well as any addressable LEDs, but to take advantage of the compact electronics enclosure I designed as well as the code I wrote, I recommend the following components:

- [The Unexpected Maker TinyPICO V2](https://www.tinypico.com/)

- [144/metre Neopixel LED Strips](https://www.adafruit.com/product/1506)

- [Adafruit USB-C Breakout Board](https://www.adafruit.com/product/4090)

- [A simple Slide Switch](https://www.adafruit.com/product/805)


If going for the V2 build, a different microcontroller is required that can take hardware serial data in, which the Tiny Pico cannot do. As such, I recommend the following components for a V2 build:

- [Adafruit ItsyBitsy MO Express](https://www.adafruit.com/product/3727)

You will also need a particulate matter sensor for V2. The one used in my code and most readily available is the PMS5003:

- [PMS5003 Particulate Matter Sensor](https://shop.pimoroni.com/products/pms5003-particulate-matter-sensor-with-cable?variant=29075640352851)

Otherwise the components are the same. V2 does require a second slide switch to provide a way to switch because live data readings and the demo mode provided in the code.



The code provided for V1 will enable connection to WiFi, an MQTT broker hosted locally, and provide animations correspondant to PM 2.5 readings from a sensor. This is set up to work with the [DesignSpark ESDK](https://www.rs-online.com/designspark/introducing-the-environmental-sensor-development-kit). The code requires the FastLED library to function, which can be found [here](https://github.com/FastLED/FastLED). It is currently set up with two patterns; a base pattern when the PM levels are below a threshold, and an overlay for when PM levels exceed threshold. Additional patterns can be added to cycle through the base pattern easily within the code.

The code for V2 also uses FastLED libraries for the animation (which is functionally the same as V1) but is set up to receive data from the PMS5003 and work with a slide switch to choose between live readings and the demo mode. This version does not require WiFi to work.

The current PM threshold is set based on information set out by the US Environmental Protection agency, but are by no means definitive and can be altered to communicate data differently. Feel free to change the threshold within the code as you see fit.
