#pragma once

#include "Colors.h"
#include <string>
#include <vector>

class Surface {
private:
	std::vector<Color> pPixels;
	int width;
	int height;
public:
	Surface() { ; } //This one shouldn't really be used
	Surface(std::string filename);
	Surface(int width, int height);
	Surface(const Surface& s);
	~Surface();

	Surface& operator= (const Surface& s);

	//Getters
	Color getPixel(int x, int y) const;
	int getWidth() const;
	int getHeight() const;

	//Other functions
	void setPixel(int x, int y, Color c);
};