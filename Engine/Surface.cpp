#include "Surface.h"
#include "ChiliWin.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include "MyException.h"

Surface::Surface(std::string filename) {
	try {
		std::ifstream file(filename, std::ios::binary);

		if (!file) {
			throw(MyException("Fisierul nu este valid!"));
		}

		BITMAPFILEHEADER bmFileHeader;
		file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

		BITMAPINFOHEADER bmInfoHeader;
		file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

		if (bmInfoHeader.biBitCount != 24) {
			throw(MyException("Fisierul trebuie sa aiba bitcount-ul 24!"));
		}
		if (bmInfoHeader.biCompression != BI_RGB) {
			throw(MyException("Fisierul trebuie sa aiba compresia BI_RGB!"));
		}

		const bool is32b = (bmInfoHeader.biBitCount == 32);

		width = bmInfoHeader.biWidth;

		int yStart, yEnd, dy;
		if (bmInfoHeader.biHeight < 0) {
			height = -bmInfoHeader.biHeight;
			yStart = 0;
			yEnd = height;
			dy = 1;
		}
		else {
			height = bmInfoHeader.biHeight;
			yStart = height - 1;
			yEnd = -1;
			dy = -1;
		}

		pPixels.resize(width*height);

		file.seekg(bmFileHeader.bfOffBits);
		//Padding is only for 24 bitcount
		const int padding = (4 - (width * 3) % 4) % 4;

		for (int y = yStart; y != yEnd; y += dy) {
			for (int x = 0; x < width; x++) {

				setPixel(x, y, Color(file.get(), file.get(), file.get()));
				if (is32b) { file.seekg(1, std::ios::cur); }
			}
			if (!is32b) { file.seekg(padding, std::ios::cur); }
		}
	}
	catch (...) {
		throw(MyException("Fisierul nu este valid!"));
	}
}
Surface::Surface(int width, int height) {
	this->width = width;
	this->height = height;
	this->pPixels.resize(width*height);
}
Surface::Surface(const Surface& s){
	width = s.width;
	height = s.height;
	pPixels.resize(s.width*s.height);
	for (int i = 0; i < s.width*s.height; i++) {
		pPixels[i] = s.pPixels[i];
	}
}
Surface::~Surface() {
	pPixels.clear();
}

Surface& Surface::operator= (const Surface& s) {
	pPixels.clear();
	
	width = s.width;
	height = s.height;

	this->pPixels.resize(width*height);
	
	for (int i = 0; i < s.width*s.height; i++) {
		pPixels[i] = s.pPixels[i];
	}

	return *this;
}

//Getters
Color Surface::getPixel(int x, int y) const{ 
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);

	return pPixels[y*width + x];
}
int Surface::getWidth() const{
	return width;
}
int Surface::getHeight() const{
	return height;
}

//Other functions
void Surface::setPixel(int x, int y, Color c) {
	
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);

	pPixels[y*width + x] = c;
}
