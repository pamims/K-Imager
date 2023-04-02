#include "csc487_kmeans_gui.h"

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <CommCtrl.h>
#include <shobjidl.h>
#include <windowsx.h>

#include "CC_Constants.h"
#include "gui_constants.h"
#include "resource.h"

#include "CC_Interface.h"

#define UWM_INITFILE (WM_APP + 7)


namespace gui {
	/* -- FORWARD DECLARATIONS -- */
	void SetInputFile(HWND hWnd, std::optional<LPWSTR> input_file);
	bool AddWindowElements(HWND hWnd);
	bool AddLabels(HWND hWnd, HFONT hFont);
	bool AddFileName(HWND hWnd, HFONT hFont);
	bool AddComboBox(HWND hWnd, HFONT hFont);
	bool AddUpDown(HWND hWnd, HFONT hFont);
	bool AddButtons(HWND hWnd, HFONT hNormalFont);

	LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	std::optional<std::string> GetOpenFileName();
	std::optional<std::string> GetSaveFileName();
	std::optional<std::string> pwstr_to_string(PWSTR ws);
	bool has_compatible_filetype(const std::string& filename);
	bool has_ending(const std::string& string, const std::string& ending);
	void DisableControls(HWND hWnd);
	void EnableMinimumControls(HWND hWnd);
	void EnableRunControls(HWND hWnd);
	void EnableAllControls(HWND hWnd);
	void ErrorMessage(const char* message);

