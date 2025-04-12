# `toggle-desktop-icons-windows`

A small utility to toggle desktop icon visibility on Windows.

## Development

### Compilation

```sh
g++ main.cpp -o toggle-desktop-icons.exe  -municode -mwindows
```

`-municode`: to support wide-strings (unicode)
`-mwindows`: because we use `wWinMain()` to setup a GUI application instead of a console application

## License

This project is licensed under the [MIT License](./LICENSE)
