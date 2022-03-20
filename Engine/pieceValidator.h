#pragma once

#include "Piece.h"
#include "MyException.h"
#include <fstream>

class pieceValidator{
public:
	void validate(Piece* p) {
		if (p->x < -1 || p->y < -1) {
			throw(MyException("Pozitia introdusa nu este corecta!"));
		}
		if (p->side < -1 || p->side > 1) {
			throw(MyException("Culoarea piesei trebuie sa fie 1 (alb), 0 (negru) sau -1 (piesa goala)"));
		}
		std::ifstream file(p->filename, std::ios::binary);
		if (!file) {
			throw(MyException("Fisierul nu este valid!"));
		}
	}
};