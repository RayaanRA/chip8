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
int fileSize;
uint8_t* fileData;
Sound beep;
uint8_t font[80] = {
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

int keys[16] = {
    KEY_X,    // 0x0
    KEY_ONE,  // 0x1
    KEY_TWO,  // 0x2
    KEY_THREE,// 0x3
    KEY_Q,    // 0x4
    KEY_W,    // 0x5
    KEY_E,    // 0x6
    KEY_A,    // 0x7
    KEY_S,    // 0x8
    KEY_D,    // 0x9
    KEY_Z,    // 0xA
    KEY_C,    // 0xB
    KEY_FOUR, // 0xC
    KEY_R,    // 0xD
    KEY_F,    // 0xE
    KEY_V     // 0xF
};

int screenWidth = 1280; // 64 x 20
int screenHeight = 640; // 32 x 20


int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("USAGE: chip8.exe <ROM>\n");
		return 1;
	}

	fileData = LoadFileData(argv[1], &fileSize);

	if (fileSize > (4096 - 0x200)) {
		printf("ROM too large\n");
		return 1;
	}

	InitWindow(screenWidth, screenHeight, argv[1]);
	InitAudioDevice();
	SetTargetFPS(60);

	float instructionTimer = 0.0f;
	float instructionInterval = 1.0f / 700.0f;

	init();

	while (!WindowShouldClose()) {
		instructionTimer += GetFrameTime();
		while (instructionTimer >= instructionInterval) {
			cycle();
			instructionTimer -= instructionInterval;
		}

		if (dTimer > 0) {
			dTimer--;
		}

		if (sTimer > 0) {
			PlaySound(beep);
			sTimer--;
		}
		
		BeginDrawing();
		draw();
		EndDrawing();
	}

	
	CloseAudioDevice();
	CloseWindow();
	UnloadFileData(fileData);
	UnloadSound(beep);

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
	beep = LoadSound("../../resources/beep.mp3");


}