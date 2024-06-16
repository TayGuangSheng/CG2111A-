# Alex: Tele-operated Search and Rescue Robot

## Introduction

Alex is a tele-operated robotic vehicle designed to assist in search and rescue operations during natural or manmade disasters. The aim is to maximize rescue efforts within the critical 72-hour window following a disaster. Alex is built to overcome the limitations of human rescuers, providing continuous operation without the need for rest. Equipped with advanced sensors and a robust communication system, Alex enhances the efficiency and safety of search and rescue missions.

## Features

- **Continuous Operation**: Powered by six 1.5V batteries, allowing for over 72 hours of operation.
- **Remote Control**: Operated remotely via a four-wheel drive system and RPLidar for obstacle and survivor detection.
- **Enhanced Communication**: Equipped with an antenna to extend communication range.
- **Survivor Detection**: Uses ultrasonic and color sensors to detect survivors and alert rescuers through a Bluetooth speaker.
- **Mapping and Navigation**: Utilizes Hector SLAM for mapping and RVIZ for visualization.

## System Architecture

Alex comprises several key components:
- **Motors**: Controlled by Arduino for movement.
- **Sensors**: Includes RPLidar for scanning, ultrasonic sensor for distance measurement, and color sensor for detecting survivors.
- **Communication**: Bluetooth speaker for audio alerts and WiFi adapter for enhanced network stability.
- **Control System**: Commands are sent from a Raspberry Pi (RPi) to Arduino, which then controls the motors and sensors.

### Command and Response Structure

| Device         | Command Type           | Response Type | Details                                                                 |
| -------------- | ---------------------- | ------------- | ----------------------------------------------------------------------- |
| Color Sensor   | COMMAND_COLOUR         | RESP_COLOUR   | Measures RGB values and distance to target.                             |
| Motor          | COMMAND_FORWARD, etc.  | RESP_OK       | Controls movement with PWM frequencies.                                 |
| Ultrasonic Sensor | COMMAND_COLOUR      | RESP_DIST     | Measures distance using echo duration.                                  |
| Bluetooth Speaker | COMMAND_COLOUR      | RESP_OK       | Plays audio files based on detected survivor's status.                  |

## Hardware Components

1. **HC-SR04 Ultrasonic Sensor**: Measures distance to prevent collisions.
2. **Color Sensor**: Detects the color of objects (Green, Red, White).
3. **Bluetooth Speaker**: Alerts rescuers and reassures survivors.
4. **Fan and Heatsink**: Cools the RPi to prevent overheating.
5. **Lidar Mount**: Ensures stable and unobstructed scanning by RPLidar.

## Firmware Design

### Arduino Algorithm
- Receives command packets from the RPi.
- Executes commands and responds with status packets.
- Polls for data packets continuously and processes them using handle functions.

### Communication Protocol
- Uses UART serial communication (8N1 format, 9600 bps).
- Packets include command type, data, and checksum for reliability.
- Movement commands are segmented to allow for smooth interruption and control.

## Software Design

### Raspberry Pi Algorithm
- Acts as the main control unit, processing user inputs and generating maps with Hector SLAM.
- Operates with a video game-like control interface for smooth teleoperation.
- Adjusts movement and sensor calibration on the fly for precise navigation.

### Additional Features
- **Streaming Input**: Uses getch() for continuous input without needing to press enter.
- **WiFi Adapter**: Enhances network stability and speed.
- **Customized RVIZ Pose Transform**: Improves visualization accuracy in RVIZ.

## Lessons Learnt

1. **Connectivity**: Enhanced with an antenna for better communication range.
2. **Battery Life**: Increased with additional batteries for extended operation.

## Conclusion

Alex is a robust and efficient tool designed to support search and rescue operations, overcoming the limitations of human rescuers by providing continuous operation and advanced sensing capabilities. This project demonstrates the integration of hardware and software to create a reliable and effective rescue robot.

## Annexes

- **Annex A**: Detailed review of state-of-the-art search and rescue robots.
- **Annex B**: Table of additional commands and responses.
- **Annex C**: Hardware components specifications.
- **Annex D**: Bare metal code implementations.
- **Annex E**: Command schema and purposes.
- **Annex F**: Colour detection algorithm code.
- **Annex G**: Cross-platform getch() implementation.

---

**Note**: For detailed setup instructions, source code, and further technical documentation, please refer to the respective annexes and code files in this repository.
