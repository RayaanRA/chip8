#include "main.h"

void draw() {
	ClearBackground(BLACK);
	int pixelSize = 10;
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			int coordinate = y * 64 + x;
			if (graphics[coordinate] == 1) {
				DrawRectangle(x * pixelSize, y * pixelSize, pixelSize, pixelSize, WHITE);
			}
		}
	}
}