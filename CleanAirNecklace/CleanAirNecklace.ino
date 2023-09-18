#include <PubSubClient.h>
#include <FastLED.h>
#include "secrets.h"
#include "ArduinoJson.h"

#define DEBUG

// Nano 33 IoT compatibility
#if defined(ARDUINO_ARCH_SAMD)
#include <WiFiNINA.h>
#define DATA_PIN 9
#else
#include <WiFi.h>
#define DATA_PIN 33
#endif

#define BRIGHTNESS 32 // Universal LED brightness override
#define NUM_LEDS 185
#define NUMBER_OF_STARS 36
#define PM_MAX 500  // Max PM value
#define PM_THRESHOLD 12

// Demo stuff
#define PM_INCREMENT 10 // Amount PM changes each interval
#define PM_INTERVAL 500 // Change PM every N milliseconds
#define NEC_CONNECTED 0  // Connected mode
#define NEC_DEMO 1  // Demo mode
#define PM_CYCLE_INTERVAL 30 // Cycle interval in seconds  
int mode = NEC_DEMO;  // Default to demo mode

CRGB leds[NUM_LEDS];
CRGB base[NUM_LEDS];
CRGB overlay[NUM_LEDS];

uint8_t colorIndex;
uint8_t pmColor;

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

// Depending on your animation, you may find different kinds of mapping useful. I've provided two types here, used by the animation below. You can add your own or use that provided here.
// 0-255 XY maps
byte coordsX256[NUM_LEDS] = { 0, 36, 73, 109, 146, 182, 219, 255, 255, 219, 182, 146, 109, 73, 36, 0, 0, 36, 73, 109, 146, 182, 219, 255, 255, 219, 182, 146, 109, 73, 36, 0, 0, 36, 73, 109, 146, 182, 219, 255, 255, 219, 182, 146, 109, 73, 36, 0, 0, 36, 73, 109, 146, 182, 219, 255, 255, 219, 182, 146, 109, 73, 36, 0 };
byte coordsY256[NUM_LEDS] = { 0, 0, 0, 0, 0, 0, 0, 0, 36, 36, 36, 36, 36, 36, 36, 36, 73, 73, 73, 73, 73, 73, 73, 73, 109, 109, 109, 109, 109, 109, 109, 109, 146, 146, 146, 146, 146, 146, 146, 146, 182, 182, 182, 182, 182, 182, 182, 182, 219, 219, 219, 219, 219, 219, 219, 219, 255, 255, 255, 255, 255, 255, 255, 255 };

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// XY maps
const byte LED_COLS = 33;
const byte LED_ROWS = 23;
const byte coordsX[NUM_LEDS] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 26, 26, 26, 26, 26, 24, 24, 24, 24, 24, 24, 24, 22, 22, 22, 22, 22, 22, 22, 22, 22, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 10, 10, 10, 10, 10, 10, 10, 10, 10, 8, 8, 8, 8, 8, 8, 6, 6, 6, 6 };
const byte coordsY[NUM_LEDS] = { 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 20, 19, 18, 17, 16, 20, 19, 18, 17, 16, 15, 14, 20, 19, 18, 17, 16, 15, 14, 13, 12, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 20, 19, 18, 17, 16, 15, 14, 13, 12, 20, 19, 18, 17, 16, 15, 20, 19, 18, 17, 22 };
const byte radii[] = { 128, 120, 112, 104, 96, 88, 80, 72, 64, 56, 48, 40, 32, 24, 16, 8, 1, 7, 15, 23, 31, 39, 47, 55, 63, 71, 79, 87, 95, 103, 111, 119, 127, 128, 120, 112, 104, 96, 88, 80, 72, 64, 56, 49, 41, 33, 26, 19, 14, 12, 14, 20, 27, 34, 42, 50, 57, 65, 73, 81, 89, 97, 105, 113, 121, 129, 83, 86, 92, 98, 105, 67, 72, 78, 86, 94, 103, 112, 53, 59, 66, 75, 84, 94, 104, 115, 125, 39, 47, 56, 66, 76, 87, 98, 109, 120, 131, 143, 154, 28, 38, 49, 60, 71, 83, 94, 105, 117, 128, 140, 151, 163, 175, 186, 198, 23, 35, 46, 58, 70, 81, 93, 104, 116, 128, 139, 151, 162, 174, 185, 197, 209, 220, 232, 243, 255, 28, 38, 49, 60, 71, 83, 94, 106, 117, 129, 140, 152, 163, 175, 186, 198, 209, 221, 40, 48, 57, 66, 77, 87, 98, 109, 120, 132, 143, 154, 54, 60, 67, 75, 85, 94, 105, 115, 126, 68, 73, 79, 87, 95, 103, 83, 87, 93, 99 };

