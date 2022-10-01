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
  3. Use quick actions (✔️ - Build, ➡️ - Upload, 🔌 - Monitor). Choose your default mode by clicking `Default (openhaptics-firmware) and switching your default` 
</details>

### Editing source code

If youre setup requires additional changes, select best suiting one, and edit it's source code. All firmware files are configured in `platformio.ini` and `ini/bhaptics.ini` files

 

## Methon 2: Arduino IDE

