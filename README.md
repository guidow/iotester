# iotester

A simple test suite to verify the basic functionality of filesystems and block storage devices.

At the moment, only a very limited test for filesystems is implemented. This program will write 1 MiB sized files filled with random data to disk, using the SHA-256 checksum of the random data as the filename. After the target number of files have been written, it will read them back and check whether the SHA256 sum from their names still correspond to their contents. At the moment, the number of files is hard coded to 10000, while the directory is hardcoded as the subdirectory "iotest" of the current working directory.

## Requirements

- Crypto++ 8.9.0+ and corresponding development files
- CMake 3.10+
- PkgConfig
- C++ compiler supporting C++20

Development has been done against Crypto++ 8.9.0, but most other non-ancient versions will likely work.

## Building

This is a standard cmake project with little to no unusual stuff, so just do the usual:

```
mkdir build
cd build
cmake ..
make
```

The iotester binary will be in the "src" subdirectory.

## Usage

```iotester```

At the moment, the program takes no options or arguments.

## Return code

Returns 0 on success, 1 if a file could not be read or a file showed signs of data corruption.
