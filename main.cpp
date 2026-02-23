#include <windows.h>  // Windows API
#include <shellapi.h> // For shell-related functions like `CommandLineToArgvW`
#include <cstdio>     // For `freopen` to redirect output to the console
#include <iostream>   // For I/O operations (e.g., `std::cout`)

#include "main.h" // Include the header file for function declarations

/// The name of the application binary
const char *NAME = "desktop-icons.exe";

/// A short description of the application
const char *DESCRIPTION = "A simple utility to toggle the visibility of desktop icons on Windows";

/// The version number
const char *VERSION = "v0.1.0";

/// Command ID used by Explorer to toggle desktop icons
// https://stackoverflow.com/questions/6402834/how-to-hide-desktop-icons-programmatically
const int TOGGLE_DESKTOP_ICONS = 0x7402;

/// Finds the handle to the `SHELLDLL_DefView` responsible for the desktop icons.
/// It may be a child of either "Progman" or a "WorkerW" window, depending on the system.
HWND GetShellViewWindow()
{
    // Get the handle to the Program Manager (desktop background window)
    HWND progman = FindWindow(L"Progman", nullptr);
    if (!progman)
    {
        return nullptr; // If we can't find Progman (very unlikely), we can't proceed
    }

    // Try to find the `SHELLDLL_DefView` as a child of Progman
    HWND defView = FindWindowEx(progman, nullptr, L"SHELLDLL_DefView", nullptr);

    // If not found, try looking under WorkerW windows (some Windows versions use these)
    if (!defView)
    {
        HWND workerW = nullptr;
        do
        {
            // Enumerate all WorkerW windows
            workerW = FindWindowEx(nullptr, workerW, L"WorkerW", nullptr);
            // Try to find `SHELLDLL_DefView` under this WorkerW
            defView = FindWindowEx(workerW, nullptr, L"SHELLDLL_DefView", nullptr);
        } while (workerW && !defView);
    }

    // Return the handle to `SHELLDLL_DefView`, or nullptr if not found
    return defView;
}

/// @brief Checks if desktop icons are currently visible
/// @return `true` if icons are visible, `false` otherwise
bool AreIconsVisible()
{
    DWORD value = 0;
    DWORD size = sizeof(DWORD);
    LSTATUS result = RegGetValue(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",
        L"HideIcons",
        RRF_RT_REG_DWORD,
        nullptr,
        &value,
        &size);
    if (result != ERROR_SUCCESS)
    {
        std::fprintf(stderr, "Failed to read registry value: %lu\n", result);
        return false; // Assume icons are hidden if we can't read the value
    }
    return value == 0; // If HideIcons is 0, icons are visible. If it's 1, icons are hidden.
}

/// @brief Sends the command to toggle desktop icons to the appropriate window
void SendToggleMessage()
{
    HWND defView = GetShellViewWindow();
    if (defView)
    {
        SendMessage(defView, WM_COMMAND, (WPARAM)TOGGLE_DESKTOP_ICONS, 0);
    }
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
    /// The exit status code of the application. `EXIT_SUCCESS` (0) for success, `EXIT_FAILURE` (1) for failure
    int status = EXIT_SUCCESS;

    // Attach to the parent process's console (if it exists) so we can print output there
    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
        // Redirect standard output and standard error to the console
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }

    /// @brief The command-line argument count
    int argc;
    /// @brief The command-line arguments as an array of wide strings (unicode)
    LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argv == nullptr)
    {
        std::fprintf(stderr, "Failed to parse command line arguments.\n");
        status = EXIT_FAILURE;
        return status;
    }

    if (argc < 2 || wcscmp(argv[1], L"toggle") == 0)
    {
        SendToggleMessage();
    }
    else if (wcscmp(argv[1], L"visible") == 0)
    {
        auto visible = AreIconsVisible() ? "true" : "false";
        std::cout << visible << std::endl;
    }
    else if (wcscmp(argv[1], L"show") == 0)
    {
        if (!AreIconsVisible())
        {
            SendToggleMessage();
        }
    }
    else if (wcscmp(argv[1], L"hide") == 0)
    {
        if (AreIconsVisible())
        {
            SendToggleMessage();
        }
    }
    else if (wcscmp(argv[1], L"--version") == 0 || wcscmp(argv[1], L"-v") == 0 || wcscmp(argv[1], L"version") == 0)
    {
        PrintVersion();
    }
    else if (wcscmp(argv[1], L"--help") == 0 || wcscmp(argv[1], L"-h") == 0 || wcscmp(argv[1], L"help") == 0)
    {
        PrintHelp();
    }
    else
    {
        // If the command is unrecognized, print an error message and show the help
        std::string narrowCommand(argv[1], argv[1] + wcslen(argv[1]));
        std::cerr << "Unknown command: " << narrowCommand << std::endl;
        std::cerr << std::endl;
        PrintHelp();
        status = EXIT_FAILURE;
    }

    // Free the memory allocated by CommandLineToArgvW
    LocalFree(argv);

    // Return the appropriate exit code
    return status;
}

/// Prints the version of the application
void PrintVersion()
{
    std::cout << VERSION << std::endl;
}

/// Prints the help message
void PrintHelp()
{
    std::cout << DESCRIPTION << std::endl;
    std::cout << std::endl;
    std::cout << "Usage: " << NAME << " <command>" << std::endl;
    std::cout << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  toggle  - Toggles the visibility of desktop icons" << std::endl;
    std::cout << "  show    - Shows the desktop icons" << std::endl;
    std::cout << "  hide    - Hides the desktop icons" << std::endl;
    std::cout << "  visible - Prints 'true' if desktop icons are visible, 'false' otherwise" << std::endl;
    std::cout << "  help    - Shows this help message" << std::endl;
    std::cout << "  version - Prints the application version" << std::endl;
    std::cout << std::endl;
}
