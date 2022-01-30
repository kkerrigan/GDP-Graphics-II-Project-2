#pragma once

#include <string>

struct sWindowSettings {
	int width;
	int height;
	const char* title;

	sWindowSettings(int width, int height, const char* title) : width(width), height(height), title(title) {}
};