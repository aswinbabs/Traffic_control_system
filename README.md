# Density-Based Traffic Control System

An intelligent traffic control system for a four-way intersection, designed to optimize traffic flow and reduce congestion by detecting vehicle density and adjusting signal timings accordingly. This project was developed using an Arduino Mega microcontroller and infrared (IR) sensors.

## Overview

This project manages traffic at intersections by utilizing real-time data processing from IR sensors to determine traffic density. Based on detected traffic levels, the system dynamically adjusts signal durations, achieving smoother traffic flow and minimizing wait times during high- and low-traffic periods. A power-saving mode also conserves energy by reducing system activity when traffic is low.

## Features

- **Real-Time Traffic Density Detection**: Uses 8 IR sensors (2 per lane) to gauge traffic flow.
  - **No Detection**: No vehicles in the lane.
  - **One Sensor Detection**: Low traffic density.
  - **Two Sensor Detection**: High traffic density.
- **Dynamic Signal Control**: Adjusts signal timing based on detected traffic, optimizing traffic movement across all lanes.
- **Power-Saving Mode**: Reduces energy consumption by 15% during low-traffic periods using PWM and timers.
- **Interrupt-Driven Logic**: Ensures efficient and immediate responses to changes in traffic conditions.

## System Components

- **Arduino Mega**: Controls the IR sensors and manages signal adjustments based on real-time density data.
- **IR Sensors**: Detect vehicle presence in each lane.
- **PWM & Timers**: Implemented for energy-saving mode during low-traffic times.

## Achievements

- **Improved Traffic Flow**: Reduced congestion by 20% through adaptive signal timing.
- **Energy Efficiency**: Lowered power consumption by 15% during off-peak hours.

## Usage

1. Deploy the system at a four-way intersection with two IR sensors per lane.
2. The system will automatically adjust signal timing based on the detected density:
   - High Density: Longer green signal for higher traffic flow.
   - Low Density: Standard green signal timing.
   - No Traffic: Shorter or skipped green signal.

## Setup

1. Connect the 8 IR sensors to the Arduino Mega as specified in the wiring diagram.
2. Upload the code to the Arduino Mega.
3. Power on the system, and it will begin managing traffic based on real-time density data.

## Future Enhancements

- Integration with IoT for remote monitoring and data collection.
- Adaptive algorithms for holiday and peak traffic scenarios.
- Real-time data logging for traffic pattern analysis.
