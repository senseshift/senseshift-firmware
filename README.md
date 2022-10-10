# OpenHaptics Firmware

This project intended to let people build their own DIY haptic-feedback devices. Integrates to all existing games, that work with bHaptics

[![Discord](https://img.shields.io/discord/966090258104062023?label=Discord&logo=discord)](https://discord.gg/YUtRKAqty2)
[![Developer's Twitter](https://img.shields.io/twitter/follow/leon0399?color=%231DA1F2&label=Developer%27s%20Twitter&logo=twitter&style=plastic)](https://twitter.com/leon0399)

[![GPL-3.0](https://img.shields.io/github/license/openhaptics/openhaptics-firmware)](/LICENSE)
[![GitHub contributors](https://img.shields.io/github/contributors/openhaptics/openhaptics-firmware)](https://github.com/openhaptics/openhaptics-firmware/graphs/contributors)

## Configuration, flashing and connecting

See [`/docs`](/docs)

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

| Device              | Image                                                                                                              | Retail price | Required Pins                                                                                                                           |
| :------------------ | :----------------------------------------------------------------------------------------------------------------- | -----------: | :-------------------------------------------------------------------------------------------------------------------------------------- |
| Tactal              | ![Tactal](https://user-images.githubusercontent.com/1759654/194772735-bd81ab12-dcbe-4d56-81df-4218d3648f18.png)    | US $149      | PWM on pins `32`, `33`, `25`, `26`, `27`, `14`                                                                                          |
| TactGlove (L and R) | ![TactGlove](https://user-images.githubusercontent.com/1759654/194772997-fbfd21e9-738b-45aa-9f9a-dab97618d394.png) | (?) US 299$  | PWM on pins `32`, `33`, `25`, `26`, `27`, `14`                                                                                          |
| Tactosy for arms    | ![Tactosy](https://user-images.githubusercontent.com/1759654/194772773-eecb7cf6-b9a2-450d-a05c-572f08fd6322.png)   | US $249      | PWM on pins `32`, `33`, `25`, `26`, `27`, `14`                                                                                          |
| Tactosy for hands   | ![Tactosy](https://user-images.githubusercontent.com/1759654/194772788-db903633-c1e1-4847-bb7c-fd86755bd2dd.png)   | US $249      | PWM on pins `32`, `33`, `25`                                                                                                            |
| Tactosy for feet    | ![Tactosy](https://user-images.githubusercontent.com/1759654/194772782-59982313-c2e7-4c28-866e-9dee9c3ed2a1.png)   | US $249      | PWM on pins `32`, `33`, `25`                                                                                                            |
| X16 Haptic Vest     | ![X16 Vest](https://user-images.githubusercontent.com/1759654/194772823-14402aa0-70bf-4534-9e17-0e09eb6c7315.png)  | US $299      | PWM on pins `32`, `33`, `25`, `26`, `27`, `14`, `12`, `13`, `19`, `18`, `5`, `17`, `16`, `4`, `2`, `15` *OR* PCA9685 on address `0x40`  |
| X40 Haptic Vest     | ![X40 Vest](https://user-images.githubusercontent.com/1759654/194772830-45f22c36-42a3-4cda-96b1-81769d20babd.png)  | US $499      | 2x PCA9685 (on address `0x40` and `0x41`) + PWM on pins `32`, `33`, `25`, `26`, `27`, `14`, `12`, `13`                                  |
