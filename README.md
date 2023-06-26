# zds2023

A sample application that demonstrates how to use [Pigweed](https://pigweed.dev)
in a [Zephyr](https://zephyrproject.org/) project.

## Assumptions & prerequisites

* This application was developed on a Linux host. It should work on Windows and
  macOS, but this hasn't been personally verified.

## Getting started

### Initial, one-time setup

```bash
$ pip install west
$ west init -l manifest
$ west update
$ . pigweed/bootstrap.sh
$ pip install -r requirements.txt
```

### Activating the development environment

After the initial one-time setup, run these steps
to re-activate your development environment:

```bash
$ . pigweed/activate.sh
```

## Samples

- [Firebase](samples/firebase/README.md)
- [Microservices](samples/microservices/README.md)

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
