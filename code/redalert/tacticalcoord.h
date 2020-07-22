// tacticalcoord.h
//

struct DisplayTacticalCoord {
	DisplayTacticalCoord();
	DisplayTacticalCoord(COORDINATE coord);

	operator DisplayTacticalCoord::COORDINATE() const { return GetCoordinate(); }
	COORDINATE GetCoordinate() const;

	void SetFromCoordinate(COORDINATE coord);
	void SetXY(int x, int y);

	int GetX() { return x; }
	int GetY() { return y; }

	void AddPixelXY(int x, int y);
private:
	int x;
	int y;
	COORDINATE coord;
};