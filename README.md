# zds2023

A sample application that demonstrates how to use [Pigweed](https://pigweed.dev)
in a [Zephyr](https://zephyrproject.org/) project.

## Assumptions & prerequisites

* This application was developed on a Linux host. It should work on Windows and
  macOS, but this hasn't been personally verified.

## Getting started

### Initial, one-time setup

```bash
$ python3 -m venv .venv
$ source .venv/bin/activate
$ pip install west
$ west init -l manifest
$ west update
$ pip install -r zephyr/scripts/requirements.txt
$ . pigweed/bootstrap.sh 
```

### Activating the development environment

After the initial one-time setup, run these steps
to re-activate your development environment:

```bash
$ source .venv/bin/activate
$ . pigweed/activate.sh
```

## Building the app

### native_posix

```bash
(.venv) $ west build -p -b native_posix app
(.venv) $ ./build/zephyr/zephyr.exe
*** Booting Zephyr OS build e03d6cf56fc9 ***
[00:00:00.000,000] <inf> pigweed: APP Hello
```

### TDK robokit1

```bash
(.venv) $ west build -p -b tdk_robokit1 app && west flash
(.venv) $ pyserial-miniterm --raw /dev/ttyUSB0 112500
--- Miniterm on /dev/ttyUSB1  112500,8,N,1 ---
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
[00:00:00.000,000] <wrn> soc: CIDR mismatch: chip = 0xa1020e01 vs HAL = 0xa1020e00
*** Booting Zephyr OS build e03d6cf56fc9 ***
[00:00:00.000,000] <inf> pigweed: APP Hello
```

## Setting up the web server

### 1. Install Node

1. Install `nvm` (https://github.com/nvm-sh/nvm)
2. Install `nodejs`: `nvm install node`)

### 2. Run the server

```bash
$ cd webconsole
$ npm install --force
$ npm run dev
```
