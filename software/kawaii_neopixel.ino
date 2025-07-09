#include <Adafruit_NeoPixel.h>

#define PIN        PIN_PC2
#define NUMPIXELS  6
#define DELAYVAL   20  // Optimal for smoothness
#define VALUE      100 // Brightness

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const uint16_t hueOffsets[6] = {0, 256, 512, 768, 1024, 1280}; 

uint32_t hsvToRgbSmooth(uint16_t h) {
    h %= 1536; // Wrap at 6*256
    uint8_t hueSegment = h >> 8; // 0-5
    uint8_t pos = h & 0xFF;     // Position in segment

    uint8_t r, g, b;
    switch(hueSegment) {
        case 0: r=255; g=pos; b=0; break;       // Red to Yellow
        case 1: r=255-pos; g=255; b=0; break;   // Yellow to Green
        case 2: r=0; g=255; b=pos; break;       // Green to Cyan
        case 3: r=0; g=255-pos; b=255; break;  // Cyan to Blue
        case 4: r=pos; g=0; b=255; break;      // Blue to Magenta
        default: r=255; g=0; b=255-pos; break; // Magenta to Red
    }
    
    // Apply brightness
    return pixels.Color(
        (r * VALUE) >> 8,
        (g * VALUE) >> 8,
        (b * VALUE) >> 8
    );
}

void setup() {
  pixels.begin();
  pixels.clear();
}

void loop() {
  static uint16_t hue = 0; // 16-bit for smoothness

  for(uint8_t i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, hsvToRgbSmooth(hue + hueOffsets[i]));
  }

  pixels.show();
  hue += 6;
  delay(DELAYVAL);
}