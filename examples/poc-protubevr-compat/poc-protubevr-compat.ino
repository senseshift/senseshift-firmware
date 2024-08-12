#include "BluetoothSerial.h"

#define PIN_RUMBLE 32
#define CHA_RUMBLE 1

#define PIN_KICK 33
#define CHA_KICK 2

const int resolution = 8;
const int maxOutput = (1 << resolution) - 1;

const int rumbleFreq = 8000;
const int kickFreq = 60;
const int minKick = (maxOutput / 4) * 3;

constexpr inline const bool checkSN(const uint32_t sn) {
  return (sn % 48991) % 521 == 0;
}

const uint32_t generateSN(const uint32_t i) {
  // TODO: I'm to lazy to write it normally, and since it is invoked only once on boot it is OK
  uint32_t sn = i;
  while (!checkSN(sn)) sn++;
  return sn;
}

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

template<typename _Tp>
constexpr inline _Tp simpleMap(_Tp x, _Tp in_max, _Tp out_max) {
  return x * out_max / in_max;
}

void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  // Serial.printf("BT Callback: %d\n", event);
}

void setup() {
  Serial.begin(115200);

  uint64_t chipId = ESP.getEfuseMac();
  const uint32_t sn = generateSN(chipId);

  Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("Chip ID: %u\n", chipId);
  Serial.printf("SN: %u\n", sn);

  char deviceName[64];
  sprintf(deviceName, "ForceTubeVR %u", sn);
  // TODO: fix not working name
  // SerialBT.begin(deviceName);  // Bluetooth device name
  SerialBT.begin("ForceTubeVR 1035102167");
  Serial.println("The device started, now you can pair it with bluetooth!");

  pinMode(PIN_RUMBLE, OUTPUT);
  pinMode(PIN_KICK, OUTPUT);

  ledcSetup(CHA_RUMBLE, rumbleFreq, resolution);
  ledcAttachPin(PIN_RUMBLE, CHA_RUMBLE);

  ledcSetup(CHA_KICK, kickFreq, resolution);
  ledcAttachPin(PIN_KICK, CHA_KICK);

  SerialBT.register_callback(btCallback);
}

#define COMMAND_START 0x2a
#define MAGIC 0xb0
#define CHANNEL_KICK 0x00
#define CHANNEL_RUMBLE 0x01

uint8_t payload[4];

void loop() {
  if (!SerialBT.available()) {
    return;
  }

  size_t s = SerialBT.readBytes(payload, 4);
  if (s == 0) return;

  Serial.printf("%03u %03u %03u %03u", payload[0], payload[1], payload[2], payload[3]);

  if (payload[0] != COMMAND_START) {
    Serial.println(" !!! not starting with COMMAND_START");
    return;
  }
  switch (payload[1]) {
    case MAGIC:
      if (payload[2] == CHANNEL_KICK) {
        const auto receivedVal = payload[3];
        uint16_t val = receivedVal == 0 ? 0 : 255;
        Serial.printf(" - kick %03u\n", val);
        // if (val == 0) {
        //   delay(100);
        // }
        ledcWrite(CHA_KICK, val);
      } else if (payload[2] == CHANNEL_RUMBLE) {
        uint16_t val = simpleMap<uint16_t>(payload[3], 255, maxOutput);
        ledcWrite(CHA_RUMBLE, val);
        Serial.printf(" - rumble %03u\n", val);
      } else Serial.print(" - unknown effect\n");
      break;
    default:
      Serial.println(" !!! Unknown 2nd value, not MAGIC");
      break;
  }
        delay(5);
}
