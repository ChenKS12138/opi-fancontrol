# opi-fancontrol

opi-fancontrol

## 这是什么

利用software pwm用于控制香橙派的散热风扇。已在香橙派3 lts上进行测试并通过。

## 如何构建

### 前置条件

1. 确保 [wiringOP](https://github.com/orangepi-xunlong/wiringOP) 已安装

### 步骤

```shell
git clone https://github.com/ChenKS12138/opi-fancontrol.git
cd opi-fancontrol
git submodule update --init --recursive
cmake -S . -B build
cmake --build build
./build/opi-fancontrol -c example.ini
```
