# STM32F7 lwIP Ethernet

Embedded Ethernet project for the STM32F767ZI Nucleo board exploring the complete path from the STM32 Ethernet peripheral to network applications using lwIP.

## Project Goals

The goal of this project is to understand the embedded Ethernet stack end-to-end rather than relying entirely on generated configuration or prebuilt examples.

The project will progressively implement and explore:

- STM32F7 peripheral initialization
- Ethernet MAC and RMII configuration
- Ethernet DMA transmit and receive paths
- PHY configuration and link management
- The interface between the STM32 Ethernet driver and lwIP
- lwIP network configuration
- DHCP
- HTTP server applications
- SSI/CGI-based hardware interaction

## Hardware

- STM32 NUCLEO-F767ZI
- Integrated Ethernet interface

## Development Environment

Development is performed from Ubuntu under WSL2 using VS Code.

Toolchain and build-system details will be documented here as the environment is established.

## Project Structure

The project structure will evolve as the firmware is developed. Major components will include:

- application code
- STM32 HAL/CMSIS support
- Ethernet interface code
- lwIP middleware
- startup and linker configuration
- build configuration

## Building

The project uses Arm GNU Toolchain 14.2.Rel1. By default, the build wrapper
expects the Linux x86-64 toolchain at:

```text
~/tools/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi
```

Verify the selected toolchain without building:

```bash
./scripts/build.sh --check
```

Build the default Debug configuration, which includes GDB symbols:

```bash
./scripts/build.sh
```

Build or clean a specific configuration:

```bash
./scripts/build.sh CONFIG=Release
./scripts/build.sh clean
./scripts/build.sh clean CONFIG=Release
./scripts/build.sh clean-all
```

Artifacts are written to `build/Debug` or `build/Release`.

If the toolchain is installed elsewhere, copy
`toolchain.local.conf.example` to `toolchain.local.conf` and set its absolute
path. The local file is ignored by Git. An exported `TOOLCHAIN_ROOT` takes
precedence over both the local override and the default location.

The root Makefile remains independent of the wrapper and can also be invoked
directly by supplying a GNU Arm tool prefix through `CROSS_COMPILE`.

## Status

🚧 In development — environment and project scaffolding.
