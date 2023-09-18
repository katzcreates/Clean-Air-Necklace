#include <FastLED.h>

//#define DEBUG 1

#define DATA_PIN 5 // Pinout for the LEDs
const int DEMO_SWITCH = 15; // A slide switch used to turn demo mode on and off
#define BRIGHTNESS 100
#define NUM_LEDS 185
#define PM_INCREMENT_REGULAR 1   // Increment for regular PM change
#define PM_MAX 350               // Max PM value
#define PM_MAX_SLOW 16           // Max PM value for slow change
#define PM_INCREMENT_SLOW 1     // Increment for slow PM change (in 1/100 units)


CRGB leds[NUM_LEDS];
CRGB base[NUM_LEDS];
CRGB overlay[NUM_LEDS];

uint8_t colorIndex;
uint8_t pmColor;


uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// XY maps
const byte LED_COLS = 33;
const byte LED_ROWS = 23;

byte coordsX[NUM_LEDS] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 6, 6, 6, 6, 6, 8, 8, 8, 8, 8, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 22, 22, 22, 22, 22, 22, 22, 22, 22, 24, 24, 24, 24, 24, 24, 26, 26, 26, 26 };
const byte coordsY[NUM_LEDS] = { 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 20, 19, 18, 17, 16, 20, 19, 18, 17, 16, 15, 14, 20, 19, 18, 17, 16, 15, 14, 13, 12, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 20, 19, 18, 17, 16, 15, 14, 13, 12, 20, 19, 18, 17, 16, 15, 20, 19, 18, 17 };
byte angles[NUM_LEDS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 130, 130, 130, 131, 131, 131, 132, 132, 133, 133, 134, 136, 137, 141, 146, 159, 191, 223, 236, 242, 245, 247, 248, 249, 250, 251, 251, 251, 252, 252, 252, 252, 252, 247, 243, 240, 236, 233, 245, 240, 236, 232, 229, 226, 223, 242, 236, 231, 227, 223, 220, 217, 215, 213, 236, 229, 223, 219, 215, 212, 210, 208, 207, 205, 204, 203, 223, 215, 210, 207, 204, 203, 201, 200, 199, 199, 198, 197, 197, 197, 196, 196, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 159, 167, 172, 176, 178, 180, 181, 182, 183, 184, 185, 185, 185, 186, 186, 186, 187, 187, 146, 154, 159, 164, 167, 170, 172, 174, 176, 177, 178, 179, 141, 146, 151, 156, 159, 162, 165, 167, 169, 137, 142, 146, 150, 154, 157, 136, 139, 143, 146 };
byte radii[NUM_LEDS] = { 185, 174, 162, 151, 139, 128, 116, 104, 93, 81, 70, 58, 46, 35, 23, 12, 0, 12, 23, 35, 46, 58, 70, 81, 93, 104, 116, 128, 139, 151, 162, 174, 185, 186, 174, 163, 151, 140, 128, 116, 105, 93, 82, 71, 59, 48, 37, 26, 16, 12, 16, 26, 37, 48, 59, 71, 82, 93, 105, 116, 128, 140, 151, 163, 174, 186, 118, 121, 125, 130, 135, 96, 99, 104, 109, 116, 123, 131, 73, 78, 84, 91, 98, 107, 116, 125, 135, 52, 58, 66, 74, 84, 93, 104, 114, 125, 136, 147, 158, 33, 42, 52, 62, 73, 84, 96, 107, 118, 130, 141, 152, 164, 175, 187, 198, 23, 35, 46, 58, 70, 81, 93, 104, 116, 128, 139, 151, 162, 174, 185, 197, 209, 220, 232, 243, 255, 33, 42, 52, 62, 73, 84, 96, 107, 118, 130, 141, 152, 164, 175, 187, 198, 210, 221, 52, 58, 66, 74, 84, 93, 104, 114, 125, 136, 147, 158, 73, 78, 84, 91, 98, 107, 116, 125, 135, 96, 99, 104, 109, 116, 123, 118, 121, 125, 130 };

byte gridIndexHorizontal(byte x, byte y) {
  for (byte i = 0; i < NUM_LEDS; i++) {
    if (coordsX[i] == x && coordsY[i] == y) {
      return i;
    }
  }
  return 255;
}
struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};
 
