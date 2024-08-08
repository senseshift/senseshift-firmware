# Code Style

## Member naming

Local variable: `snake_case`
Global variable: `snake_case`

Class: `UpperCamelCase`
Class member: `snake_case_` (**with trailing underscore**)
ClassFunction: `camelCase`
Function argument: `snake_case`

## Useful Scripts

### Fix `clang-format`

```shell
find lib include firmware test examples -type f -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' ! -regex '^lib/frozen\(/.*\)' -exec clang-format-16 -style=file -i {} \;
```

# Debugging

## Debugging in Wokwi

Run and debug firmware in Wokwi Simulator

1. Run this command:
   ```shell
   pio debug -e opengloves_lucidgloves_proto3
   ```
2. `Ctrl+Shift+P` => `> Wokwi: Start Simulator and Wait for Debugger`,
3. Add launch option (this step is required until PlatformIO fixes this issue: [#3824](https://github.com/platformio/platformio-core/issues/3824)):
   ```json
   ...
   
    {
        "name": "Wokwi GDB - opengloves_lucidgloves_proto3",
        "type": "cppdbg",
        "request": "launch",
        "program": "${workspaceFolder}/.pio/build/opengloves_lucidgloves_proto3/firmware.elf",
        "cwd": "${workspaceFolder}",
        "MIMode": "gdb",
        "miDebuggerPath": "${userHome}/.platformio/packages/tool-xtensa-esp-elf-gdb/bin/xtensa-esp32-elf-gdb.exe",
        "miDebuggerServerAddress": "localhost:3333",
    },
   ...
   ```
