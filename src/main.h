#ifndef __MAIN__
#define __MAIN__

#include <stdio.h>
#include <stdint.h>
#include "raylib.h"

extern uint8_t memory[4096];
extern uint16_t pc;
extern uint16_t ir;
extern uint16_t stack[16];
extern uint8_t dTimer;
extern uint8_t sTimer;
extern uint8_t sp;
extern uint8_t graphics[64 * 32];
extern uint8_t V[16];
extern uint8_t font[80];
extern int keys[16];
extern int screenWidth;
extern int screenHeight;


void init();

void cycle();

void draw();

#endif