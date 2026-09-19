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

Build instructions will be added once the command-line build environment is established and verified from a clean checkout.

## Status

🚧 In development — environment and project scaffolding.