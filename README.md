# 🌦️ MyInssetWeather - Autonomous Weather Station

> Intelligent weather station developed for INSSET - An innovative student project for real-time environmental monitoring.


## 📋 Table of Contents
- [About](#-about)
- [Features](#-features)
- [Collected Data](#-collected-data)
- [Project Structure](#-project-structure)
- [Development Steps](#-development-steps)
- [Team](#-team)
- [License](#-license)

## 🎯 About

MyInssetWeather is an autonomous weather station developed as part of a Club Elec project. This station collects and transmits real-time accurate weather data through a set of professional sensors and an Arduino microcontroller.

### Project Objectives
- **Practical** : Create a functional weather monitoring solution
- **Innovation** : Develop an autonomous and connected station
- **Community** : Provide local weather data for INSSET

## ⚡ Features

### Environmental Measurements
- 🌡️ **Temperature** - Precise ambient temperature measurement
- 💧 **Humidity** - Relative air humidity rate
- 📊 **Atmospheric pressure** - Barometric pressure in hPa
- 💨 **Wind speed** - Measurement in km/h with anemometer
- 🧭 **Wind direction** - 8 main directions (N, NE, E, SE, S, SW, W, NW)
- 🌧️ **Precipitation** - Rain intensity in mm/h


## 📊 Collected Data

### Measured Parameters
| Parameter | Unit | Range | Precision | 
|-----------|------|-------|-----------|
| Temperature | °C | -40 to +85 | ±1°C |
| Humidity | % | 0 to 100 | ±3% |
| Pressure | hPa | 300 to 1100 | ±1 hPa |
| Wind speed | km/h | 0 to 150+ | ±0.1 km/h |
| Wind direction | ° | 0 to 360 | ±22.5° |
| Precipitation | mm/h | 0 to 100+ | ±0.1 mm/h |

## 📁 Project Structure

```
├── 📁 Station/
│       ├── 📁 Main/
│       │   └── 📄 main.ino
│       └── 📁 PCB/
│            └── 📄 WeatherStation.eprj
├── 📜 LICENSE
└── 📖 README.md
```

## 🛠️ Development Steps

### ✅ Phase 1 - Development (Completed)
- [x] Electronic schematic design
- [x] Arduino code development
- [x] Individual sensor testing
- [x] Complete system integration

### 🔄 Phase 2 - Implementation (In Progress)
- [x] Electronic board realization
- [x] Program testing
- [ ] Power supply and station installation
- [ ] Extended field testing

### 📋 Phase 3 - Deployment (Planned)
- [ ] Final on-site installation
- [ ] Web interface development
- [ ] Website data testing

### 🚀 Phase 4 - Improvements (Future)
- [ ] WiFi/LoRa connectivity
- [ ] Historical database
- [ ] Mobile application

## 👥 Team



| Name | Class | Position |
|-----|------|------------|
| **Thomas Cousiney** | L3MN | President |
| **Yoni Taine** | L3MN | Vice-President |
| **Eliot Hauet** | L3MN | Secretary |
| **Zakariya EL Kandoussi** | L3SE | Member |

### Supervision
- **Institution** : INSSET (Club Elec)
- **Period** : 2025
- **Type** : Collaborative student project

## 📝 License

This project is under MIT license. See the [LICENSE](LICENSE) file for more details.

---

### 📞 Contact

For any questions or suggestions about this project:
- 📧 Email : [clubelec@insset.u-picardie.fr](mailto: clubelec@insset.u-picardie.fr)
- 🌐 GitHub : [ClubElecINSSET](https://github.com/ClubElecINSSET)
- 🏫 Club Elec - [Website](https://clubelec.insset.fr/)

---