# OpenHaptics Firmware

This project intended to let people build their own DIY haptic-feedback devices. Integrates to all existing games, that work with bHaptics

[![Discord](https://img.shields.io/discord/966090258104062023?label=Discord&logo=discord)](https://discord.gg/YUtRKAqty2)
[![Developer's Twitter](https://img.shields.io/twitter/follow/leon0399?color=%231DA1F2&label=Developer%27s%20Twitter&logo=twitter&style=plastic)](https://twitter.com/leon0399)

<a href="/LICENSE"><img alt="GPL-V3.0 License" src="https://img.shields.io/github/license/openhaptics/openhaptics-firmware.svg"></a>
<a href="https://github.com/MarlinFirmware/Marlin/graphs/contributors"><img alt="Contributors" src="https://img.shields.io/github/contributors/openhaptics/openhaptics-firmware.svg"></a>

## Configuration, flashing and connecting

See [`/docs`](/docs/)

Please note, that documentation is still work-in-progress

## Supported Boards

| Board                | Status       |
| :------------------- | :----------- |
| ESP-WROOM-32         | Tested       |

## Supported Protocols

| Protocol           | Supported                   |
| :----------------- | :-------------------------: |
| bHaptics Bluetooth | Yes (Android/Windows/Quest) |

## Supported devices

| Device              | Required Pins                                                                                           |
| :------------------ | :------------------------------------------------------------------------------------------------------ |
| Tactal              | PWM on pins `32`, `33`, `25`, `26`, `27`, `14`                                                          |
| TactGlove (L and R) | PWM on pins `32`, `33`, `25`, `26`, `27`, `14`                                                          |
| Tactosy for arms    | PWM on pins `32`, `33`, `25`, `26`, `27`, `14`                                                          |
| Tactosy for hands   | PWM on pins `32`, `33`, `25`                                                                            |
| Tactosy for feet    | PWM on pins `32`, `33`, `25`                                                                            |
| X16 Haptic Vest     | PWM on pins `32`, `33`, `25`, `26`, `27`, `14`, `12`, `13`, `19`, `18`, `5`, `17`, `16`, `4`, `2`, `15` |
| X16 Haptic Vest     | PCA9685 on address `0x40`                                                                               |
| X40 Haptic Vest     | 2x PCA9685 (on address `0x40` and `0x41`) + PWM on pins `32`, `33`, `25`, `26`, `27`, `14`, `12`, `13`  |
