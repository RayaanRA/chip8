#include "main.h"

void cycle() {
	uint16_t opcode = memory[pc] << 8 | memory[pc + 1];
	uint8_t firstNum = (opcode & 0xF000) >> 12;
	uint16_t NNN = opcode & 0x0FFF;
	uint8_t NN = opcode & 0x00FF;
	uint8_t N = opcode & 0x000F;
	uint8_t X = (opcode & 0x0F00) >> 8;
	uint8_t Y = (opcode & 0x00F0) >> 4;

	switch(firstNum) {
		case 0x0:
			switch(NN) {
				case 0xE0:
					memset(graphics, 0, sizeof(graphics));
					pc += 2;
					break;
				case 0xEE:
					if (sp > 0x0) {
						sp--;
					}
					pc = stack[sp];
					pc += 2;
					break;
			}
			break;
		case 0x1:
			pc = NNN;
			break;
		case 0x2:
			stack[sp] = pc;
			sp++;
			pc = NNN;
			break;
		case 0x3:
			if (V[X] == NN) {
				pc += 2;
			}
			pc += 2;
			break;
		case 0x4:
			if (V[X] != NN) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		case 0x5:
			if (V[X] == V[Y]) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		case 0x6:
			V[X] = NN;
			pc += 2;
			break;
		case 0x7:
			V[X] += NN;
			pc += 2;
			break;
		case 0x8:
			switch(N) {
				case 0x0:
					V[X] = V[Y];
					pc += 2;
					break;
				case 0x1:
					V[X] |= V[Y];
					V[0xF] = 0;
					pc += 2;
					break;
				case 0x2:
					V[X] &= V[Y];
					V[0xF] = 0;
					pc += 2;
					break;
				case 0x3:
					V[X] ^= V[Y];
					V[0xF] = 0;
					pc += 2;
					break;
				case 0x4:
					uint8_t vx = V[X];
					uint8_t vy = V[Y];
					V[X] += V[Y];
					uint16_t sum = vx + vy;
					V[0xF] = (sum > 255) ? 1 : 0;
					pc += 2;
					break;
				case 0x5:
					vx = V[X];
					vy = V[Y];
					V[X] = V[X] - V[Y];
					V[0xF] = (vx >= vy) ? 1 : 0;
					pc += 2;
					break;
				case 0x6:
					vy = V[Y];
					V[X] = vy >> 1;
					V[0xF] = vy & 0x1;
					pc += 2;
					break;
				case 0x7:
					vx = V[X];
					vy = V[Y];
					V[X] = V[Y] - V[X];
					V[0xF] = (vy >= vx) ? 1 : 0;
					pc += 2;
					break;
				case 0xE:
					vy = V[Y];
					V[X] = vy << 1;
					V[0xF] = (vy & 0x80) >> 7;
					pc += 2;
					break;
			}
			break;
		case 0x9:
			if (V[X] != V[Y]) {
				pc += 4;
			} else {
				pc += 2;
			}
			break;
		case 0xA:
			ir = NNN;
			pc += 2;
			break;
		case 0xB:
			pc = NNN + V[0];
			break;
		case 0xC:
			int randomNum = GetRandomValue(0, 255);
			V[X] = randomNum & NN;
			pc += 2;
			break;
		case 0xD:
			V[0xF] = 0;
			uint8_t xCoordinate = V[X] % 64;
			uint8_t yCoordinate = V[Y] % 32;
			for (int i = 0; i < N; i++) {
				uint8_t spriteByte = memory[ir + i];
				for (int j = 0; j < 8; j++) {
					if (xCoordinate + j >= 64) {
						break;	
					} 
					uint8_t bit = (spriteByte >> (7 - j)) & 1;
					int graphicsIndex = (yCoordinate + i) * 64 + (xCoordinate + j);
					if (bit == 1) {
						if (graphics[graphicsIndex] == 1) {
							V[0xF] = 1;	
						} 
						graphics[graphicsIndex] ^= 1;
					}
				}
				if (yCoordinate + i >= 32) break;
			}
			pc += 2;
			break;
		case 0xE:
			switch(NN) {
				case 0x9E:
					if (IsKeyDown(keys[V[X]])) {
						pc += 2;
					}
					pc += 2;
					break;
				case 0xA1:
					if (!IsKeyDown(keys[V[X]])) {
						pc += 2;
					}
					pc += 2;
					break;
			}
			break;
		case 0xF:
			switch(NN) {
				case 0x07:
					V[X] = dTimer;
					pc += 2;
					break;
				case 0x15:
					dTimer = V[X];
					pc += 2;
					break;
				case 0x18:
					sTimer = V[X];
					pc += 2;
					break;
				case 0x1E:
					ir += V[X];
					if (ir > 0x0FFF) {
						V[0xF] = 1;
					} else {
						V[0xF] = 0;
					}
					pc += 2;
					break;
				case 0x0A:
					bool keyPressed = false;
					for (int i = 0; i < 16; i++) {
						if (IsKeyReleased(keys[i])) {
							V[X] = i;
							keyPressed = true;
							break;
						}
					}
					if (!keyPressed) {
						return;
					}
					pc += 2;
					break;
				case 0x29:
					ir = V[X] * 5;
					pc += 2;
					break;
				case 0x33:
					int thirdDigit;
					int secondDigit;
					int firstDigit;
					thirdDigit = V[X] % 10;
					secondDigit = (V[X] / 10) % 10;
					firstDigit = V[X] / 100;
					memory[ir] = firstDigit;
					memory[ir + 1] = secondDigit;
					memory[ir + 2] = thirdDigit;
					pc += 2;
					break;
				case 0x55:
					for (int i = 0; i < X + 1; i++) {
						memory[ir + i] = V[i];
					}
					ir = ir + X + 1;
					pc += 2;
					break;
				case 0x65:
					for (int i = 0; i < X + 1; i++) {
						V[i] = memory[ir + i];
					}
					ir = ir + X + 1;
					pc += 2;
					break;
			}
			break;


	}
}