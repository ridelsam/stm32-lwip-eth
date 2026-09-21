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

| Command | Purpose |
|---|---|
| `./scripts/build.sh --check` | Verify the pinned toolchain and Cortex-M7 multilib without building. |
| `./scripts/build.sh` | Incrementally build Debug (`-Og -g3`) in `build/Debug`; use its ELF with GDB. |
| `./scripts/build.sh CONFIG=Release` | Incrementally build size-optimized Release (`-Os -g0`) in `build/Release`. |
| `./scripts/build.sh clean` | Remove only `build/Debug`. |
| `./scripts/build.sh clean CONFIG=Release` | Remove only `build/Release`. |
| `./scripts/build.sh clean-all` | Remove both build configurations. |

| Artifact | Purpose |
|---|---|
| `.elf` | Linked firmware with addresses and Debug symbols; used for flashing and GDB. |
| `.bin` | Raw Flash image without symbols. |
| `.map` | Linker memory and symbol map. |
| `.list` | Disassembly and section listing. |

These commands do not yet flash the board or launch a debugger.

If the toolchain is installed elsewhere, copy
`toolchain.local.conf.example` to `toolchain.local.conf` and set its absolute
path. The local file is ignored by Git. An exported `TOOLCHAIN_ROOT` takes
precedence over both the local override and the default location.

The root Makefile remains independent of the wrapper and can also be invoked
directly by supplying a GNU Arm tool prefix through `CROSS_COMPILE`.

## Status

🚧 In development — environment and project scaffolding.
