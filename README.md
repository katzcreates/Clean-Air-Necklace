# The Clean Air Necklace
Files and information for creating the Clean Air Necklace with the Design Spark Environmental Sensor Development Kit. To see how the necklace was designed and built, check out the [full build video on YouTube](https://youtu.be/niAL5Nqm1jY).

Both STEP and STL files can be found inside the 3D Models folder. Each STEP file contains all relevant models for that part of the project. Any adjustments to sizing or design changes can be made therein. Otherwise, for simple printing, individual STLs are provided, unsliced. One .3MF file has been provided for the clear top panels which is preset with the settings to create an alternating Hilbert Curve pattern for creating diffusion. Simply import the other top panel STLs into this file once opened to print with the same settings.

For the electronics components, virtually any microcontroller with an ESP32 enabled chip will do, as well as any addressable LEDs, but to take advantage of the compact electronics enclosure I designed as well as the code I wrote, I recommend the following components:

- [The Unexpected Maker TinyPICO V2](https://www.tinypico.com/)

- [144/metre Neopixel LED Strips](https://www.adafruit.com/product/1506)

- [Adafruit USB-C Breakout Board](https://www.adafruit.com/product/4090)

- [A simple Slide Switch](https://www.adafruit.com/product/805)


The code provided will enable connection to WiFi, an MQTT broker hosted locally, and provide animations correspondant to PM 2.5 readings from a sensor. This is set up to work with the [DesignSpark ESDK](https://www.rs-online.com/designspark/introducing-the-environmental-sensor-development-kit) but the code can be changed to use the animation with another sensor. The code requires the FastLED library to function, which can be found [here](https://github.com/FastLED/FastLED). It is currently set up with two patterns; a base pattern when the PM levels are below a threshold, and an overlay for when PM levels exceed threshold. Additional patterns can be added to cycle through the base pattern easily within the code.

The current PM threshold is set based on information set out by the US Environmental Protection agency, but are by no means definitive and can be altered to communicate data differently. Feel free to change the threshold within the code as you see fit.
