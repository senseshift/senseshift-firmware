# Debugging

## Debugging in Wokwi

Run and debug firmware in Wokwi Simulator

1. Run this command:
   ```shell
   pio debug -e lucidgloves-prototype3
   ```
2. `Ctrl+Shift+P` => `> Wokwi: Start Simulator and Wait for Debugger`,
3. Add launch option (this step is required until PlatformIO fixes this issue: [#3824](https://github.com/platformio/platformio-core/issues/3824)): 
   ```json
   ...
   
    {
        "name": "Wokwi GDB - lucidgloves-prototype3",
        "type": "cppdbg",
        "request": "launch",
        "program": "${workspaceFolder}/.pio/build/lucidgloves-prototype3/firmware.elf",
        "cwd": "${workspaceFolder}",
        "MIMode": "gdb",
        "miDebuggerPath": "${userHome}/.platformio/packages/toolchain-xtensa-esp32/bin/xtensa-esp32-elf-gdb",
        "miDebuggerServerAddress": "localhost:3333",
    },
   ...
   ```

## Useful Scripts

### Fix `clang-format`

```shell
find lib include firmware test examples -type f -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' ! -regex '^lib/frozen\(/.*\)' -exec clang-format-16 -style=file -i {} \;
```
