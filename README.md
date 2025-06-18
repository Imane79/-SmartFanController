# Smart Fan Control System using PIC16F877A

This project is a temperature-based control system built using the PIC16F877A microcontroller. It turns a fan and buzzer on or off depending on the ambient temperature. The system is designed to help automate simple cooling control in environments where temperature monitoring is needed.

## Project Overview

- The system reads temperature data using an LM35 sensor.
- If the temperature crosses a predefined threshold (default is 20°C), the fan and buzzer are activated.
- The current temperature and system status are displayed on a 16x2 LCD.
- A push button allows the user to increase the temperature threshold manually. Once the threshold goes above 35°C, it wraps back to 20°C.

## Features

- Real-time temperature reading and display
- Automatic fan and buzzer control
- Manual threshold adjustment with a button
- LCD interface for user feedback
- Uses interrupts for responsive button handling

## Hardware Used

- PIC16F877A Microcontroller
- LM35 Temperature Sensor
- 16x2 LCD (in 4-bit mode)
- Mini DC fan or LED (simulated cooling output)
- Buzzer
- Push button
- Power supply (5V regulated)
- Resistors, wires, breadboard, or simulation in Proteus

## Software and Tools

- MPLAB X IDE with XC8 Compiler (for writing and compiling the code)
- Proteus 8 (for simulating the circuit and behavior)
- VS Code (for editing and version control)
- GitHub (for source code management)

## Team Members and Contributions

- **Imane Mohamed Lemine**  
  Responsible for writing the embedded C code, managing GitHub, and integrating logic for the sensor, output devices, and LCD.

- **Sabina Novruzova**  
  Focused on hardware design and Proteus simulation, including connecting all components and verifying the circuit layout.

- **Masah Alkhatib**  
  Led the testing process, wrote the project proposal, and designed the experimental scenarios for validation.

## How it Works

1. The system starts and displays a "System Ready" message.
2. It continuously reads temperature from the LM35 sensor.
3. If the temperature is equal to or higher than the threshold, the fan and buzzer are turned on.
4. If the temperature drops below the threshold, both are turned off.
5. The LCD shows the current temperature and threshold.
6. Pressing the button increases the threshold in steps of 5°C (from 20°C up to 35°C, then loops back to 20°C).

## Project Files

- `smart_fan_controller.c`: The main C source file for the microcontroller.
- `proposal.pdf`: Full project proposal including design, objectives, and testing plans.

## Notes

This project was developed as part of the Spring 2024–2025 Microprocessor course. It demonstrates the use of basic embedded system concepts such as GPIO control, ADC, LCD interfacing, interrupts, and real-time data processing.
