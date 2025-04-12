#include <windows.h> // Windows API

/// Command ID used by Explorer to toggle desktop icons
// https://stackoverflow.com/questions/6402834/how-to-hide-desktop-icons-programmatically
const int TOGGLE_DESKTOP_ICONS = 0x7402;

/// Finds the handle to the `SHELLDLL_DefView` responsible for the desktop icons.
/// It may be a child of either "Progman" or a "WorkerW" window, depending on the system.
HWND GetShellViewWindow()
{
    // Get the handle to the Program Manager (desktop background window)
    HWND progman = FindWindow(L"Progman", nullptr);

    // Try to find the `SHELLDLL_DefView` as a child of Progman
    HWND defView = FindWindowEx(progman, nullptr, L"SHELLDLL_DefView", nullptr);

    // If not found, try looking under WorkerW windows (some Windows versions use these)
    if (!defView)
    {
        HWND workerW = nullptr;
        do
        {
            // Enumerate all WorkerW windows
            workerW = FindWindowEx(nullptr, workerW, L"workerW", nullptr);
            // Try to find `SHELLDLL_DefView` under this WorkerW
            defView = FindWindowEx(workerW, nullptr, L"SHELLDLL_DefView", nullptr);
        } while (workerW && !defView);
    }

    // Return the handle to `SHELLDLL_DefView`, or nullptr if not found
    return defView;
}

/// The main entrypoint of the application
///
/// We used `wWinMain` instead of `main` to create a GUI application instead of a console application
///
/// `hInstance`: A handle (unique ID) to the current application instance
/// `hPrevInstance`: Obsolete! Always `NULL` in modern Windows
/// `lpCmdLine`: The command-line arguments as wide-string (unicode)
/// `nCmdShow`: How the window should be shown (minimized, maximized, normal etc). Used when calling `ShowWindow()`
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    HWND defView = GetShellViewWindow();
    if (defView)
    {
        SendMessage(defView, WM_COMMAND, (WPARAM)TOGGLE_DESKTOP_ICONS, 0);
    }
    return 0;
}
