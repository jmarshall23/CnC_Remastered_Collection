// launcher_main.cpp
//

#include <windows.h>

int (*WINAPI CnCMain)(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HMODULE handle = LoadLibrary("redalertx64.dll");
	if (handle == NULL)
		return 0;
	
	CnCMain = (int(__cdecl *)(HINSTANCE, HINSTANCE, LPSTR, int))GetProcAddress(handle, "CnCMain");
	if (CnCMain == NULL)
		return 0;

	return CnCMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}