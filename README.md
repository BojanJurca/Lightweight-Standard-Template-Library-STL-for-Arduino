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
