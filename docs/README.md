Table of Contents
=================

- [Table of Contents](#table-of-contents)
- [Flashing firmware](#flashing-firmware)
  - [Method 1: PlatformIO IDE](#method-1-platformio-ide)
    - [Prerequisites:](#prerequisites)
    - [Download](#download)
    - [Flashing firmware to ESP32](#flashing-firmware-to-esp32)
    - [Editing source code](#editing-source-code)
  - [Method 2: Arduino IDE](#method-2-arduino-ide)
- [Connecting to PCVR/Quest/etc.](#connecting-to-pcvrquestetc)
  - [bHaptics Player](#bhaptics-player)
    - [Tested versions](#tested-versions)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->

# Flashing firmware

## Method 1: PlatformIO IDE

### Prerequisites:

1. Install Visual Studio Code: https://code.visualstudio.com/
2. Download PlatformIO here: https://platformio.org/platformio-ide

### Download

Clone repository with git: 

```bash
git clone git@github.com:openhaptics/openhaptics-firmware.git
```

or [download as ZIP](https://github.com/openhaptics/openhaptics-firmware/archive/refs/heads/master.zip)

### Flashing firmware to ESP32

Open project directory (extract archive if needed) in VSCode with PlatformIO exension and choose desired firmware mode

<details>
  <summary>Screenshots</summary>

  ![image](https://user-images.githubusercontent.com/1759654/193428679-148f0c8f-8439-451f-8c6d-6d6be4dbdf87.png)

  1. PlatformIO IDE homepage
  2. Select desired firmware mode and run command (Build, Upload or Monitor)
  3. Use quick actions (`✔️ - Build`, `➡️ - Upload`, `🔌 - Monitor`). Choose your default mode by clicking `Default (openhaptics-firmware)` and switching your default 
</details>

### Editing source code

If youre setup requires additional changes, select best suiting one, and edit it's source code. All firmware files are configured in `platformio.ini` and `ini/bhaptics.ini` files


## Method 2: Arduino IDE

`TODO`

# Connecting to PCVR/Quest/etc.

## bHaptics Player

1. Download and launch bHaptics Player: https://www.bhaptics.com/support/download
2. Turn on Bluetooth
3. Connect to appropriate device

### Tested versions

| App        | Versions |
| :--------- | :------- |
| PC         | `1.7.1`  |
| Android    | `TBA`    |
| iOS        | `TBA`    |
| Meta Quest | `TBA`    |