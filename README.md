# Sensory Processing Pipeline

***This project is actively being developed***

Sensory Processing Pipeline (SPP) is a multithreaded application designed to process various types of sensory data. It currently supports video processing via OpenCV, but the goal is to eventually expand SPP's capabilities to handle a wider range of sensory data, including   audio.


## Features

- **Video Processing with OpenCV:** Efficiently process videos with a range of utilities and functionalities provided by OpenCV.

- **Multithreaded Architecture:** Leverage the power of modern multi-core CPUs for faster, simultaneous processing.

- **Modular Design:** Easily extend the platform to support various video inputs (webcam, .mp4, etc), video processing pipelines and video outputs (Video Player, AI Models, etc).

## Getting Started

### Prerequisites

- [CMake](https://cmake.org/)
- [OpenCV](https://opencv.org/releases/) (Version 4.x recommended)
- [spdlog](https://github.com/gabime/spdlog) 

### Installation

1. Clone the repository:<br>
`git clone https://github.com/keatoncscheible/spp.git`

1. Build SPP. Go to to root of the SPP repo and enter the following commands:<br>
`mkdir build`<br>
`cd build`<br>
`cmake ..`<br>
`sudo make`<br>
`cd ..`<br>

1. Run the application. Go to to root of the SPP repo and enter the following command:<br>
`sudo ./build/spp_app`

