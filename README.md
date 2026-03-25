# libjuice - UDP Interactive Connectivity Establishment

[![License: MPL 2.0](https://img.shields.io/badge/License-MPL_2.0-blue.svg)](https://www.mozilla.org/en-US/MPL/2.0/)
[![Build and test](https://github.com/paullouisageneau/libjuice/actions/workflows/build.yml/badge.svg)](https://github.com/paullouisageneau/libjuice/actions/workflows/build.yml)
[![Packaging status](https://repology.org/badge/tiny-repos/libjuice.svg)](https://repology.org/project/libjuice/versions)
[![Latest packaged version](https://repology.org/badge/latest-versions/libjuice.svg)](https://repology.org/project/libjuice/versions)
[![Gitter](https://badges.gitter.im/libjuice/community.svg)](https://gitter.im/libjuice/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Discord](https://img.shields.io/discord/903257095539925006?logo=discord)](https://discord.gg/jXAP8jp3Nn)

libjuice :lemon::sweat_drops: (_JUICE is a UDP Interactive Connectivity Establishment library_) allows to open bidirectionnal User Datagram Protocol (UDP) streams with Network Address Translator (NAT) traversal.

The library is a simplified implementation of the Interactive Connectivity Establishment (ICE) protocol, client-side and server-side, written in C without dependencies for multiple platforms, including GNU/Linux, Android, FreeBSD, Apple macOS, iOS, and Microsoft Windows. The client supports only a single component over UDP per session in a standard single-gateway network topology, as this should be sufficient for the majority of use cases nowadays.

libjuice is licensed under MPL 2.0, see [LICENSE](https://github.com/paullouisageneau/libjuice/blob/master/LICENSE).

libjuice is available on [AUR](https://aur.archlinux.org/packages/libjuice/) and [vcpkg](https://vcpkg.io/en/getting-started). Bindings are available for [Rust](https://github.com/VollmondT/juice-rs).

For a STUN/TURN server application based on libjuice, see [Violet](https://github.com/paullouisageneau/violet).

## Compatibility

The library implements a simplified full ICE agent ([RFC5245](https://www.rfc-editor.org/rfc/rfc5245.html) then [RFC8445](https://www.rfc-editor.org/rfc/rfc8445.html)) featuring:
- STUN protocol ([RFC5389](https://www.rfc-editor.org/rfc/rfc5389.html) then [RFC8489](https://www.rfc-editor.org/rfc/rfc8489.html))
- TURN relaying ([RFC5766](https://www.rfc-editor.org/rfc/rfc5766.html) then [RFC8656](https://www.rfc-editor.org/rfc/rfc8656.html))
- TCP candidates ([RFC6544](https://www.rfc-editor.org/rfc/rfc6544.html))
- ICE Consent freshness ([RFC7675](https://www.rfc-editor.org/rfc/rfc7675.html))
- ICE Patiently Awaiting Connectivity ([RFC 8863](https://www.rfc-editor.org/rfc/rfc8863.html))
- SDP-based interface ([RFC8839](https://www.rfc-editor.org/rfc/rfc8839.html))
- IPv4 and IPv6 dual-stack support
- Optional multiplexing on a single UDP port

The limitations compared to a fully-featured ICE agent are:
- Only UDP is supported as transport protocol and other protocols are ignored.
- Only one component is supported, which is sufficient for WebRTC Data Channels and multiplexed RTP+RTCP.
- Only [RFC8828](https://www.rfc-editor.org/rfc/rfc8828) mode 2 is supported (default route + all local addresses).

It also implements a lightweight STUN/TURN server ([RFC8489](https://www.rfc-editor.org/rfc/rfc8489.html) and [RFC8656](https://www.rfc-editor.org/rfc/rfc8656.html)). The server can be disabled at compile-time with the `NO_SERVER` flag.

## Dependencies

None!

Optionally, [Nettle](https://www.lysator.liu.se/~nisse/nettle/) can provide SHA1 and SHA256 algorithms instead of the internal implementation. On ESP-IDF, the built-in mbedtls is used automatically.

## Building

### ESP-IDF (ESP32 series)

This library is ported as an ESP-IDF component supporting ESP32, ESP32-S2, ESP32-S3, ESP32-C2, ESP32-C3, ESP32-C5, ESP32-C6, ESP32-H2, etc. Requires ESP-IDF v5.0+.

#### Quick start

```bash
cd examples/connectivity
idf.py set-target esp32c3   # or esp32c5, esp32s3, etc.
idf.py menuconfig           # configure WiFi credentials under "Example Connection Configuration"
idf.py build flash monitor
```

#### Component integration

Add libjuice to your project's `components/` directory, or reference it via `idf_component.yml`. The component auto-registers with `idf_component_register()` and provides the `juice/juice.h` public header.

```cmake
# In your main component CMakeLists.txt
idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES libjuice)
```

#### ESP-IDF platform adaptations

- **Crypto**: Uses ESP-IDF's mbedtls instead of the internal picohash implementation
- **RNG**: Uses hardware `esp_fill_random()` for cryptographically secure random numbers
- **Threading**: Uses FreeRTOS tasks (not pthreads) with configurable stack sizes
- **Networking**: Uses `esp_netif` API for interface enumeration; provides `getnameinfo()` compatibility shim for lwIP
- **Symbol namespacing**: Internal UDP/HMAC/hash symbols are prefixed to avoid conflicts with lwIP and wpa_supplicant

#### Memory optimization via Kconfig

Run `idf.py menuconfig` and navigate to **libjuice** to tune memory usage:

| Parameter | Default | Range | Description |
|-----------|---------|-------|-------------|
| `JUICE_ICE_MAX_CANDIDATES` | 8 | 4-20 | Max ICE candidates per description (~220B each) |
| `JUICE_TASK_STACK_SIZE` | 6144 | 4096-16384 | Internal FreeRTOS task stack size |
| `JUICE_MAX_STUN_SERVERS` | 1 | 1-4 | Max STUN server entries |
| `JUICE_MAX_RELAY_SERVERS` | 1 | 1-4 | Max TURN relay entries |
| `JUICE_STUN_MAX_CREDENTIAL_LEN` | 128 | 64-764 | STUN credential field max length |
| `JUICE_BUFFER_SIZE` | 1500 | 1024-4096 | Network send/receive buffer size |
| `JUICE_LOG_BUFFER_SIZE` | 256 | 128-4096 | Log formatting buffer size |
| `JUICE_ENABLE_SERVER` | off | on/off | ICE/TURN server support |

With default settings, each ICE agent uses approximately 18-22KB of heap memory.

### Clone repository (desktop platforms)

```bash
$ git clone https://github.com/paullouisageneau/libjuice.git
$ cd libjuice
```

### Build with CMake

The CMake library targets `libjuice` and `libjuice-static` respectively correspond to the shared and static libraries. The default target will build the library and tests. It exports the targets with namespace `LibJuice::LibJuice` and `LibJuice::LibJuiceStatic` to link the library from another CMake project.

#### POSIX-compliant operating systems (including Linux and Apple macOS)

```bash
$ cmake -B build
$ cd build
$ make -j2
```

The option `USE_NETTLE` allows to use the Nettle library instead of the internal implementation for HMAC-SHA1:
```bash
$ cmake -B build -DUSE_NETTLE=1
$ cd build
$ make -j2
```

#### Microsoft Windows with MinGW cross-compilation

```bash
$ cmake -B build -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-x86_64-w64-mingw32.cmake # replace with your toolchain file
$ cd build
$ make -j2
```

#### Microsoft Windows with Microsoft Visual C++

```bash
$ cmake -B build -G "NMake Makefiles"
$ cd build
$ nmake
```

### Build directly with Make (Linux only)

```bash
$ make
```

The option `USE_NETTLE` allows to use the Nettle library instead of the internal implementation for HMAC-SHA1:
```bash
$ make USE_NETTLE=1
```

## Example

See [examples/connectivity/](examples/connectivity/) for an ESP-IDF connectivity example.

See [test/connectivity.c](https://github.com/paullouisageneau/libjuice/blob/master/test/connectivity.c) for a complete local connection example (desktop).

See [test/server.c](https://github.com/paullouisageneau/libjuice/blob/master/test/server.c) for a server example.

