#include "csc487_kmeans_gui.h"
#include <shellapi.h>
#include <optional>


int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pCmdLine, _In_ int nCmdShow) {

    // Get command line arguments, set input file if exists
    LPWSTR lpRealCmdLine = GetCommandLineW();
    std::optional<LPWSTR> input_file;

    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(lpRealCmdLine, &argc);

    if (argc == 2) {
        input_file = argv[1];
    }

    // Build the main window, run the program
    bool is_registered = gui::RegisterMainWindow(hInstance);
    if (!is_registered) {
        return 1;
    }
    bool is_created = gui::CreateMainWindow(hInstance, nCmdShow, input_file);
    if (!is_created) {
        return 5;
    }
    int result = gui::Run();

    return result;
}