byte gridIndexHorizontal(byte x, byte y) {
  for (byte i = 0; i < NUM_LEDS; i++) {
    if (coordsX[i] == x && coordsY[i] == y) {
      return i;
    }
  }
  return 255;
}

// ESDK MQTT Connection
//const char broker[] = "255.255.255.255"; // Replace with the local IP of your sensor
const char broker[] = "airquality";
int        port     = 1883;
#define TOPIC "airquality/#"

// Wifi Connection
// I recommend creating your own secrets.h file to keep your connection information secure!

char ssid[] = SECRET_SSID;  // Network SSID // Use info from secrets.h or place your own SSID here
char pass[] = SECRET_PASS;  // WPA key // Use info from secrets.h or place your own password here

WiFiClient wifiClient;
long lastReconnectWIFIAttempt = 0;
PubSubClient mqttClient(wifiClient);
long lastReconnectMQTTAttempt = 0;

int wifiState = WL_IDLE_STATUS;


// ESDK Sensor Data
// Please see the ESDK documentation for more information and data options

//int co2 = 0;
//double temperature = 0;
//double humidity = 0;
//int tvoc = 0;
int pm = 0;

// Custom FastLED Palette
DEFINE_GRADIENT_PALETTE( warning_p ) {
  0, 255, 249, 155,
  60, 255, 102, 0,
  120, 255, 102,  0,
  180,  255,  0,  0,
  220, 217, 5, 102,
  255, 149, 0, 46
};

CRGBPalette16 myPal = warning_p;



// Bad Air Sparks Animation
class Star {
  public:
    float z;
    float startY;
    float y;
    float x;
    unsigned long startTime;
    int hue;
    int saturation;



    void restart() {
      z = random(1, 4);
      y = 0;
      hue = pmColor;
      // saturation = 80 + 128 / z;
      saturation = 255 - (z - 1) * 50;
      startY = y;
      x = random(0, LED_COLS);
      startTime = millis();
      pm = constrain(pm, 0, PM_MAX);
      colorIndex = map(pm, PM_THRESHOLD, PM_MAX, 0, 240); 
      pmColor = ColorFromPalette(myPal, colorIndex, LINEARBLEND);
    }

    void start() {
      restart();
      y = 0;
      startY = y;
    }

    void move() {
      unsigned long age = millis() - startTime;
      float speed = z * 10;
      y = startY + age / 2300.0 * speed;

      hue++;

      if (isDead())
        restart();
    }

    bool isDead() {
      return x < 0 || x >= LED_COLS || y < 0 || y >= LED_ROWS;
    }

    float length() {
      return 3 + z * 5;
    }

    float brightness() {
      return 0.2 + 0.4 * (z - 1);
    }
};

