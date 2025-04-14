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
> You can create a shortcut and place it in the `$HOME\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\` folder to run it using the Start Menu. I recommend creating a `Shortcuts` folder to keep your shortcuts organized.

> [!TIP]
>
> I have setup PowerToys Keyboard Manager to run this executable with a keyboard shortcut.

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