	/* -- FUNCTION DEFINITIONS -- */
	bool RegisterMainWindow(HINSTANCE hInstance) {
		WNDCLASSEXA window_class = { 0 };

		window_class.cbSize = sizeof(WNDCLASSEXA);
		window_class.lpfnWndProc = WindowProcess;
		window_class.hInstance = hInstance;
		window_class.lpszClassName = gui::strings::CLASS;
		window_class.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		window_class.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));;

		ATOM result = RegisterClassExA(&window_class);
		return (result != INVALID_ATOM);
	}

	bool CreateMainWindow(HINSTANCE hInstance, int nCmdShow, std::optional<LPWSTR> input_file) {

		RECT window_rect = { 0, 0, gui::sizes::WIDTH, gui::sizes::HEIGHT };
		AdjustWindowRect(&window_rect, gui::styles::MAINWINDOW, FALSE);
		const int width = window_rect.right - window_rect.left;
		const int height = window_rect.bottom - window_rect.top;
		SystemParametersInfoA(SPI_GETWORKAREA, 0, (PVOID) & window_rect, 0);
		const int x = (window_rect.right - window_rect.left) / 2 - width / 2;
		const int y = (window_rect.bottom - window_rect.top) / 2 -  height / 2;
		HWND hWnd = CreateWindowExA(
			0, gui::strings::CLASS, gui::strings::TITLE, gui::styles::MAINWINDOW,
			x, y, width, height,
			NULL, NULL, hInstance, NULL
		);

		if (hWnd == FALSE) {
			return false;
		}

		bool elements_are_added = AddWindowElements(hWnd);
		if (!elements_are_added) {
			return false;
		}

		SetInputFile(hWnd, input_file);

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
		return true;
	}

	bool has_compatible_filetype(const std::string& filename) {
		return (
			has_ending(filename, ".jpeg") ||
			has_ending(filename, ".jpg")  ||
			has_ending(filename, ".png")  ||
			has_ending(filename, ".bmp")
		);
	}

	bool has_ending(const std::string& string, const std::string& ending) {
		if (string.length() <= ending.length()) { // if it's the exact same length, then it has no beginning --> bad file name
			return false;
		}

		const size_t start_index = string.length() - ending.length();
		const size_t num_chars = ending.length();

		return (string.compare(start_index, num_chars, &ending[0]) == 0);
	}

	void SetInputFile(HWND hWnd, std::optional<LPWSTR> input_file) {
		if (!input_file.has_value()) {
			return;
		}

		auto result = pwstr_to_string(*input_file);
		if (!result.has_value()) {
			return;
		}

		std::string filename = *result;
		if (!has_compatible_filetype(filename)) {
			return;
		}


		HWND hFileNameTextBox = GetDlgItem(hWnd, (int)gui::ID::FILENAME);
		SetWindowTextA(hFileNameTextBox, filename.c_str());
		SendMessageA(hWnd, UWM_INITFILE, (WPARAM)0, (LPARAM)0);
		return;
	}

	int Run() {
		MSG message;
		while (GetMessageA(&message, nullptr, 0, 0)) {
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}

		return static_cast<int>(message.wParam);
	}

	bool AddWindowElements(HWND hWnd) {
		HFONT normal_font = CreateFontA(
			18, 0, 0, 0, FW_REGULAR,
			FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			CLEARTYPE_NATURAL_QUALITY, FF_MODERN, "Segoe UI"
		);

		if (normal_font == NULL) {
			return false;
		}

		bool labels_are_added = AddLabels(hWnd, normal_font);
		if (!labels_are_added) {
			return false;
		}

		bool file_name_added = AddFileName(hWnd, normal_font);
		if (!file_name_added) {
			return false;
		}

		bool combobox_added = AddComboBox(hWnd, normal_font);
		if (!combobox_added) {
			return false;
		}

		bool updown_added = AddUpDown(hWnd, normal_font);
		if (!updown_added) {
			return false;
		}

		bool buttons_are_added = AddButtons(hWnd, normal_font);
		if (!buttons_are_added) {
			return false;
		}

		return true;
	}

	bool AddLabels(HWND hWnd, HFONT hFont) {

		const int x = gui::sizes::MARGIN;
		int y = gui::sizes::MARGIN;
		const int delta_y = gui::sizes::ELEMENT_HEIGHT + gui::sizes::VSPACE;
		const int width = gui::sizes::LABEL_WIDTH;
		const int height = gui::sizes::ELEMENT_HEIGHT;
		const int num_labels = 3;

		const char* const titles[num_labels] = {
			gui::strings::FILE,
			gui::strings::MODE,
			gui::strings::COLORS
		};

		const gui::ID IDs[num_labels] = {
			gui::ID::FILE,
			gui::ID::MODE,
			gui::ID::COLORS
		};

		for (int i = 0; i < num_labels; i++) {
			HWND label = CreateWindowExA(
				0, WC_STATICA, titles[i], gui::styles::LABEL,
				x, y, width, height, hWnd, (HMENU)IDs[i],
				(HINSTANCE)GetWindowLongPtrA(hWnd, GWLP_HINSTANCE), NULL
			);
			if (label == NULL) {
				return false;
			}
			SendMessageA(label, WM_SETFONT, (WPARAM)hFont, TRUE);
			y += delta_y;
		}
		return true;
	}

	bool AddFileName(HWND hWnd, HFONT hFont) {

		const int x = gui::sizes::MARGIN + gui::sizes::LABEL_WIDTH + gui::sizes::HSPACE;
		int y = gui::sizes::MARGIN;
		const int width = gui::sizes::FILENAME_WIDTH;
		const int height = gui::sizes::ELEMENT_HEIGHT;
		const int num_labels = 3;

		HWND label = CreateWindowExA(
			WS_EX_CLIENTEDGE, WC_EDITA, gui::strings::DEFAULT_FILENAME_TEXT, gui::styles::FILENAME,
			x, y, width, height, hWnd, (HMENU)gui::ID::FILENAME,
			(HINSTANCE)GetWindowLongPtrA(hWnd, GWLP_HINSTANCE), NULL
		);
		if (label == NULL) {
			return false;
		}
		SendMessage(label, WM_SETFONT, (WPARAM)hFont, TRUE);

		return true;
	}

	bool AddComboBox(HWND hWnd, HFONT hFont) {
		const int x = gui::sizes::MARGIN + gui::sizes::LABEL_WIDTH + gui::sizes::HSPACE;
		const int y = gui::sizes::MARGIN + gui::sizes::ELEMENT_HEIGHT + gui::sizes::VSPACE;
		const int width = gui::sizes::SELECTOR_WIDTH;
		const int height = 8 * gui::sizes::ELEMENT_HEIGHT;

		HWND combobox = CreateWindowExA(
			0, WC_COMBOBOXA, NULL, gui::styles::COMBOBOX,
			x, y, width, height, hWnd, (HMENU)gui::ID::COMBOBOX,
			(HINSTANCE)GetWindowLongPtrA(hWnd, GWLP_HINSTANCE),
			NULL);
		if (combobox == NULL) {
			return false;
		}

		using namespace gui::strings::combobox;

		const char* const items[4] = { KMEANS, KMEANSPP, KMEANS_UNWEIGHTED, KMEANSPP_UNWEIGHTED };
		for (const char* item : items) {
			SendMessageA(combobox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)item);
		}

		SendMessageA(combobox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		SendMessageA(combobox, WM_SETFONT, (WPARAM)hFont, TRUE);

		return true;
	}

	bool AddUpDown(HWND hWnd, HFONT hFont) {
		const int x = gui::sizes::MARGIN + gui::sizes::LABEL_WIDTH + gui::sizes::HSPACE;
		const int y = gui::sizes::MARGIN + 2 * gui::sizes::ELEMENT_HEIGHT + 2 * gui::sizes::VSPACE;
		const int width = gui::sizes::SELECTOR_WIDTH;
		const int height = gui::sizes::ELEMENT_HEIGHT;

		HWND buddy = CreateWindowExA(
			WS_EX_CLIENTEDGE, WC_EDITA, NULL, gui::styles::UPDOWNBUDDY,
			x, y, width, height, hWnd, NULL,
			(HINSTANCE)GetWindowLongPtrA(hWnd, GWLP_HINSTANCE),
			NULL
		);
		if (buddy == NULL) {
			return false;
		}

		HWND updown = CreateWindowExA(
			0, UPDOWN_CLASSA, NULL, gui::styles::UPDOWN,
			0, 0, 0, 0, hWnd, (HMENU)gui::ID::UPDOWN,
			(HINSTANCE)GetWindowLongPtrA(hWnd, GWLP_HINSTANCE),
			NULL
		);
		if (updown == NULL) {
			return false;
		}

		SendMessageA(buddy, WM_SETFONT, (WPARAM)hFont, TRUE);

		const UINT max = cc::default_max_k;
		const UINT min = cc::default_min_k;
		const UINT default_value = cc::default_k;

		SendMessageA(updown, UDM_SETBUDDY, (WPARAM)buddy, (LPARAM)0);
		SendMessageA(updown, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(max, min));
		SendMessageA(updown, UDM_SETPOS, (WPARAM)0, (LPARAM)default_value);

		return true;
	}

	bool AddButtons(HWND hWnd, HFONT hNormalFont) {
		HFONT run_font = CreateFontA(
			40, 0, 0, 0, FW_BOLD,
			FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			CLEARTYPE_NATURAL_QUALITY, FF_MODERN, "Segoe UI"
		);
		HFONT save_font = CreateFontA(
			20, 0, 0, 0, FW_BOLD,
			FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			CLEARTYPE_NATURAL_QUALITY, FF_MODERN, "Segoe UI"
		);
		HFONT browse_font = CreateFontA(
			14, 0, 0, 0, FW_REGULAR,
			FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			CLEARTYPE_NATURAL_QUALITY, FF_MODERN, "Segoe UI"
		);

		int x = gui::sizes::MARGIN;
		int y = gui::sizes::MARGIN + gui::sizes::ELEMENT_HEIGHT * 3 + gui::sizes::VSPACE * 3;

		HWND checkbox = CreateWindowExA(
			0, WC_BUTTONA, gui::strings::GREYSCALE, gui::styles::CHECKBOX,
			x, y, gui::sizes::CHECKBOX_WIDTH, gui::sizes::ELEMENT_HEIGHT,
			hWnd, (HMENU)gui::ID::CHECKBOX, (HINSTANCE)GetWindowLongPtrA(hWnd, GWLP_HINSTANCE),
			NULL
		);
		if (checkbox == NULL) {
			return false;
		}
		SendMessageA(checkbox, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
		Button_SetCheck(checkbox, FALSE);

		y += gui::sizes::ELEMENT_HEIGHT + gui::sizes::VSPACE;
		HWND run_button = CreateWindowExA(
			0, WC_BUTTONA, gui::strings::RUN, gui::styles::BUTTON,
			x, y, gui::sizes::BUTTON_WIDTH, gui::sizes::LARGE_ELEMENT_HEIGHT,
			hWnd, (HMENU)gui::ID::RUN, (HINSTANCE)GetWindowLongPtrA(hWnd, GWLP_HINSTANCE),
			NULL
		);
		if (run_button == NULL) {
			return false;
		}
		SendMessageA(run_button, WM_SETFONT, (WPARAM)run_font, TRUE);
		EnableWindow(run_button, FALSE);

		y += gui::sizes::LARGE_ELEMENT_HEIGHT + gui::sizes::VSPACE;
		HWND save_button = CreateWindowExA(
			0, WC_BUTTONA, gui::strings::SAVE, gui::styles::BUTTON,
			x, y, gui::sizes::BUTTON_WIDTH, gui::sizes::ELEMENT_HEIGHT,
			hWnd, (HMENU)gui::ID::SAVE, (HINSTANCE)GetWindowLongPtrA(hWnd, GWLP_HINSTANCE),
			NULL
		);
		if (save_button == NULL) {
			return false;
		}
		SendMessageA(save_button, WM_SETFONT, (WPARAM)save_font, TRUE);
		EnableWindow(save_button, FALSE);

		y = gui::sizes::MARGIN;
		x = gui::sizes::MARGIN + gui::sizes::LABEL_WIDTH + 2 * gui::sizes::HSPACE + gui::sizes::FILENAME_WIDTH;
		HWND browse_button = CreateWindowExA(
			0, WC_BUTTONA, gui::strings::BROWSE, gui::styles::BUTTON,
			x, y, gui::sizes::BROWSE_WIDTH, gui::sizes::ELEMENT_HEIGHT,
			hWnd, (HMENU)gui::ID::BROWSE, (HINSTANCE)GetWindowLongPtrA(hWnd, GWLP_HINSTANCE),
			NULL
		);
		if (browse_button == NULL) {
			return false;
		}
		SendMessageA(browse_button, WM_SETFONT, (WPARAM)browse_font, TRUE);

		return true;
	}
	

	LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
		static std::unique_ptr<cc::CC_Interface> cc_interface;

		switch (message)
		{
		case WM_CREATE:
		{
			HICON hIcon = (HICON)GetClassLongPtrA(window, GCLP_HICON);
			cc_interface = std::make_unique<cc::CC_Interface>(hIcon);
		}
		return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(window, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_MENU + 1));

			EndPaint(window, &ps);
		}
		return 0;

		case UWM_INITFILE:
		{
			HWND hFileNameTextBox = GetDlgItem(window, (int)gui::ID::FILENAME);
			int size = GetWindowTextLengthA(hFileNameTextBox) + 1;
			std::vector<char> vFileName;
			vFileName.resize(size);
			LPSTR lpFileName = &vFileName[0];
			int result = GetWindowTextA(hFileNameTextBox, lpFileName, size);

			if (result == 0) {
				break;
			}

			std::string filename(lpFileName);
			if (has_compatible_filetype(filename)) {
				cc_interface->ImageLoad(filename.c_str());

				if (cc_interface->ImageIsValid() && !cc_interface->ImageIsTooLarge()) {
					cc_interface->Display();
					EnableRunControls(window);
					break;
				}
				else {
					SetWindowTextA(hFileNameTextBox, gui::strings::DEFAULT_FILENAME_TEXT);
					break;
				}
			}
		}
		return 0;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case (int)ID::RUN:
			{
				// Don't let user spam buttons
				DisableControls(window);

				// Get handles to necessary information
				HWND hK = GetDlgItem(window, (UINT)gui::ID::UPDOWN);
				HWND hAlgorithm = GetDlgItem(window, (UINT)gui::ID::COMBOBOX);
				HWND hGreyscale = GetDlgItem(window, (UINT)gui::ID::CHECKBOX);

				// Set value of k to selected value
				LRESULT result = SendMessageA(hK, UDM_GETPOS, (WPARAM)0, (LPARAM)0);
				unsigned int k = LOWORD(result);

				// Get index and length of currently selected combobox item
				size_t item_index = (size_t)SendMessageA(hAlgorithm, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				size_t length = (size_t)SendMessageA(hAlgorithm, CB_GETLBTEXTLEN, (WPARAM)item_index, (LPARAM)0);

				// Get the buffer to store the combobox item string in
				std::vector<char> algorithm_vector;
				algorithm_vector.resize(length, '\0');
				char* algo_c_str = &algorithm_vector[0];

				// Place selected combobox item string into the buffer
				SendMessageA(hAlgorithm, CB_GETLBTEXT, (WPARAM)item_index, (LPARAM)algo_c_str);

				// Get the specific algorithm ID for the selected Mode
				bool had_error = false;
				cc::AlgorithmType algorithm_type = cc::AlgorithmType::kMeans;
				if (strncmp(algo_c_str, gui::strings::combobox::KMEANS, length) == 0) {
					algorithm_type = cc::AlgorithmType::kMeans;
				}
				else if (strncmp(algo_c_str, gui::strings::combobox::KMEANS_UNWEIGHTED, length) == 0) {
					algorithm_type = cc::AlgorithmType::kMeansUnweighted;
				}
				else if (strncmp(algo_c_str, gui::strings::combobox::KMEANSPP, length) == 0) {
					algorithm_type = cc::AlgorithmType::kMeansPP;
				}
				else if (strncmp(algo_c_str, gui::strings::combobox::KMEANSPP_UNWEIGHTED, length) == 0) {
					algorithm_type = cc::AlgorithmType::kMeansUnweightedPP;
				}
				else {
					ErrorMessage("Selected Mode is unavailable :(");
					had_error = true;
				}
				
				UINT check_state = (UINT)SendMessageA(hGreyscale, BM_GETCHECK, (WPARAM)0, (LPARAM)0);
				bool make_greyscale = (check_state == BST_CHECKED);

				// Process the image if there was no error.
				if (!had_error) {
					SetDlgItemTextA(window, (int)ID::RUN, "PROCESSING...");
					cc_interface->Process(k, algorithm_type, make_greyscale);
					cc_interface->Display();
					SetDlgItemTextA(window, (int)ID::RUN, gui::strings::RUN);
				}

				// Re-enable the controls
				EnableAllControls(window);
			}
			break;


			case (int)ID::SAVE:
			{
				DisableControls(window);
				if (cc_interface->CanSave()) {

					auto dialog_result = GetSaveFileName();
					std::string filename;

					if (dialog_result.has_value()) {

						filename = *dialog_result;
						if (!filename.empty()) {

							cc_interface->Save(filename.c_str());
							EnableAllControls(window);
							break;
						}
						else {
							ErrorMessage("The file name provided was empty. Cannot save a file with an empty name.");
						}
					}
					else {
						//ErrorMessage("This program cannot handle unicode characters. Please use ASCII characters.");
					}
				}
				else {
					ErrorMessage("Image data is empty, cannot save image.");
					EnableRunControls(window);
					break;
				}

				EnableAllControls(window);
			}
			break;


			case (int)ID::BROWSE:
			{
				DisableControls(window);
				cc_interface->Close();

				auto dialog_result = GetOpenFileName();

				std::string filename;
				if (dialog_result.has_value()) {

					filename = *dialog_result;
					if (!filename.empty()) {

						cc_interface->ImageLoad(filename.c_str());
						HWND hFileNameTextBox = GetDlgItem(window, (UINT)gui::ID::FILENAME);

						if (cc_interface->ImageIsValid() && !cc_interface->ImageIsTooLarge()) {

							SetWindowTextA(hFileNameTextBox, filename.c_str());
							cc_interface->Display();
							EnableRunControls(window);
							break;
						}
						else {
							SetWindowTextA(hFileNameTextBox, gui::strings::DEFAULT_FILENAME_TEXT);
							ErrorMessage("Image could not be properly loaded or is too large. Please try a different image.");
						}
					}
					else {
						ErrorMessage("The file name provided is empty. Cannot open a file with an empty name.");
					}
				}
				else {
					// User cancelled or sent bad filename
				}

				EnableMinimumControls(window);
			}
			break;
			}

			return 0;
		}

		return DefWindowProcA(window, message, wParam, lParam);
	}

	std::optional<std::string> GetOpenFileName() {

		std::optional<std::string> return_value = std::nullopt;
		std::string error_message = "";

		HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(result)) {
			IFileOpenDialog* dialog = nullptr;

			result = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&dialog));
			if (SUCCEEDED(result)) {

				DWORD flags;
				dialog->GetOptions(&flags);
				dialog->SetOptions(flags | FOS_FORCEFILESYSTEM);

				COMDLG_FILTERSPEC fileTypes[] =
				{
					{ L"Supported images", L"*.png;*.jpg;*.jpeg;*.bmp;" },
					{ L"png", L"*.png;" },
					{ L"jpeg", L"*.jpeg;" },
					{ L"bmp", L"*.bmp;" }
				};
				dialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

				result = dialog->Show(NULL);
				if (SUCCEEDED(result)) {

					IShellItem* item = nullptr;
					result = dialog->GetResult(&item);

					if (SUCCEEDED(result)) {
						PWSTR filename = nullptr;
						result = item->GetDisplayName(SIGDN_FILESYSPATH, &filename);
						
						if (SUCCEEDED(result)) {
							return_value = pwstr_to_string(filename);

							if (!return_value.has_value()) {
								ErrorMessage("This program cannot handle unicode characters.\nPlease rename the file and try again.");
							}

							CoTaskMemFree(filename);
						}
						else {
							error_message = "Could not get open file name from shell item.";
						}
						item->Release();
					}
					else {
						error_message = "Could not get open shell item.";
					}
				}
				else {
					//error_message = "Could not show FileOpenDialog.";
				}
				dialog->Release();
			}
			else {
				error_message = "Could not create instance of FileOpenDialog.";
			}
			CoUninitialize();
		}
		else {
			error_message = "Could not initialize COM.";
		}
		if (!error_message.empty()) {
			ErrorMessage(error_message.c_str());
		}
		return return_value;
	}

	std::optional<std::string> GetSaveFileName() {

		std::optional<std::string> return_value = std::nullopt;
		std::string error_message = "";

		HRESULT result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(result)) {
			IFileSaveDialog* dialog = nullptr;

			result = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&dialog));
			if (SUCCEEDED(result)) {

				DWORD flags;
				dialog->GetOptions(&flags);
				dialog->SetOptions(flags | FOS_FORCEFILESYSTEM);
				dialog->SetFileName(L"image.png");
				dialog->SetDefaultExtension(L"png");
				COMDLG_FILTERSPEC fileTypes[] =
				{
					{ L"png", L"*.png;" },
					{ L"jpeg", L"*.jpeg;" },
					{ L"bmp", L"*.bmp;" }
				};
				dialog->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

				result = dialog->Show(NULL);
				if (SUCCEEDED(result)) {

					IShellItem* item = nullptr;
					result = dialog->GetResult(&item);

					if (SUCCEEDED(result)) {
						PWSTR filename = nullptr;
						result = item->GetDisplayName(SIGDN_FILESYSPATH, &filename);

						if (SUCCEEDED(result)) {
							return_value = pwstr_to_string(filename);
							if (!return_value.has_value()) {
								error_message = "This program cannot handle unicode characters. Please use ASCII characters.";
							}
							CoTaskMemFree(filename);
						}
						else {
							error_message = "Could not get save file name from shell item.";
						}
						item->Release();
					}
					else {
						error_message = "Could not get save shell item.";
					}
				}
				else {
					//error_message = "Could not show FileOpenDialog.";
				}
				dialog->Release();
			}
			else {
				error_message = "Could not create instance of FileSaveDialog.";
			}
			CoUninitialize();
		}
		else {
			error_message = "Could not initialize COM.";
		}
		if (!error_message.empty()) {
			ErrorMessage(error_message.c_str());
		}
		return return_value;
	}

	std::optional<std::string> pwstr_to_string(PWSTR pwstr) {
		std::string result;
		result.reserve(wcslen(pwstr));
		constexpr WCHAR compatibility_mask = std::numeric_limits<uint8_t>::max();

		for (; *pwstr; pwstr++) {
			WCHAR temp = *pwstr & compatibility_mask;
			if (temp == *pwstr) {
				result += static_cast<char>(*pwstr);
			}
			else return std::nullopt;
		}
		return result;
	}

	void DisableControls(HWND hWnd) {
		std::vector<HWND> controls;
		controls.push_back(GetDlgItem(hWnd, (UINT)gui::ID::RUN));
		controls.push_back(GetDlgItem(hWnd, (UINT)gui::ID::SAVE));
		controls.push_back(GetDlgItem(hWnd, (UINT)gui::ID::BROWSE));
		controls.push_back(GetDlgItem(hWnd, (UINT)gui::ID::COMBOBOX)); // does nothing?
		controls.push_back(GetDlgItem(hWnd, (UINT)gui::ID::UPDOWN));
		controls.push_back(GetDlgItem(hWnd, (UINT)gui::ID::CHECKBOX));

		for (HWND control : controls) {
			EnableWindow(control, FALSE);
		}

		// Disable the close-program [x] button
		HMENU hSysMenu = GetSystemMenu(hWnd, FALSE);
		EnableMenuItem(hSysMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
	}

	void EnableMinimumControls(HWND hWnd) {
		std::vector<HWND> controls;
		controls.push_back(GetDlgItem(hWnd, (UINT)gui::ID::BROWSE));
		controls.push_back(GetDlgItem(hWnd, (UINT)gui::ID::COMBOBOX)); // does nothing?
		controls.push_back(GetDlgItem(hWnd, (UINT)gui::ID::UPDOWN));
		controls.push_back(GetDlgItem(hWnd, (UINT)gui::ID::CHECKBOX));

		for (HWND control : controls) {
			EnableWindow(control, TRUE);
		}

		// Enable the close-program [x] button
		HMENU hSysMenu = GetSystemMenu(hWnd, FALSE);
		EnableMenuItem(hSysMenu, SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
	}

	void EnableRunControls(HWND hWnd) {
		HWND run_button = GetDlgItem(hWnd, (UINT)gui::ID::RUN);
		EnableWindow(run_button, TRUE);
		EnableMinimumControls(hWnd);
	}

	void EnableAllControls(HWND hWnd) {
		HWND save_button = GetDlgItem(hWnd, (UINT)gui::ID::SAVE);
		EnableWindow(save_button, TRUE);
		EnableRunControls(hWnd);
	}

	void ErrorMessage(const char* message) {
		MessageBeep(MB_ICONASTERISK);
		MessageBoxA(NULL, message, "Error", MB_OK);
		return;
	}

	//End namespace
}
