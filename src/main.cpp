#include "main.h"
#include "snowflakes\snowflakes.h"
#include <algorithm> // für std::sort

LGFX tft;
LGFX_Sprite canvas(&tft);

void setup() {
  Serial.begin(115200);
  delay(1000);

  printMemoryInfo();

  tft.begin();
  tft.setRotation(0);
  tft.invertDisplay(false);
  tft.fillScreen(TFT_BLACK);

  canvas.setColorDepth(8); //16bit zu groß für einen Speicherblock
  canvas.createSprite(CANV_W, CANV_H);

  for (int i = 0; i < NUMFLAKES; i++) {
    constexpr uint8_t variants = NUM_SNOWFLAKES; // Define NUM_SNOWFLAKES as the size of snowflakesData
    uint8_t index = random(0, variants);
    schneeflocken[i].x = random(0, CANV_W);
    schneeflocken[i].y = random(-CANV_H, 0);
    schneeflocken[i].size = snowflakesData[index].width; // Access as an array of structs
    schneeflocken[i].bitmap = snowflakesData[index].data;
    schneeflocken[i].geschwindigkeit = map(schneeflocken[i].size, 1, MAX_FLOCKEN_SIZE, 1, 4)+random(2);
    uint8_t col = map(schneeflocken[i].size, 1, MAX_FLOCKEN_SIZE, 50, 255);
    //schneeflocken[i].color = random(0xFFFF);
    schneeflocken[i].color = tft.color565(col, col, col);
  }

  // Schneeflocken sortieren (kleinste zuerst)
  std::sort(schneeflocken, schneeflocken + NUMFLAKES, vergleicheSchneeflocken);

  printMemoryInfo();
}

void loop() {
  unsigned long currentMillis = millis();

  static unsigned long fpsMillis = 0;
  static int fpsCounter = 0;

  zeichneSchneeflocken();

  fpsCounter++;

  if (currentMillis - fpsMillis >= 1000) {
    // Anzahl der Schleifen pro Sekunde ausgeben
    Serial.print(fpsCounter);
    Serial.println(" fps");
    // Zurücksetzen des Zählers und der Zeit
    fpsCounter = 0;
    fpsMillis = currentMillis;
  }
}

void zeichneSchneeflocken() {
  canvas.fillScreen(TFT_BLACK);

  for (int i = 0; i < NUMFLAKES; i++) {
    int16_t x = schneeflocken[i].x;
    int16_t y = schneeflocken[i].y;
    int16_t s = schneeflocken[i].size;

    canvas.drawBitmap(x, y, schneeflocken[i].bitmap, s, s, schneeflocken[i].color);

    // Bewegung updaten
    schneeflocken[i].y += schneeflocken[i].geschwindigkeit;
    uint16_t seed = random(0, 100);
    if (seed < 10) schneeflocken[i].xnext -= random(10);
    else if (seed > 90) schneeflocken[i].xnext += random(10);

    if (schneeflocken[i].y > CANV_H) {
      schneeflocken[i].y = random(-CANV_H, 0);
      schneeflocken[i].x = random(0, CANV_W);
      schneeflocken[i].xnext = schneeflocken[i].x;
    }
  }

  // Sprites auf Display schieben
  canvas.pushSprite(0, 0);
}

/*
void zeichneSchneeflocken() {
  canvas.fillScreen(TFT_BLACK);
    
  // Schneeflocken zeichnen und bewegen
  for (int i = 0; i < NUMFLAKES; i++) {

    //canvas.fillCircle(schneeflocken[i].x, schneeflocken[i].y, schneeflocken[i].size, schneeflocken[i].color);
    canvas.drawBitmap(schneeflocken[i].x, schneeflocken[i].y, schneeflocken[i].bitmap, schneeflocken[i].size, schneeflocken[i].size, schneeflocken[i].color);
    //canvas.drawPixel(schneeflocken[i].xnext, schneeflocken[i].y, TFT_RED);

    schneeflocken[i].y += schneeflocken[i].geschwindigkeit;
    
    // Zufällige Bewegung nach links oder 
    uint16_t seed = random(0, 100);

    if (seed < 10) {
      schneeflocken[i].xnext -= random(10);
    } else if (seed > 90) {
      schneeflocken[i].xnext += random(10);
    }

    // Wenn die Schneeflocke den Bildschirm verlässt, von der anderen Seite wieder erscheinen
    if (schneeflocken[i].y > CANV_H ) {
      schneeflocken[i].y = random(-CANV_H, 0);  // Von oben starten
      schneeflocken[i].x = random(0, CANV_W);
      schneeflocken[i].xnext = schneeflocken[i].x;
    }

    if (schneeflocken[i].x > CANV_W) {
      schneeflocken[i].x = 0;
    }
    if (schneeflocken[i].x <= 0) {
      schneeflocken[i].x = CANV_W;
    }

    if (schneeflocken[i].xnext > CANV_W | schneeflocken[i].xnext <= 0) {
      schneeflocken[i].xnext = schneeflocken[i].x;
    }
    
    if(schneeflocken[i].x < schneeflocken[i].xnext) {
      schneeflocken[i].x++;
    }
    if(schneeflocken[i].x > schneeflocken[i].xnext) {
      schneeflocken[i].x--;
    }
  }
  
  canvas.pushSprite(0, 0);
}
*/
// Vergleichsfunktion für std::sort
bool vergleicheSchneeflocken(const Schneeflocke &a, const Schneeflocke &b) {
  return a.size < b.size;
}

void printMemoryInfo() {

  if (psramFound())
  {
      Serial.printf("PSRAM Größe: %d bytes\n", ESP.getPsramSize());
      Serial.printf("Freier PSRAM: %d bytes\n", ESP.getFreePsram());
  }
  else
  {
      Serial.println("Kein PSRAM vorhanden");
  }

  Serial.printf("SRAM Größe: %d bytes\n", ESP.getHeapSize());
  Serial.printf("Freier SRAM: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("Largest DMA block: %d bytes\n", heap_caps_get_largest_free_block(MALLOC_CAP_DMA));

}
