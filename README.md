# smartAgriLora
This Repository contains the source code and documentation for a Smart Agriculture Monitoring System which utilizes resources efficiently with LoRa communication system.

## Project Overview
The Smart Agriculture Monitoring System is designed to efficiently monitor agricultural parameters using LoRa (Long Range) communication technology. The system consists of multiple nodes organized in a hierarchical structure to collect, process, and transmit agricultural data.

## Current Implementation Status

### ✅ Completed Components
- **data__alert.cpp** - Advanced 16-bit bitfield alert system with 25 unique colors and agricultural combinations
- **data_collector.cpp** - Optimized data collection module for sensor readings (DHT11 + soil moisture)
- **lora_receiver.cpp** - LoRa receiver implementation with memory-optimized decode functions
- **lora_sender.cpp** - LoRa sender implementation for data transmission with compressed payloads
- **config_manager.cpp** - Configuration management with India-compliant 865MHz frequency
- **local_node.cpp** - Local node implementation with sensor integration and LoRa communication
- **Header files** - Complete struct definitions and class interfaces for all components

### � In Progress
- **System integration testing** - Validation of complete communication chain
- **Power optimization** - Fine-tuning for battery-powered deployment

### 📋 Planned Implementation
- **sublocal_node.cpp** - Sublocal node for intermediate data processing and routing
- **central_node.cpp** - Central node for data aggregation and cloud integration
- **web_interface.cpp** - Web dashboard for real-time monitoring and control

## System Architecture
The system follows a hierarchical communication structure:
```
Central Node
    ↓
Sublocal Nodes
    ↓
Local Nodes
    ↓
Sensors/Actuators
```

## Directory Structure

```
smartAgriLora/
├── src/                    # Core project source code
│   ├── *.cpp              # C++ implementation files (core functionality)
│   └── *.h                # Header files (core interfaces)
├── analysis/               # Analysis and demonstration code
│   ├── comprehensive_analysis.cpp    # Complete system analysis
│   ├── deployment_comparison.cpp     # Deployment scenario comparisons
│   ├── power_analysis.cpp/h          # Power consumption analysis
│   └── power_demo.cpp               # Power optimization demonstrations
├── docs/                   # Documentation and reports
│   ├── *.pdf              # Technical reports and datasheets
│   └── *.tex              # LaTeX source files
├── latex-artifacts/        # LaTeX compilation artifacts
│   └── *.aux, *.log, etc. # Generated files from LaTeX compilation
├── images/                 # Project images and logos
│   └── *.png              # Image files
├── README.md               # This file
├── LICENSE                 # GNU GPL v3.0 license
└── .gitignore             # Git ignore rules
```

## Core Components

### Communication Layer
- **LoRa Sender**: Handles message transmission with compressed payload encoding
- **LoRa Receiver**: Manages incoming LoRa communications with memory-optimized decoding
- **Message Protocol**: Standardized communication format with 4 message types (DATA, CONFIG, THRESHOLDS, SENDFAIL)
- **Config Manager**: Manages LoRa parameters and sensor thresholds with validation

### Node Hierarchy
- **Local Nodes**: Direct sensor interface with DHT11 and soil moisture sensors
- **Sublocal Nodes**: Intermediate data aggregation and routing (planned)
- **Central Node**: Main control center for system coordination (planned)

### Data Management
- **Data Collector**: Sensor data acquisition with hardware abstraction
- **Alert System**: Advanced 16-bit bitfield system with agricultural context-aware colors
- **Memory Optimization**: Reference-based parameter passing to minimize memory usage

## Features
- **Long-range communication** using LoRa technology (865MHz for India compliance)
- **Hierarchical node structure** for scalable deployment
- **Memory-optimized code** with reference-based parameter passing
- **Compressed data transmission** for efficient bandwidth usage
- **Real-time agricultural monitoring** with DHT11 and soil moisture sensors
- **Advanced alert system** with 16-bit bitfield and 25 unique colors for agricultural conditions
- **Smart color logic** - appropriate colors for seasonal conditions vs critical system alerts
- **Adaptive LoRa parameters** based on communication range requirements
- **Round-robin addressing** for load balancing across multiple destinations
- **Error handling and recovery** with automatic parameter adjustment

## Getting Started
1. Set up Arduino IDE with LoRa library
2. Configure hardware connections
3. Upload appropriate node firmware
4. Configure network parameters

## Hardware Requirements
- Arduino-compatible microcontrollers
- LoRa modules (SX1276/SX1278)
- Agricultural sensors (temperature, humidity, soil moisture, etc.)
- Power management system

## Software Dependencies
- Arduino IDE
- LoRa library (for SX1276/SX1278 modules)
- DHT sensor library (for DHT11 temperature/humidity sensor)
- Standard Arduino libraries (Wire, SPI)

## Technical Specifications

### Communication Protocol
- **Frequency**: 865MHz (India ISM band compliant)
- **Modulation**: LoRa with configurable SF7-SF12
- **Bandwidth**: 125kHz standard, adjustable for range optimization
- **Message Types**: DATA, CONFIG, THRESHOLDS, SENDFAIL
- **Payload Compression**: 32-bit compressed sensor data transmission

### Sensor Support
- **Temperature**: DHT11 (-40°C to +80°C, ±2°C accuracy)
- **Humidity**: DHT11 (5% to 95% RH, ±5% accuracy)
- **Soil Moisture**: Analog sensor with 10-bit ADC (0-1023 raw values)

### Alert System
- **16-bit Bitfield**: Efficient alert representation with bitwise operations
- **25 Unique Colors**: Context-aware colors for agricultural and system conditions
- **Agricultural Combinations**: Predefined colors for seasonal conditions (summer heat, drought, etc.)
- **Priority System**: Critical system alerts override normal environmental conditions
- **Memory Efficient**: PROGMEM storage for static data, buffer-based functions

### Memory Optimization
- Reference-based parameter passing eliminates unnecessary object copying
- Compressed payload reduces transmission bandwidth
- PROGMEM usage for static data (alert descriptions and colors)

## Documentation
- Technical documentation and analysis reports are available in the `docs/` folder
- Power consumption analysis and system comparisons are in the `analysis/` folder
- LaTeX source files and generated PDFs provide comprehensive technical specifications
- SX1276/SX1278 datasheet is included for hardware reference

## Development Roadmap

### Phase 1: Core Components (✅ Complete)
1. ✅ LoRa communication layer (sender/receiver)
2. ✅ Data collection and sensor integration
3. ✅ Local node implementation
4. ✅ Memory optimization and buffer management
5. ✅ India frequency compliance (865MHz)

### Phase 2: Network Layer (🚧 In Progress)
1. 🚧 System integration testing
2. 🚧 Power optimization for battery operation
3. 📋 Sublocal node implementation
4. 📋 Multi-hop routing protocol

### Phase 3: Central System (📋 Planned)
1. 📋 Central node data aggregation
2. 📋 Web interface and dashboard
3. 📋 Database integration
4. 📋 Real-time alert notifications

### Phase 4: Advanced Features (📋 Future)
1. 📋 Over-the-air firmware updates
2. 📋 Mesh networking capabilities
3. 📋 Machine learning for predictive analytics
4. 📋 Mobile application interface

## Contributing
Contributions are welcome! Please ensure code follows the established architecture and communication protocols.

## License
This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

## Contact
For questions, suggestions, or contributions, please create an issue or submit a pull request.
