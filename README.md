[简体中文](README_zh-CN.md)

# vfd-test

A small project for driving an 8-digit, 14-segment VFD with an **STC89C52RC**.

The original goal was simple: make a VFD test board that lights up, displays characters, and accepts text over a USB serial port. The board has been built and tested, so the firmware and schematics are shared here for reference and for anyone who enjoys VFDs, boost converters, and classic 8051 microcontrollers.

![VFD board](vfd.jpg)

## What it can do

- Drive an 8-digit, 14-segment VFD
- Use three TBD62783AFG driver arrays for the grid/anode and segment lines
- Scan the display and receive serial data with an STC89C52RC
- Provide a USB serial port through a CH340N and USB-C connector
- Include an RTS-based automatic cold-start circuit for one-click programming with STC-ISP
- Use `9600 8N1` as the default serial format
- Receive ASCII characters and scroll them to the left using the built-in font table

In short: plug in USB, flash the firmware, open a serial terminal, and start typing.

## Repository layout

```text
.
├── vfd-test-firmware/       # Keil C51 project and STC89C52RC firmware
│   ├── main.c
│   └── Objects/              # Generated HEX and other build files
├── vfd-test-pcb/             # Schematic project, backups, and schematic PDF
├── vfd.jpg                   # Photo of the assembled board
└── LICENSE                   # MIT License
```

## Hardware overview

The main parts are:

- MCU: STC89C52RC-40I-PDIP40
- USB-to-serial bridge: CH340N
- VFD drivers: TBD62783AFG × 3
- Grid/anode boost converter: LMR64010
- Filament driver: DRV8837
- 8051 crystal: 11.0592 MHz
- Display: 8-digit, 14-segment VFD (D1319WF in this project)

## Voltage settings

These are the working points that turned out to be suitable for this particular board and tube. They are starting points, not universal values; re-check them when using a different VFD or power circuit.

- Voltage across the two filament terminals: approximately **2 Vrms**
- Grid and anode voltage relative to system ground: approximately **30 V**
- Filament voltage relative to system ground: approximately **25 V**

The 25 V value is not the voltage across the filament. It is the filament's DC operating point relative to system ground, with an approximately 5 V DC bias superimposed on the filament drive. This gives the filament and the grid/anode a more suitable potential relationship and helps reduce VFD ghosting. When debugging, distinguish carefully between the voltage across the filament and the voltage from the filament to ground.

The boost section gets hot during operation, and its output capacitors can remain charged. Turn the board off and allow the capacitors to discharge before probing or changing wiring.

## VFD segment mapping

### Conventional seven-segment part

| Segment | VFD pin |
| --- | --- |
| a | P4 |
| b | P3 |
| c | P14 |
| d | P5 |
| e | P6 |
| f | P11 |
| g (left half) | P12 |
| g (right half) | P13 |

### Starburst and center vertical segments

| Position | VFD pin |
| --- | --- |
| Upper-left diagonal | P10 |
| Lower-left diagonal | P7 |
| Lower-right diagonal | P9 |
| Upper-right diagonal | P2 |
| Upper center vertical | P1 |
| Lower center vertical | P8 |

The corresponding MCU connections are:

- `P3.7 ~ P3.2` correspond to VFD `P14 ~ P9`
- `P2.7 ~ P2.0` correspond to VFD `P1 ~ P8`

The font table is in [`vfd-test-firmware/main.c`](vfd-test-firmware/main.c). If the segment wiring changes, this is the main place to update.

## Using the firmware

After startup, the firmware scans all eight display positions. Each character received over serial is inserted into the display buffer and shifts the existing text to the left.

On Linux, minicom can be used as follows:

```bash
minicom -D /dev/ttyUSB0 -b 9600
```

The serial settings are:

```text
Baud rate: 9600
Data bits: 8
Parity: None
Stop bits: 1
Flow control: None
```

Whether typed characters are echoed is controlled by the terminal program. The firmware mainly receives characters and displays them; it does not promise to echo them back to the host.

## Building and programming

The firmware is a Keil C51 project. Open:

```text
vfd-test-firmware/vfd-test-firmware.uvproj
```

After building, the generated file can be found at:

```text
vfd-test-firmware/Objects/vfd-test-firmware.hex
```

Use an STC89C52RC-compatible programmer to flash the HEX file. The board includes an automatic cold-start circuit driven by the USB-UART chip's RTS signal, so STC-ISP can enter programming mode, erase, and program the MCU with one click—no manual power cycling required.

If you use a different programmer, its cold-start and serial wiring may be different. Follow that programmer's instructions.

## Ideas for future work

- Add brightness control and a display enable switch
- Add a more complete Unicode or Chinese dot-matrix extension
- Split the font table and display scanner into reusable modules
- Improve boost and filament-driver efficiency, thermal performance, and EMI
- Add test points and an enclosure to the PCB

## Safety notes

This is a first-revision experimental board. Its parameters and layout were tuned around the VFD on hand. Although the VFD supply is only in the tens of volts, the boost converter, capacitors, and drive waveforms can still cause burns, shorts, or component damage.

Do not touch the grids, anodes, filament driver, or related test points while the circuit is operating. The human body conducts electricity: touching the circuit can conduct grid/anode voltage through the body to the MCU and make it behave abnormally. It can also conduct interference or voltage into the filament-supply LDO, raising the filament voltage and, in a serious case, making the filament glow red. Power the board off and let the capacitors discharge before measuring or rewiring it. When using an oscilloscope, pay close attention to where the probe ground clip is connected.

If you recreate this board, adjust the filament voltage, grid/anode voltage, and current-limiting values for your own VFD.

## License

This project is released under the [MIT License](LICENSE). You are free to use, modify, and redistribute it, provided that the original copyright and license notices are retained. See the `LICENSE` file for the complete terms.
