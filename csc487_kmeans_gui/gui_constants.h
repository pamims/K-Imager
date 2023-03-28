#ifndef GUI_CONSTANTS_H
#define GUI_CONSTANTS_H

namespace gui {
	namespace sizes {
		constexpr int BOX_WIDTH = 14;
		constexpr int MARGIN = 20;
		constexpr int VSPACE = 7;
		constexpr int ELEMENT_HEIGHT = 25;
		constexpr int LARGE_ELEMENT_HEIGHT = 45;
		constexpr int LABEL_WIDTH = 60;
		constexpr int HSPACE = 10;
		constexpr int FILENAME_WIDTH = 140;
		constexpr int BROWSE_WIDTH = 50;
		constexpr int SELECTOR_WIDTH = FILENAME_WIDTH + HSPACE + BROWSE_WIDTH;
		constexpr int BUTTON_WIDTH = SELECTOR_WIDTH + HSPACE + LABEL_WIDTH;
		constexpr int CHECKBOX_WIDTH = LABEL_WIDTH + HSPACE + BOX_WIDTH;
		constexpr int WIDTH = 2 * MARGIN + BUTTON_WIDTH;
		constexpr int HEIGHT = 2 * MARGIN + 5 * VSPACE + 5 * ELEMENT_HEIGHT + LARGE_ELEMENT_HEIGHT;
	}
}

namespace gui {
	namespace strings {
		const char* TITLE = "K-Imager";
		const char* CLASS = "mainwindow";
		const char* FILE = "File";
		const char* MODE = "Mode";
		const char* COLORS = "Colors";
		const char* RUN = "RUN";
		const char* SAVE = "SAVE";
		const char* BROWSE = "browse";
		const char* GREYSCALE = "Greyscale";
		const char* DEFAULT_FILENAME_TEXT = "Select an image file  ";

		namespace combobox {
			const char* KMEANS = "k-means";
			const char* KMEANS_UNWEIGHTED = "unweighted k-means";
			const char* KMEANSPP = "k-means++";
			const char* KMEANSPP_UNWEIGHTED = "unweighted k-means++";
		}
	}
}

namespace gui {
	namespace styles {
		constexpr DWORD MAINWINDOW = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
		constexpr DWORD LABEL = SS_LEFT | SS_CENTERIMAGE | WS_VISIBLE | WS_CHILD;
		constexpr DWORD FILENAME = ES_RIGHT | ES_AUTOHSCROLL | ES_READONLY | WS_VISIBLE | WS_CHILD | WS_BORDER;
		constexpr DWORD UPDOWNBUDDY = ES_CENTER | ES_READONLY | ES_NUMBER | WS_VISIBLE | WS_CHILD | WS_BORDER;
		constexpr DWORD UPDOWN = UDS_ALIGNRIGHT | UDS_SETBUDDYINT | WS_VISIBLE | WS_CHILD;
		constexpr DWORD COMBOBOX = CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE;
		constexpr DWORD BUTTON = BS_PUSHBUTTON | BS_TEXT | BS_CENTER | BS_VCENTER | WS_VISIBLE | WS_CHILD | WS_BORDER;
		constexpr DWORD CHECKBOX = BS_AUTOCHECKBOX | BS_TEXT | BS_LEFTTEXT | BS_LEFT | WS_CHILD | WS_VISIBLE;
	}
}

namespace gui {
	enum class ID : int {
		FILE = 500,
		FILENAME,
		BROWSE,
		MODE,
		COMBOBOX,
		COLORS,
		UPDOWN,
		RUN,
		SAVE,
		CHECKBOX
	};
}

#endif
