// sidebareditor.h
//

struct Image_t;

class SidebarEditor {
public:
	void				init(void);
	void				draw_it(void);

	int					mainmenu_selection;
private:
	Image_t*			LoadEditorImage(const char* name);
private:
	Image_t*			metalplt;
};

extern bool Debug_LockScroll;