// bigoverlay.h
//

#include <vector>

#define MAX_WORLD_BIGOVERLAYS			2300 // This is just a random number, increase as you want.

struct Image_t;

//
// BigOverlayType_t
//
struct BigOverlayType_t {
	Image_t* image;
};


class BigOverlay {
friend class BigOverlayManager;
public:
	BigOverlay();
	int GetIndex(void) { return index; }
	void SetImage(int index, Image_t* image) { this->index = index; this->image = image; }
	void Place(int screenx, int screeny);
	bool GetRenderPosition(int& x, int& y);
	void draw_it(void);
	Image_t* GetImage(void) { return image; }
	void SetEnabled(bool enabled) { this->enabled = enabled; }
	COORDINATE GetPosition() { return position; }
protected:
	void SetPosition(COORDINATE position) { this->position = position; }
	void SetOverlayNum(int overlayNum) { this->overlayNum = overlayNum; }
	bool IsEnabled(void) { return enabled; }	
private:
	int overlayNum;
	COORDINATE position;
	Image_t* image;
	bool enabled;
	int index;
};

class BigOverlayManager {
public:
	BigOverlayManager();

	// Init's all the big overlay types.
	void Init(void);

	// Resets all big overlays.
	void Reset(void);

	// Render's all the active big overlays.
	void Render(void);

	void Read_Scenerio(CCINIClass* ini);
	void Write_Scenerio(CCINIClass* ini);

	// Adjusts the tile image.
	void AdjustTile(BigOverlay* bigOverlay, int direction);

	// Allocates a big overlay
	BigOverlay*	 AllocateBigOverlay(void);

	BigOverlay *Editor_PlaceNewBigOverlay(void);
private:	
	void RegisterBigOverlay(int theater, const char* name);

	BigOverlay bigOverlays[MAX_WORLD_BIGOVERLAYS];
	std::vector<BigOverlayType_t> bigOverlayTypes;

	int editor_selectedbigoverlay;
};

extern BigOverlayManager bigOverlayManager;