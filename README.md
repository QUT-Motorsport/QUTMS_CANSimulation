# CAN Simulation
The code is dedicated only for QUT Motorsports usage. Code functionality is based on implementation of several people from different part of the world, refer to ACNOWLEDGEMNT.

---
## Table of Contents

> If your `README` has a lot of info, section headers might be nice.

- [Installation](#installation)
- [Features](#features)
- [Contributing](#contributing)
- [Team](#team)
- [FAQ](#faq)
- [Support](#support)
- [License](#license)

---

## Instructions
To get started with implementation, the UNIX based environment has to be created. Usage of Cygwin within Windows environment is acceptable. All OS has to have avr-gcc and relative libraries installed. Refer to SOWTWARE_REQUIREd.

Before implementation, hardware based settings has to be set for used language. Refer to Libs folder to identefy or correct pinouts for SPI signals.
NOTE that Raspberry Pi has only 2 CS pins for SPI needs. Refer to README_PI.md for configuration instructions.
---
## Build
### Requirements
Refer to INSTRUCTIONS.md to get all necessary instalations.
C version code:
```
make main
```
To build code and prepare for upload use run:
```
make build
```
Plug in Teensy and press RESET. When devices enters programming mode CL version of loader will upload
---
## Contributing
###### Contribution guide is dedicated for QUTMS members to implement.
Each assigned role has to be implemented within separate bracnh and kept as a separate release.
```
git clone git@github.com:QUT-Motorsport/QUTMS_CANSIM.git
```

```
git checkout -b SHUTDOWN
```

Performing changing within regular work process.
To update code with new changes from master perform merge within release branch.

```
git add . && git commit -b "MESSAGE"
git pull origin master
```
---
## Acknowledgments
https://github.com/dergraaf/avr-can-lib
http://microsin.net/adminstuff/hardware/mcp2515-stand-alone-can-controller-with-spi-interface.html

https://radiokot.ru/forum/viewtopic.php?f=54&t=154271