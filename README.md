# iotester

A simple test suite to verify the basic functionality of filesystems and block storage devices. Put shortly, it tests whether you get the same data back that you have written onto a device.

At the moment, only a very limited test for filesystems is implemented. This program will write 1 MiB sized files filled with random data to disk, using the SHA-256 checksum of the random data as the filename. After the target number of files have been written, it will read them back and check whether the SHA256 sum from their names still correspond to their contents. At the moment, the number of files is hard coded to 10000, while the directory is hardcoded as the subdirectory "iotest" of the current working directory.

## Purpose

The intended use of this suite is to allow you to find data corrupting bugs in filesystems or block device drivers, find faulty storage devices and identify fraudulent devices that advertise more storage to the OS than they can actually store.

## Requirements

- Crypto++ 8.9.0+ and corresponding development files
- CMake 3.10+
- PkgConfig
- C++ compiler supporting C++20

Development has been done against Crypto++ 8.9.0, but most other non-ancient versions will likely work.

## Platform

This software should work on any reasonably recent Linux platform, although in some cases you may have to install a C++ compiler supporting C++ 20 first.

It will probably also run on most BSDs and on MacOS, but this has never been tested. If you try this, please report your findings.

It will almost certainly not run on Microsoft Windows outside of a Cygwin or WSL environment.

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

At the moment, the program takes no options or arguments and will start working in the current working directory.

### Testing block devices

An alternative mode specifically for testing block devices is planned, but not yet implemented. For the time being, to test a block device, create a filesystem on it (it needs to support filenames 64 characters in length, so no FAT without VFAT extensions), mount that, then use iotester as you would for a filesystem.

### Return code

Returns 0 on success, 1 if a file could not be read or a file showed signs of data corruption.

## Limitations

Since the SHA-256 checksums are stored alongside the data, a malicious actor who knows about this software can create a filesystem or mass storage device that generates both the random data and the corresponding checksums on the fly during the read phase, thereby defeating the test.

A filesystem that completely loses files, including their directory entries, will not be found as faulty.