struct pms5003data data;

int pm = 0;
int threshold = 12;

unsigned long lastUpdateTime = 0;  // Time when PM value was last updated
unsigned long updateInterval = 50; // Time interval (in milliseconds) between PM value updates

int pm_whole = 0;     // Whole part of PM value
int pm_fraction = 0;  // Fractional part of PM value (in 1/100 units)

unsigned long slowUpdateTime = 0; // Time when PM value was last updated (below PM_MAX_SLOW)
unsigned long slowUpdateInterval = 5; // Time interval (in milliseconds) between PM value updates (below PM_MAX_SLOW)


DEFINE_GRADIENT_PALETTE( warning_p ) {
  0, 255, 253, 225,
  30, 255, 248, 0,
  120, 255, 102,  0,
  205,  255,  0,  0,
  229, 217, 5, 102,
  255, 149, 0, 133
};



CRGBPalette16 myPal = warning_p;




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
      colorIndex = map(pm, threshold, PM_MAX, 0, 240);
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
      y = startY + age / 2500.0 * speed;

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

#define NUMBER_OF_STARS 36

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

  //  fill_solid(leds, NUM_LEDS, CRGB::Black);

  for (int i = 0; i < NUMBER_OF_STARS; i++) {
    stars[i].move();
    drawStar(stars[i]);
  }

  // blur2d(leds, LED_COLS, LED_ROWS, 64);
}
} // namespace Starfield

void setup() {
  //demo mode switch setup
  pinMode(DEMO_SWITCH, INPUT);

  Serial.begin(115200);
#ifdef DEBUG
  while (!Serial) {
    ; // wait for serial port to connect
  }
#endif
  delay(10);

  // sensor baud rate is 9600
  Serial1.begin(9600);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

}





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


void loop()
{
  unsigned long currentTime = millis();
  FastLED.show();

  int ValueDEMO_SWITCH;  
  ValueDEMO_SWITCH = digitalRead(DEMO_SWITCH);  
  if (ValueDEMO_SWITCH == HIGH) {  


 

  if (readPMSdata(&Serial1)) {
    // reading data was successful!
    pm = data.pm25_env;
    Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_standard);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_standard);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_standard);
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (environmental)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
    Serial.println("---------------------------------------");
    Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
    Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
    Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
    Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
    Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
    Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(data.particles_100um);
    Serial.println("---------------------------------------");
  }
  }
  else {
    if (pm_whole >= PM_MAX_SLOW) {
    if (currentTime - lastUpdateTime >= updateInterval) {
      // Update PM value at regular rate
      pm_whole += PM_INCREMENT_REGULAR;

      if (pm_whole >= PM_MAX) {
        pm_whole = 0;
        pm_fraction = 0;
      }

      pm = pm_whole + pm_fraction / 100.0; // Convert back to float for display

      // Update last update time
      lastUpdateTime = currentTime;
    }
  } else {
    if (currentTime - slowUpdateTime >= slowUpdateInterval) {
      // Update PM value at slow rate
      pm_fraction += PM_INCREMENT_SLOW;

      if (pm_fraction >= 100) {
        pm_whole += pm_fraction / 100;
        pm_fraction %= 100;
      }

      pm = pm_whole + pm_fraction / 100.0; // Convert back to float for display

      // Update last update time
      slowUpdateTime = currentTime;
    }
  }
  } 


#ifdef DEBUG
//  Serial.print("TEST: ");
//  Serial.println(pm);
//  Serial.print(colorIndex);
//  Serial.println();
EVERY_N_SECONDS(1) {
  Serial.print("TEST: ");
  Serial.println(pm);
  Serial.print(colorIndex);
  Serial.println();
}

  
#endif

  basePattern();
  overlayPattern();
  combinePatterns();


}

boolean readPMSdata(Stream *s) {
  if (! s->available()) {
    return false;
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }
 
  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }
 
  /* debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  */
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
 
  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);
 
  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // success!
  return true;
}

// Increment PM value for testing / demo
void incPm(int inc) {
  pm = pm + inc;
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

void overlayPattern() {  //NOTE: we are drawing to the "overlay" CRGB here
  if (pm > threshold) { //overlay is currently "On"

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
}//end_combinePatterns
