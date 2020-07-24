// sidebareditor.cpp
//
#include	"imgui.h"
#include "function.h"
#include "image.h"

bool Debug_LockScroll = false;

void SidebarEditor::init() {
	metalplt = LoadEditorImage("metalplt.shp");

	mainmenu_selection = -1;
}

Image_t* SidebarEditor::LoadEditorImage(const char* name) {
	const void* shapefile = MFCD::Retrieve(name);
	if(shapefile == NULL) {
		return NULL;
	}

	int width = Get_Build_Frame_Width(shapefile);
	int height = Get_Build_Frame_Height(shapefile);	
	unsigned char *shape_pointer = (unsigned char*)Build_Frame(shapefile, 0, _ShapeBuffer);
	short frameCount = Get_Build_Frame_Count(shapefile);

	Image_t* shape_image = Image_CreateImageFrom8Bit(name, width, height, shape_pointer);
	for(int i = 1; i < frameCount; i++) {
		shape_pointer = (unsigned char*)Build_Frame(shapefile, i, _ShapeBuffer);
		Image_Add8BitImage(shape_image, 0, i, width, height, shape_pointer, NULL);
	}

	return  shape_image;
}

void SidebarEditor::draw_it(void) {
	bool toolActive;	

	ImGuiStyle& style = ImGui::GetStyle();
	style.FramePadding = ImVec2(0, 0);

	if (Debug_RenderEditorSplash)
	{
		static Image_t* edwinBackground = Image_LoadImage("ui/edwin/eng_edhi.png");
		GL_RenderImage(edwinBackground, 0, 0, ScreenWidth, ScreenHeight);
	}

	Debug_LockScroll = false;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			Debug_LockScroll = true;
			if (ImGui::MenuItem("New Scenario")) { mainmenu_selection = 0; }
			if (ImGui::MenuItem("Load Scenario")) { mainmenu_selection = 1; }
			if (ImGui::MenuItem("Save Scenario")) { mainmenu_selection = 2; }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Map"))
		{
			Debug_LockScroll = true;
			if (ImGui::MenuItem("Size Map")) { mainmenu_selection = 3; }
			if (ImGui::MenuItem("Scenario Options")) { mainmenu_selection = 5; }
			if (ImGui::MenuItem("AI Options")) { mainmenu_selection = 6; }
			ImGui::EndMenu();
		}		

		if (ImGui::BeginMenu("Objects"))
		{
			Debug_LockScroll = true;
			if (ImGui::MenuItem("Add Game Object")) { mainmenu_selection = 4; }
			if (ImGui::MenuItem("Add Big Overlay Object")) { mainmenu_selection = 9; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Lights"))
		{
			Debug_LockScroll = true;
			if (ImGui::MenuItem("Add Light")) { mainmenu_selection = 8; }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Run"))
		{
			Debug_LockScroll = true;
			if (ImGui::MenuItem("Play Scenario")) { mainmenu_selection = 7; }
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}


	//ImGui::SetNextWindowSize(ImVec2(160, ScreenHeight));
	//ImGui::SetNextWindowPos(ImVec2(ScreenWidth-160, 0));
	//ImGui::Begin("EditorSidebar", &toolActive, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
	//	// Map Preview
	//	//ImGui::Image((ImTextureID)metalplt->GetImageForHouse(0, 2), ImVec2(160, 192));
	//
	//	
	//
	//ImGui::End();
}