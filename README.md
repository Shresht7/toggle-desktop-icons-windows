# `toggle-desktop-icons-windows`

A small utility to toggle desktop icon visibility on Windows.

## Usage

Simply run the binary to toggle the desktop icons

```
. .\toggle-desktop-icons.exe
```

> [!TIP]
>
> I have setup PowerToys Keyboard Manager run this executable on with a keyboard shortcut.

## Development

### Compilation

```sh
g++ main.cpp -o toggle-desktop-icons.exe  -municode -mwindows
```

`-municode`: to support wide-strings (unicode)
`-mwindows`: because we use `wWinMain()` to setup a GUI application instead of a console application

## References

- https://stackoverflow.com/questions/6402834/how-to-hide-desktop-icons-programmatically
- https://techcommunity.microsoft.com/discussions/windows10space/showhide-desktop-icons-with-a-single-click/2016194
- https://www.autohotkey.com/boards/viewtopic.php?t=75890

## License

This project is licensed under the [MIT License](./LICENSE)
