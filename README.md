# Concurrent Traffic Simulation

<img src="data/traffic_simulation.gif"/>

A multithreaded traffic simulation that demonstrates concurrent programming principles using C++. Vehicles navigate through intersections controlled by traffic lights, with thread-safe communication implemented using mutexes, locks, and message queues.

## Features

- Real-time traffic simulation with multiple vehicles
- Thread-safe traffic light system with random cycle durations (4-6 seconds)
- Concurrent vehicle movement and intersection management
- Visual representation using OpenCV
- Message queue-based communication between components

## Dependencies

* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./traffic_simulation`

## Architecture

The simulation consists of several key components:

- **TrafficLight**: Manages traffic light phases (red/green) with random cycle durations
- **MessageQueue**: Thread-safe communication between traffic lights and vehicles
- **Intersection**: Coordinates vehicle entry and traffic light control
- **Vehicle**: Simulates vehicle movement and intersection navigation
- **Graphics**: Renders the simulation using OpenCV
