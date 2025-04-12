#include <windows.h> // Windows API

// The main entrypoint of the application
//
// We used `wWinMain` instead of `main` to create a GUI application instead of a console application
//
// `hInstance`: A handle (unique ID) to the current application instance
// `hPrevInstance`: Obsolete! Always `NULL` in modern Windows
// `lpCmdLine`: The command-line arguments as wide-string (unicode)
// `nCmdShow`: How the window should be shown (minimized, maximized, normal etc). Used when calling `ShowWindow()`
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    MessageBox(NULL, L"Hello from GUI!", L"MyApp", MB_OK);
    return 0;
}
