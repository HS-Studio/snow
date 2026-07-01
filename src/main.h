#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
//#include "LGFX_SPI_ILI9341.h" //Display-Konfiguration
#include "LGFX_SPI_ST7789.h"
#include "esp_heap_caps.h"

// Canvas-Größe
#define CANV_W 240
#define CANV_H 280

// Schneeflocken-Parameter
#define NUMFLAKES 250
#define MAX_FLOCKEN_SIZE 12
#define NUM_SNOWFLAKES 12

// Globale Variablen
extern LGFX tft;
extern LGFX_Sprite canvas;

struct Schneeflocke {
  int16_t x;
  int16_t y;
  uint16_t xnext;

  uint8_t size;
  int8_t geschwindigkeit;
  uint16_t color;
  const uint8_t *bitmap;
};

// Array für Schneeflocken
Schneeflocke schneeflocken[NUMFLAKES];

// Funktionsdeklarationen
//void zeichneSchneeflocken(const uint8_t *bitmap[], uint16_t w, uint16_t h);
void zeichneSchneeflocken();
bool vergleicheSchneeflocken(const Schneeflocke &a, const Schneeflocke &b);

void printMemoryInfo();

#endif
//EOF