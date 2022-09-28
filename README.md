# Openhaptics Firmware

This project intended to let people build their own DIY haptic-feedback devices

[![Discord](https://img.shields.io/discord/966090258104062023?label=Discord&logo=discord)](https://discord.gg/YUtRKAqty2)

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