namespace Starfield {
Star stars[NUMBER_OF_STARS] = {};
bool setup = false;

void drawStar(Star star) {
  int16_t index = gridIndexHorizontal(star.x, star.y);
  float length = star.length();
  int radius = 5;

  // 20% + brightness / 80%
  // float brightnessFactor = (star.z / 3.0) * 0.9 + 0.1;
  // float brightnessFactor = 0.1 + 0.3 * star.z -;
  float brightnessFactor = star.brightness();

  int x = star.x;
  double startY = star.y;
  int lowY = floor(startY) - radius;
  int ceilY = ceil(startY) + radius;

  int headLength = 2;
  for (int y = startY - headLength; y <= ceilY; y++) {
    if (y < 0 || y >= LED_ROWS) continue;

    int16_t index = gridIndexHorizontal(x, y);
    if (index >= NUM_LEDS)
      continue;

    double distance = (fabs(y - startY) - 0.5) / headLength;
    if (distance < 0) {
      leds[index] += ColorFromPalette(myPal, colorIndex, 255.0 * brightnessFactor);
    } else if (distance > 1) {} else {
      int brightness = 255 - distance * 255.0;
      leds[index] += ColorFromPalette(myPal, colorIndex, brightness * brightnessFactor);
    }
  }

  // Tail
  int tailLength = length;
  for (int y = startY; y >= startY - tailLength; y--) {
    if (y < 0 || y >= LED_ROWS) continue;

    int16_t index = gridIndexHorizontal(x, y);
    if (index >= NUM_LEDS)
      continue;

    double distance = (fabs(y - startY) - 0.5) / tailLength;
    if (distance < 0) {
      leds[index] += ColorFromPalette(myPal, colorIndex, 255.0 * brightnessFactor);
    } else if (distance > 1) {} else {
      int brightness = 255 - distance * 255.0;
      leds[index] += ColorFromPalette(myPal, colorIndex, brightness * brightnessFactor);
    }
  }
}

void draw() {
  if (!setup) {
    setup = true;
    for (int i = 0; i < NUMBER_OF_STARS; i++) {
      stars[i].start();
    }
  }

  for (int i = 0; i < NUMBER_OF_STARS; i++) {
    stars[i].move();
    drawStar(stars[i]);
  }
}
}

void setup() {
  Serial.begin(115200);
#ifdef DEBUG
  while (!Serial) {
    ; // wait for serial port to connect
  }
  Serial.println("Starting Clean Air Necklace");
#endif
  delay(10);

  // We start by connecting to a WiFi network
  // WL_IDLE_STATUS     = 0
  // WL_NO_SSID_AVAIL   = 1
  // WL_SCAN_COMPLETED  = 2
  // WL_CONNECTED       = 3
  // WL_CONNECT_FAILED  = 4
  // WL_CONNECTION_LOST = 5
  // WL_DISCONNECTED    = 6
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
    if ((WiFi.status() == WL_CONNECT_FAILED) || (WiFi.status() == WL_NO_SSID_AVAIL)) {
      mode = NEC_DEMO;
      Serial.println("Entering demo mode");
      break;
    }
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    mode = NEC_CONNECTED;
    Serial.println("Entering connected mode");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    mqttClient.setServer(broker, port);
    mqttClient.setCallback(callback);
    mqttClient.setBufferSize(384);
  }

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}

// Base Animation Patterns get set here! Add your own if you want!

void outwardOcean() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(OceanColors_p, radii[i] - gHue);
  }
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {
  outwardOcean
};

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

int pm_cycle = 0;

void loop()
{
  FastLED.show();

  switch (mode) {
    // Do connected stuff
    case NEC_CONNECTED:
      if (WiFi.status() != WL_CONNECTED) {
        reconnectWiFi();
      }

      if (!mqttClient.connected()) {
        // Attempt to reconnect without blocking
        long now = millis();
        if (now - lastReconnectMQTTAttempt > 5000) {
          lastReconnectMQTTAttempt = now;
          if (reconnectMQTT()) {
            lastReconnectMQTTAttempt = 0;
          }
        }
      }
      else {
        mqttClient.loop();
      }
      break;

    // Do demo stuff
    case NEC_DEMO:
      changePmCycle(PM_CYCLE_INTERVAL);
      switch (pm_cycle) {
        case 0:
          pm = 1;
          break;
        case 1:
          incPM(PM_INTERVAL, PM_INCREMENT);
          break;
        case 2:
          decPM(PM_INTERVAL, PM_INCREMENT);
          break;
      }
  }
#ifdef DEBUG
  // This can be used to help debug problems with the sensor connection if needed.
  EVERY_N_SECONDS(1) {
    Serial.print("PM2.5: ");
    Serial.print(pm);
    Serial.println();
  }
#endif
  basePattern();
  overlayPattern();
  combinePatterns();
}  // end loop


