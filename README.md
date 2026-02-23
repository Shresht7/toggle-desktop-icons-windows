# `toggle-desktop-icons-windows`

A small utility to toggle desktop icon visibility on Windows.

![Icon](./resources/icon.ico)

## Usage

Simply run the binary to toggle the desktop icons

```
. .\toggle-desktop-icons.exe
```

> [!TIP]
>
> You can create a shortcut and place it in the `$HOME\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\` folder to run it using the Start Menu. I recommend creating a `Shortcuts` folder there to keep all your shortcuts organized.

## How it works

Windows does not provide a public API for toggling desktop icon visibility. Instead, this utility uses an undocumented `WM_COMMAND` message that Explorer handles internally; the same one triggered when you right-click the desktop and choose **`View → Show desktop icons`**.

### Finding the right window

Desktop icons are rendered by a `ListView` control inside a window called `SHELLDLL_DefView`, which is managed by Explorer. To send it a command, we first need its handle.

`SHELLDLL_DefView` is always a child of a window with class `Progman` (the Program Manager, which owns the desktop background). However, when the **Show Desktop** command has been used or wallpaper slideshow is enabled, Explorer inserts an intermediate `WorkerW` window between `Progman` and `SHELLDLL_DefView`. So we check both:

1. Look for `SHELLDLL_DefView` as a direct child of `Progman`
2. If not found, enumerate all `WorkerW` windows and check each one

### Sending the toggle command

Once we have the `SHELLDLL_DefView` handle, we send it `WM_COMMAND` with the undocumented command ID `0x7402`:

```cpp
const int TOGGLE_DESKTOP_ICONS = 0x7402;
SendMessage(defView, WM_COMMAND, (WPARAM)TOGGLE_DESKTOP_ICONS, 0);
```

This immediately toggles icon visibility. Explorer also persists the new state to the registry at:

```
HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced\HideIcons
```

(`0` = visible, `1` = hidden)

### Why not use the registry directly?

Writing to `HideIcons` directly is possible, but changing the registry value alone does not update the desktop visually; Explorer must be notified separately, which requires either restarting it (disruptive) or additional Win32 calls. The `SendMessage` approach both applies the change instantly and updates the registry automatically.

## Development

### Compilation

To compile the `resources.rc` file:

```sh
windres resources.rc -O coff -o resources.o
```

To compile the main executable (and link the resources object):

```sh
g++ -municode -mwindows main.cpp resources.o -o toggle-desktop-icons.exe
```

- `-municode`: to support wide-strings (unicode)
- `-mwindows`: because we use `wWinMain()` to setup a GUI application instead of a console application

## References

- https://stackoverflow.com/questions/6402834/how-to-hide-desktop-icons-programmatically
- https://techcommunity.microsoft.com/discussions/windows10space/showhide-desktop-icons-with-a-single-click/2016194
- https://www.autohotkey.com/boards/viewtopic.php?t=75890
- https://github.com/TikoTako/iconHider

## License

This project is licensed under the [MIT License](./LICENSE)
