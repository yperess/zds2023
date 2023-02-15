## Getting started Zephyr

On first setup:
```bash
$ python3 -m venv .venv
$ source .venv/bin/activate
$ pip install west
$ west init -l manifest
$ west update
$ pip install -r zephyr/scripts/requirements.txt
$ . pigweed/bootstrap.sh 
```

On following work days:
```bash
$ source .venv/bin/activate
$ . pigweed/activate.sh
```

## Building the app

native_posix:
```bash
(.venv) $ west build -p -b native_posix app
(.venv) $ ./build/zephyr/zephyr.exe
*** Booting Zephyr OS build e03d6cf56fc9 ***
[00:00:00.000,000] <inf> pigweed: APP Hello
```

TDK robokit1:
```bash
(.venv) $ west build -p -b tdk_robokit1 app && west flash
(.venv) $ pyserial-miniterm --raw /dev/ttyUSB0 112500
--- Miniterm on /dev/ttyUSB1  112500,8,N,1 ---
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
[00:00:00.000,000] <wrn> soc: CIDR mismatch: chip = 0xa1020e01 vs HAL = 0xa1020e00
*** Booting Zephyr OS build e03d6cf56fc9 ***
[00:00:00.000,000] <inf> pigweed: APP Hello
```

## Web server

Install node:
1. Install `nvm` (https://github.com/nvm-sh/nvm)
2. Install `nodejs`: `nvm install node`)

Run the server:
```bash
$ cd pigweed/pw_web/webconsole
$ npm install
$ npm run dev
```
