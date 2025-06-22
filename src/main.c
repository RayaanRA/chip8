#include "main.h"


uint8_t memory[4096];
uint16_t pc;
uint16_t ir;
uint16_t stack[16];
uint8_t dTimer;
uint8_t sTimer;
uint8_t sp;
uint8_t graphics[64 * 32];
uint8_t V[16];
uint8_t font[] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

int screenWidth = 640;
int screenHeight = 320;

int fileSize;
uint8_t* fileData;



int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("USAGE: chip8.exe <ROM>");
		return 1;
	}

	fileData = LoadFileData(argv[1], &fileSize);

	if (fileSize > (4096 - 0x200)) {
		printf("ROM too large");
		return 1;
	}

	init();



	InitWindow(screenWidth, screenHeight, argv[1]);
	SetTargetFPS(60);

	float instructionTimer = 0.0f;
	float instructionInterval = 1.0f / 700.0f;



	while (!WindowShouldClose()) {
		instructionTimer += GetFrameTime();
		while (instructionTimer >= instructionInterval) {
			cycle();
			instructionTimer -= instructionInterval;
		}

		// update timers
		BeginDrawing();
		draw();
		EndDrawing();
	}

	

	CloseWindow();

	return 0;
}


void init() {
	for (int i = 0; i < 80; i++) {
		memory[i] = font[i];
	}

	pc = 0x200;
	ir = 0;
	dTimer = 0;
	sTimer = 0;
	sp = 0;

	for (int i = 0; i < 16; i++) {
		V[i] = 0;
		stack[i] = 0;
	}

	for (int i = 0; i < fileSize + 1; i++) {
		memory[0x200 + i] = fileData[i];
	}


}