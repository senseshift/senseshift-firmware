# SenseShift Firmware - Open-Source VR accessories

This project is intended to let people build their own DIY haptic-feedback devices. Integrates into all existing games that work with bHaptics and OpenGloves.

Build your own DIY haptic vest, haptic gloves, and more!

<b>Get involved: [Discord](https://discord.gg/YUtRKAqty2) • [Website](https://senseshift.io) • [Issues](https://github.com/senseshift/senseshift-firmware/issues) • [Twitter](https://twitter.com/senseshiftio)</b>

[![Discord Widget](https://discord.com/api/guilds/966090258104062023/widget.png?style=banner2)](https://discord.gg/YUtRKAqty2)

[![PlatformIO CI](https://github.com/senseshift/senseshift-firmware/actions/workflows/ci.yml/badge.svg)](https://github.com/senseshift/senseshift-firmware/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/senseshift/senseshift-firmware/graph/badge.svg?token=8CEuP8vk0f)](https://codecov.io/gh/senseshift/senseshift-firmware)
[![GitHub bug issues](https://img.shields.io/github/issues/senseshift/senseshift-firmware/bug?color=%23d73a4a)](https://github.com/senseshift/senseshift-firmware/issues?q=is%3Aissue+is%3Aopen+label%3Abug)

[![GPL-3.0](https://img.shields.io/github/license/senseshift/senseshift-firmware)](/LICENSE)
[![GitHub contributors](https://img.shields.io/github/contributors/senseshift/senseshift-firmware)](https://github.com/senseshift/senseshift-firmware/graphs/contributors)
[![GitHub](https://img.shields.io/github/stars/senseshift/senseshift-firmware.svg)](https://github.com/senseshift/senseshift-firmware)
[![Contributor Covenant](https://img.shields.io/badge/code_of_conduct-contributor_covenant_v2.1-ff69b4)](/CODE_OF_CONDUCT.md)

## Configuration, flashing, and connecting

The main documentation on this project is located on [senseshift.github.io](https://senseshift.github.io/)

* [Getting Started](https://senseshift.github.io/docs/getting-started)
* **NEW** [Build Guides](https://senseshift.github.io/guides/getting-started)
* [Hardware Schematics](https://github.com/senseshift/senseshift-hardware)
* [Flashing Firmware](https://senseshift.github.io/docs/category/firmware)

Please note that documentation is still work-in-progress.

## Supported Hardware

### MCU

| Board                | Status       |
| :------------------- | :----------: |
| ESP32-WROOM-32       | ✅            |
| ESP32-WROVER         | ✅            |
| ESP32-C3             | 🚧            |
| ESP32-S3             | 🚧            |

> **Note**  
> Please, refer to [Documentation on MCUs](https://senseshift.github.io/docs/hardware/mcu) for more detailed info

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
| Internal ADC         | ✅            |
| MAX1704x             | ✅            |
| INA219               | ✅            |
| MCP3021              | 🚧            |
| STUSB4500            | 🚧            |

## Supported Protocols

| Protocol                  | Status                                                              |
| :------------------------ | :-----------------------------------------------------------------: |
| bHaptics Bluetooth LE     | ✅ (Android/Windows/Quest)                                           |
| OpenGloves Alpha-encoding | ✅ (Serial/Bluetooth Serial)                                         |
| ProTubeVR                 | 🚧 (Bluetooth)                                                       |
| SenseShift                | [RFC](https://github.com/senseshift/senseshift-firmware/issues/9)   |

## Supported devices

| Device                 | Supported Devices    | Retail price | DIY Price         | Hardware                                                                                          |
| :--------------------  | :------------------- | -----------: | ----------------: | :------------------------------------------------------------------------------------------------ |
| Haptics Face Interface | Tactal, TactVisor    | US $149      | ~$20              | See [Hardware Reference](https://github.com/senseshift/senseshift-hardware#haptic-face-interface) |
| Haptic Gloves          | TactGlove            | US $299      | ~$20              | See [Hardware Reference](https://github.com/senseshift/senseshift-hardware#haptic-glove)          |
| Haptic Sleeves         | Tactosy for arms     | US $249      | ~$20              | See [Hardware Reference](https://github.com/senseshift/senseshift-hardware#haptic-forearm-sleeve) |
| Haptic Hand Gauntlet   | Tactosy for hands    | US $249      | ~$20              | See [Hardware Reference](https://github.com/senseshift/senseshift-hardware#haptic-gauntlet)       |
| Haptic Feet Device     | Tactosy for feet     | US $249      | ~$20              | See [Hardware Reference](https://github.com/senseshift/senseshift-hardware#haptic-feet-device)    |
| X16 Haptic Vest        | TactSuit X16         | US $299      | ~$40              | See [Hardware Reference](https://github.com/senseshift/senseshift-hardware#x16-haptic-vest)       |
| X40 Haptic Vest        | TactSuit X40, Tactot | US $499      | ~$70              | See [Hardware Reference](https://github.com/senseshift/senseshift-hardware#x40-haptic-vest)       |
| VR Glove / OpenGloves  | LucidGloves          | N/A          | ~$40 &mdash; ~$80 | See [Original Wiki](https://github.com/LucidVR/lucidgloves/wiki)                                  |

## For Developers

* [Code of Conduct](./CODE_OF_CONDUCT.md)
* [Contributing Guidelines](./CONTRIBUTING.md)

## Repo Activity

![Repo Activity](https://repobeats.axiom.co/api/embed/578cddcaa2b3d076f11ac2e681289cf47276258f.svg "Repobeats analytics image")

## Credits

* [LucasVRTech](https://github.com/lucas-vrtech) of the LucidGlove project
* [JohnRThomas](https://github.com/JohnRThomas) for the his implementation of LucidGlove firmware

## Licensing

[![GPL-3.0](https://www.gnu.org/graphics/gplv3-or-later-sm.png)](./LICENSE)

This firmware is licensed under [GPL-3.0](./LICENSE). Please see the [LICENSE](./LICENSE) file for details.

SenseShift are not affiliated with bHaptics. All brands and trademarks belong to their respective owners. SenseShift is not a bHaptics-approved software, nor is it associated with bHaptics.

SenseShift are not affiliated with ProTubeVR. All brands and trademarks belong to their respective owners. SenseShift is not a ProTubeVR-approved software, nor is it associated with ProTubeVR.
