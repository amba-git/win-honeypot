# win-honeypot

A lightweight honeypot built in C++ for Windows systems that mimics real services like HTTP and SSH.

## Features
Fake HTTP login portal (serves a form via GET, logs credentials via POST)
- Simulates SSH banner to fool scanners (e.g., OpenSSH_7.9p1)
- Logs:
  - IP address and port of incoming connections
  - `User-Agent` (HTTP only)
  - Submitted credentials (HTTP only)
- Modular service-based design (HTTP/SSH separate)
- Threaded connection handling
- JSON based configuration
  - Enable or Disable HTTP/SSH service in config.json file.
  - No need to re-compile program.

## Prerequisites 
- [CMake](https://cmake.org/download/)
- [vcpkg](https://github.com/microsoft/vcpkg) for nlohmann/json
- C++11-compatible compiler (e.g., MinGW-w64 or MSVC)
- JSON library (https://github.com/nlohmann/json)
- Windows SDK (https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/)

  ### Install Dependencies
  Install `nlohmann-json` via vcpkg:

```bash
  vcpkg install nlohmann-json
```

## Build
### (Option 1) Using CMAKE
```bash
  git clone https://github.com/amba-git/win-honeypot.git honeypot
  cd honeypot
  mkdir build
  cd build
  
  cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
  cmake --build .
```
### (Option 2) Manually Linking

```bash
  g++ -std=c++11 -pthread main.cpp _http\src\http_honeypot.cpp _http\src\http_log.cpp _ssh\src\ssh_honeypot.cpp _ssh\src\ssh_log.cpp -o honeypot.exe -I C:\vcpkg\installed\x64-windows\include -L C:\vcpkg\installed\x64-windows\lib -lws2_32
```
Make changes based on your choice in config.json and run the program
```bash
  .\honeypot.exe
```
