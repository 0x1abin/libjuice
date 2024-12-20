# Connectivity Example

This example demonstrates connectivity between two agents using the libjuice library.

## Supported Targets

| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- |

## Prerequisites

- ESP-IDF (tested with release/v5.3)
- libjuice (fork from https://github.com/paullouisageneau/libjuice)

## Building the Example

1. Set up the ESP-IDF environment:
    ```sh
    . $HOME/esp/esp-idf/export.sh
    ```

2. Navigate to the example directory:
    ```sh
    cd libjuice/examples/connectivity
    ```

3. Configure Set the Wi-Fi SSID and password under `Example Connection Configuration`:
    ```sh
    idf.py menuconfig
    ```
    

1. Build the project:
    ```sh
    idf.py build
    ```

## Flashing the Example

1. Flash the project to your ESP32 device:
    ```sh
    idf.py flash
    ```

2. Monitor the output:
    ```sh
    idf.py monitor
    ```

## Description

This example creates two agents using the libjuice library and establishes connectivity between them using STUN and TURN servers. The agents exchange messages to verify the connection.

## Configuration

The example uses the following configuration options:

- STUN server: `stun.cloudflare.com`
- TURN server: (commented out in the code, can be enabled if needed)
- Local port range: 60000-61000

## License

This project is licensed under the Mozilla Public License, v. 2.0. See the [LICENSE](https://mozilla.org/MPL/2.0/) file for details.

