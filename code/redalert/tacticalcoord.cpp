// tacticalcoord.cpp
//

#include "function.h"

DisplayTacticalCoord::DisplayTacticalCoord() {
	x = 0;
	y = 0;
}

DisplayTacticalCoord::DisplayTacticalCoord(COORDINATE coord)
{
	SetFromCoordinate(coord);	
}

COORDINATE DisplayTacticalCoord::GetCoordinate() const
{
	return coord;
}

void DisplayTacticalCoord::SetXY(int x, int y) {
	this->x = x;
	this->y = y;
	this->coord = XY_Coord(x, y);
}

void DisplayTacticalCoord::SetFromCoordinate(COORDINATE coord)
{
	this->coord = coord;
	x = Coord_X(coord);
	y = Coord_Y(coord);
}

void DisplayTacticalCoord::AddPixelXY(int x, int y) {
	int _x = this->x + Pixel_To_Lepton(x);
	int _y = this->y + Pixel_To_Lepton(y);
	SetXY(_x, _y);
}