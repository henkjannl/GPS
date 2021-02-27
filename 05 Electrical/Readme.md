## Interconnect PCB

Dimensions 17.5 x 22 mm

At least 5 tracks with 6 holes needed

Order from top to bottom is as implemented:

| Track | Label | ESP32 | SD Card | TFT | XT2046 | BN-180 | BMI 160 |
|:-----:|:-----:|:-----:|:-------:|:---:|:------:|:------:|:-------:|
|   1   | MISO  | IO19  | MISO    | SDO | T_DO   |        |         |
|   2   | MOSI  | IO23  | MOSI    | SDI | T_DIN  |        |         |
|   3   | SCK   | IO18  | CLK     | SCK | T_CLK  |        |         |
|   4   | 3.3V  | 3V3   | 3V3     | VCC |        | VCC    | 3V3     |
|   5   | GND   | GND   | GND     | GND |        | GND    | GND     |
