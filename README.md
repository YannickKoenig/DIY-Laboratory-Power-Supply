![image info](/pictures/Cover.png)
# DIY Laboratory Power Supply Unit
A simple, microcontroller-based PWM buck converter power supply, designed to provide precise voltage control and current measurement. The output voltage is easily adjustable using a rotary encoder for accurate and intuitive input. This project is compact, affordable, and perfect for DIY laboratory power supply applications.

## Table of Contents  
- [BOM](#bom)
- [Assembly](#assembly)  
- [Usage](#usage)  
- [Contributing](#contributing)  
- [License](#license)

## Bill of Materials (BOM)

| Item                   | Description                         | Quantity | Price   | Total   |
|------------------------|-------------------------------------|----------|---------|---------|
| KY-040                 | Rotary Encoder                      | 1        | 0,89 €  | 0,89 €  |
| IRLZ44N                | MOSFET-Transistor (TO-220)          | 1        | 0,22 €  | 0,22 €  |
| Fuse (2A)              | 5x20mm Glass Fuse                   | 1        | 0,03 €  | 0,03 €  |
| SS34                   | Schottky Diode                      | 1        | 0,02 €  | 0,02 €  |
| TM1637                 | 4-digit 7-segment LED Display       | 2        | 1,90 €  | 3,80 €  |
| DC Plug                | -                                   | 1        | 0,24 €  | 0,24 €  |
| Power Switch           | -                                   | 1        | 0,12 €  | 0,12 €  |
| TC4427                 | MOSFET Driver (SOP-8)               | 1        | 0,29 €  | 0,29 €  |
| Buck Converter PCB     | Custom PCB for Buck Converter       | 1        | 0,08 €  | 0,08 €  |
| Frontend PCB (optional)| Custom PCB for Input & Displays     | 1        | 0,80 €  | 0,80 €  |
| Raspberry Pi Pico      | Microcontroller                     | 1        | 2,00 €  | 2,00 €  |
| Electrolytic Capacitor  | 470µF                               | 1        | 0,30 €  | 0,30 €  |
| Ceramic Capacitor      | 1µF                                 | 1        | 0,10 €  | 0,10 €  |

**Total: 8,88 €**





## Assembly
1. **Order the PCBs**:
For easier and cleaner assembly, it is recommended to order the PCBs from manufacturers like JLCPCB, PCBWAY, or AllPCB. Both the Buck Converter PCB and the optional Frontend PCB can be used. Just upload the appropriate Gerber files ([buckdown-pcb](.hardware/electrical/buck-converter/Gerber), [frontend-pcb](.hardware/electrical/frontend/Gerber))
2. **Solder the components**:
Carefully solder all the required components. The project includes both SMD and THT parts, so make sure you have the right tools and technique.
3. **Flash the microcontroller**:
-   Connect the Raspberry Pi Pico to your computer while holding the **BOOTSEL** button during startup.
-   You can either compile the firmware yourself or simply upload the precompiled `.uf2` file provided in the **releases** section.

4. **Connect the wiring**:
-   Connect the **Buck Converter PCB** and the **Frontend PCB** (if used) using the following connections:
    
    -   `GND` → `GND`
        
    -   `I+` → `I_in`
        
    -   `U+` → `U_in`
        
-   Connect the output of the Buck Converter:
    
    -   `V_out+` → Positive output terminal (e.g., banana plug).
        
    -   `V_out-` → Negative output terminal (e.g., banana plug).
        

You can use banana plugs, terminal blocks, or any other suitable connectors for your output.

-   If using the **Frontend PCB**, connect both **TM1637 displays** and the **rotary encoder** to the appropriate headers.
    
-   Make sure to add a **fuse (e.g., 2A)** to protect your circuit from potential damage.
    
-   To power the **Raspberry Pi Pico**, you can either:
    
    -   Power it via **USB**, or
        
    -   Use a separate **buck converter module** (e.g., LM2596S) connected in parallel with the DC input.

## Usage
<img src="/pictures/display.gif)" alt="display" width="150"/>

To adjust the output voltage:

- Turn the rotary encoder **left** to decrease the selected digit.
- Turn it **right** to increase the selected digit.
- Press the rotary encoder to switch between digits.

You can fine-tune the PI controller parameters (`K_P` and `K_I`) experimentally in the firmware to improve voltage regulation stability and response. See [RegulatorManager.hpp](./software/RegulatorManager.hpp) for details.


## Contributing

Contributions are welcome! Feel free to fork this project, submit pull requests, or open issues for improvements and bug fixes.

## Third-Party Libraries

This project uses the following third-party software:

- **Raspberry Pi Pico SDK**  
  Official SDK for Raspberry Pi Pico microcontroller.  
  License: BSD 3-Clause  
  [https://github.com/raspberrypi/pico-sdk](https://github.com/raspberrypi/pico-sdk)

- **TM1637_lib**  
  Library for controlling TM1637 7-segment displays.  
  License: MIT  
  [https://github.com/gavinlyonsrepo/displaylib_LED_PICO](https://github.com/gavinlyonsrepo/displaylib_LED_PICO)

## License

This project is open-source and available under the [MIT License](LICENSE.md).
