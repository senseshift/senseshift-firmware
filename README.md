# OpenHaptics Firmware

This project intended to let people build their own DIY haptic-feedback devices. Integrates to all existing games, that work with bHaptics.

Build your own haptic vest, haptic gloves and more!

[![Discord](https://img.shields.io/discord/966090258104062023?label=Discord&logo=discord)](https://discord.gg/YUtRKAqty2)
[![Developer's Twitter](https://img.shields.io/twitter/follow/leon0399?color=%231DA1F2&label=Developer%27s%20Twitter&logo=twitter)](https://twitter.com/leon0399)

[![PlatformIO CI](https://github.com/openhaptics/openhaptics-firmware/actions/workflows/ci.yml/badge.svg)](https://github.com/openhaptics/openhaptics-firmware/actions/workflows/ci.yml)

[![GPL-3.0](https://img.shields.io/github/license/openhaptics/openhaptics-firmware)](/LICENSE)
[![GitHub contributors](https://img.shields.io/github/contributors/openhaptics/openhaptics-firmware)](https://github.com/openhaptics/openhaptics-firmware/graphs/contributors)
[![GitHub](https://img.shields.io/github/stars/openhaptics/openhaptics-firmware.svg)](https://github.com/openhaptics/openhaptics-firmware)
[![Contributor Covenant](https://img.shields.io/badge/code_of_conduct-contributor_covenant_v2.1-ff69b4)](/CODE_OF_CONDUCT.md)


## Configuration, flashing and connecting

* [Firmware Documentation](https://openhaptics.github.io/)
* [Hardware Documentation](https://github.com/openhaptics/openhaptics-hardware)

Please note, that documentation is still work-in-progress

## Supported Hardware

### MCU

| Board                | Status       |
| :------------------- | :----------: |
| ESP32-WROOM-32       | ✅            |
| ESP32-WROVER         | ✅            |
| ESP32-C3             | ❌            |
| ESP32-S3             | ❌            |

> **Note**  
> Please, refer to [Documentation on MCUs](https://openhaptics.github.io/docs/hardware/mcu) for more detailed info

### PWM Drivers

| Board                | Status       | Max outputs per board |
| :------------------- | :----------: | ---------------------: |
| Internal PWM         | ✅            |                    16 |
| PCA9685              | ✅            |                    16 |
| PCA9634              | 🚧            |                     8 |
| PCA9531              | 🚧            |                     8 |
| PCA9635              | 🚧            |                    16 |
| SN3218               | 🚧            |                    18 |
| MP3326               | 🚧            |                    16 |
| TLC59108F            | 🚧            |                     8 |
| LP50xx               | 🚧            |             9 _or_ 12 |

### Battery controllers

| Board                | Status       |
| :------------------- | :----------: |
| Internal ADC         | 🚧            |
| MAX1704x             | 🚧            |
| INA219               | 🚧            |
| MCP3021              | 🚧            |
| STUSB4500            | 🚧            |

## Supported Protocols

| Protocol             | Status                                                              |
| :------------------- | :-----------------------------------------------------------------: |
| bHaptics Bluetooth   | ✅ (Android/Windows/Quest)                                           |
| OpenHaptics          | [RFC](https://github.com/openhaptics/openhaptics-firmware/issues/9) |

## Supported devices

| Device                 | Supported Devices    | Retail price | Hardware                                                                                            |
| :--------------------  | :------------------- | -----------: | :-------------------------------------------------------------------------------------------------- |
| Haptics Face Interface | Tactal               | US $149      | See [Hardware Reference](https://github.com/openhaptics/openhaptics-hardware#haptic-face-interface) |
| Haptic Gloves          | TactGlove            | US $299      | PWM on pins `32`, `33`, `25`, `26`, `27`, `14`                                                      |
| Haptic Sleeves         | Tactosy for arms     | US $249      | See [Hardware Reference](https://github.com/openhaptics/openhaptics-hardware#haptic-forearm-sleeve) |
| Haptic Hand Gauntlet   | Tactosy for hands    | US $249      | See [Hardware Reference](https://github.com/openhaptics/openhaptics-hardware#haptic-gauntlet)       |
| Haptic Feet Device     | Tactosy for feet     | US $249      | See [Hardware Reference](https://github.com/openhaptics/openhaptics-hardware#haptic-feet-device)    |
| X16 Haptic Vest        | TactSuit X16         | US $299      | See [Hardware Reference](https://github.com/openhaptics/openhaptics-hardware#x16-haptic-vest)       |
| X40 Haptic Vest        | TactSuit X40, Tactot | US $499      | See [Hardware Reference](https://github.com/openhaptics/openhaptics-hardware#x40-haptic-vest)       |
