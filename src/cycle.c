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
			switch(opcode) {
				case 0x00E0:
					for (int i = 0; i < (64 * 32); i++) {
						graphics[i] = 0;
					}
					pc += 2;
					break;
				case 0x00EE:
					sp--;
					pc = stack[sp];
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
				pc += 2;
			}
			pc += 2;
		case 0x5:
			if (V[X] == V[Y]) {
				pc += 2;
			}
			pc += 2;
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
				case 0:
					V[X] = V[Y];
					pc += 2;
					break;
				case 1:
					V[X] |= V[Y];
					pc += 2;
					break;
				case 2:
					V[X] &= V[Y];
					pc += 2;
					break;
				case 3:
					V[X] ^= V[Y];
					pc += 2;
					break;
				case 4:
					uint16_t sum = V[X] + V[Y];
					if (sum > 255) {
						V[0xF] = 1;
					} else {
						V[0xF] = 0;
					}

					V[X] += V[Y];
					pc += 2;
					break;
				case 5:
					if (V[X] >= V[Y]) {
						V[0xF] = 1;
					}
					if (V[X] < V[Y]) {
						V[0xF] = 0;
					}
					V[X] -= V[Y];
					pc += 2;
					break;
				case 6:
					V[X] = V[Y];
					V[0xF] = V[X] && 0x1;
					V[X] >>= 1;
					pc += 2;
					break;
				case 7:
					if (V[X] >= V[Y]) {
						V[0xF] = 1;
					}
					if (V[X] < V[Y]) {
						V[0xF] = 0;
					}
					V[X] = V[Y] - V[X];
					pc += 2;
					break;
				case 0xE:
					V[X] = V[Y];
					V[0xF] = V[X] && 0x1;
					V[X] <<= 1;
					pc += 2;
					break;
			}
			break;
		case 0x9:
			if (V[X] != V[Y]) {
				pc += 2;
			}
			pc += 2;
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

	}
}