#ifndef CSC487_KMEANS_GUI_H
#define CSC487_KMEANS_GUI_H

#include <SDKDDKVer.h>
#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <optional>


namespace gui {
	/* -- FUNCTION DECLARATIONS -- */
	bool RegisterMainWindow(HINSTANCE hInstance);
	bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow, std::optional<LPWSTR> input_file);
	int Run();
}

#endif
