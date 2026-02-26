# Circuit Diagram & Wiring Guide

This document describes the full wiring of the Automatic Fan Control System. The schematic was designed and simulated using **Proteus 8**.

---

## Block Diagram

```
[9V Power Supply]
       │
       ▼
[Voltage Regulator] ──── 5V ────┬──── [Arduino Uno]
                                │           │
                                │    ┌──────┼──────┬──────┐
                                │    │      │      │      │
                             [PIR] [LCD] [Relay] [Trans-
                            Sensor 16×2  Module  istor]
                                               │
                                          [Axial Fan]
                                          (12V supply)
```

---

## Pin Connection Table

### Power Supply & Voltage Regulator
| Connection | Detail |
|---|---|
| Regulator IN | 9V DC from battery |
| Regulator GND | Common circuit ground |
| Regulator OUT | 5V rail → Arduino 5V pin |
| C_in capacitor | 10µF between IN and GND |
| C_out capacitor | 0.1µF between OUT and GND |

---

### Arduino Uno
| Arduino Pin | Connected To |
|---|---|
| 5V | 5V power rail |
| GND | Common ground rail |
| A0 | PIR/IR sensor OUT pin |
| D6 | Relay IN pin (IR sketch) |
| D9 | Relay IN pin (PIR sketch) / Transistor base resistor |
| A4 | LCD I2C SDA |
| A5 | LCD I2C SCL |

> When using direct LCD (non-I2C): RS→D7, EN→D8, D4→D9, D5→D10, D6→D11, D7→D12

---

### PIR Motion Sensor
| PIR Pin | Connected To |
|---|---|
| VCC | 5V rail |
| GND | Ground rail |
| OUT | Arduino A0 |

---

### IR Sensor Module
| IR Pin | Connected To |
|---|---|
| VCC | 5V rail |
| GND | Ground rail |
| OUT | Arduino A0 |

---

### 16×2 LCD with I2C Module
| I2C Module Pin | Connected To |
|---|---|
| VCC | 5V rail |
| GND | Ground rail |
| SDA | Arduino A4 |
| SCL | Arduino A5 |

> I2C address: `0x27` (scan with I2C scanner sketch if display is blank)

---

### Relay Module
| Relay Pin | Connected To |
|---|---|
| VCC | 5V rail |
| GND | Ground rail |
| IN | Arduino D9 (PIR) / D6 (IR) |
| COM | 12V positive supply |
| NO  | Fan positive terminal |

> Fan negative terminal connects to 12V supply ground.

---

### NPN Transistor (2N2222)
| Transistor Pin | Connected To |
|---|---|
| Base | 100Ω resistor → Arduino D9 |
| Collector | Fan negative terminal |
| Emitter | Ground rail |

> A 1N4007 flyback diode is placed across the fan terminals (cathode to positive) to suppress voltage spikes.

---

### Protection Components
| Component | Placement | Purpose |
|---|---|---|
| 100Ω resistor | Arduino D9 → transistor base | Limits base current |
| 1N4007 diode | Across fan terminals | Flyback protection |
| 0.1µF capacitor | Regulator OUT to GND | Output decoupling |
| 10µF capacitor | Regulator IN to GND | Input decoupling |

---

## Simulation

The full circuit was simulated in **Proteus 8 Professional** prior to hardware construction. The simulation verified:
- PIR signal detection and relay switching
- LCD display output under motion/no-motion conditions
- Transistor switching behaviour under load

See `schematics/circuit_diagram.png` for the Proteus schematic screenshot.