// Change PM cycle every interval (demo mode)
void changePmCycle(int interval) {
  EVERY_N_SECONDS(interval) {
    pm_cycle++;
    if (pm_cycle > 2) {
      pm_cycle = 0;
    }
  }
}

// Increment PM every interval (demo mode)
void incPM(int interval, int increment) {
  if (pm < PM_MAX) {
    EVERY_N_MILLISECONDS(interval) {
      pm += increment;
    }
  }
  else {  // Hold at max pm
    pm = PM_MAX;
  }
}

// Decrement PM every interval (demo mode)
void decPM(int interval, int increment) {
  if (pm < 1) { // Hold at 1
    pm = 1;
  }
  else {
    EVERY_N_MILLISECONDS(interval) {
      pm -= increment;
    }
  }
}

void basePattern() {
  gPatterns[gCurrentPatternNumber]();

  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }

  EVERY_N_SECONDS( 10 ) {
    nextPattern();  // change patterns periodically
  }

}//end_basePattern

int fadeAmount1 = 0;
int fadeAmount2 = 0;
int maxFadeAmount1 = 255;
int maxFadeAmount2 = 255;

void overlayPattern() {  // This is where the Sparks animation gets triggered if the PM levels are above threshold
  if (pm > PM_THRESHOLD) {

    fadeToBlackBy( leds, NUM_LEDS, fadeAmount1);

    EVERY_N_MILLISECONDS(30) {
      if (fadeAmount1 < maxFadeAmount1) {
        fadeAmount1++;
      }
    }



    EVERY_N_MILLIS(30) {
      gHue++;
    }

    Starfield::draw();
  }


  else {  //overlay is currently "Off"
    fadeToBlackBy( leds, NUM_LEDS, fadeAmount1);
    EVERY_N_MILLISECONDS(20) {
      if (fadeAmount1 > 0) {
        fadeAmount1--;
      }
    }
    fadeToBlackBy( overlay, NUM_LEDS, fadeAmount2);
    EVERY_N_MILLISECONDS(30) {
      if (fadeAmount2 < maxFadeAmount2) {
        fadeAmount2++;  //fadeout overlay pattern
      }
    }
  }
}

void combinePatterns() {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] += overlay[i];  //add overlay to leds
  }
}

// Connecting to a WiFi network
void reconnectWiFi() {
#ifdef DEBUG
  Serial.print("Wifi Status: ");
  Serial.println(WiFi.status());
  Serial.print("Trying connection to ");
  Serial.println(ssid);
#endif
  WiFi.disconnect();
  delay(10000);
  wifiState = WiFi.begin(ssid, pass);
}

boolean reconnectMQTT() {
  Serial.print("Attempting connection to MQTT broker: ");
  Serial.println(broker);
  if (mqttClient.connect("necklaceClient")) {
    mqttClient.subscribe("airquality/#");
    Serial.println("MQTT connected");
  }
  return mqttClient.connected();
}

void callback(char* topic, byte * payload, unsigned int length) {
#ifdef DEBUG
  Serial.println("Message");
  //  Serial.print("Message arrived [");
  //  Serial.print(topic);
  //  Serial.print("] ");
  //  for (int i = 0; i < length; i++) {
  //    Serial.print((char)payload[i]);
  //  }
  //  Serial.println();
#endif
  // ESDK sends a large JSON payload
  // - ensure you have enough memory allocated
  StaticJsonDocument<384> doc;
  deserializeJson(doc, payload, length);
  // Unused ESDK values
  //  co2 = doc["co2"]["co2"];
  //  temperature = doc["thv"]["temperature"];
  //  humidity = doc["thv"]["humidity"];
  //  tvoc = doc["thv"]["vocIndex"];
  pm = doc["pm"]["pm2.5"];



}
