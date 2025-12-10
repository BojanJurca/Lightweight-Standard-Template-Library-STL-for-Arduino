# Lightweight C++ Standard Template Library (STL) for Arduino
A modular, exception-free implementation of selected C++ STL components - designed for microcontrollers with limited resources.

## Features
This library brings core STL functionality to Arduino platforms, including:
   - iostream-style output streams (cin, cout library for Arduino is included)
   - locale support for formatting
   - containers: vector, list, queue, and map
   - optional PSRAM support for extended memory
   - robust error reporting via errorFlags, without exceptions

Unlike standard STL implementations, this library avoids dynamic exceptions that could crash your microcontroller. Instead, all supported classes report runtime issues through the errorFlags() method, allowing graceful error handling.

You can configure the library to use PSRAM (if available) for container storage, reducing pressure on the heap and enabling larger datasets.

## Compatibility
Works across AVR, ESP32, and other Arduino-compatible platforms.


## Repository status

[![GitHub stars](https://img.shields.io/github/stars/BojanJurca/vb6WebServer?style=flat-square)](https://github.com/BojanJurca/vb6WebServer/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/BojanJurca/vb6WebServer?style=flat-square)](https://github.com/BojanJurca/vb6WebServer/network/members)
[![GitHub issues](https://img.shields.io/github/issues/BojanJurca/vb6WebServer?style=flat-square)](https://github.com/BojanJurca/vb6WebServer/issues)
[![GitHub release downloads](https://img.shields.io/github/downloads/BojanJurca/vb6WebServer/latest/total?style=flat-square)](https://github.com/BojanJurca/vb6WebServer/releases/latest)
