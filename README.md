# opi-fancontrol

opi-fancontrol

## What is it

Utilize software pwm to control fan on your Orange Pi. Having tested on OrangePi 3 LTS, it works well.

## How To Build

### Prerequisites

1. Ensure [wiringOP](https://github.com/orangepi-xunlong/wiringOP) Installed

### Steps

```shell
git clone https://github.com/ChenKS12138/opi-fancontrol.git
cd opi-fancontrol
git submodule update --init --recursive
cmake -S . -B build
cmake --build build
./build/opi-fancontrol -c example.ini
```
