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

## Flashing and debugging

The NUCLEO-F767ZI uses its on-board ST-Link over SWD. OpenOCD runs in WSL and
uses `openocd.cfg` for the probe and target configuration. Install the host
utilities once:

```bash
sudo apt-get update
sudo apt-get install openocd usbutils
```

WSL needs the ST-Link USB device attached through usbipd. With the board plugged
in, run the following from Windows PowerShell, substituting its bus ID:

```powershell
usbipd list
usbipd bind --busid <BUSID>        # Administrator PowerShell; normally once
usbipd attach --wsl --busid <BUSID>
```

Confirm the device from WSL, build, and flash:

```bash
lsusb
./scripts/build.sh
./scripts/flash.sh
```

`flash.sh` programs, verifies, resets, and exits. It uses the Debug ELF by
default; set `CONFIG=Release` to flash the Release ELF. Flashing remains
separate from the Makefile.

For a manual GDB session, use two WSL terminals. In both terminals, first
change to the project root (the directory containing `Makefile` and this
README).

**Terminal 1 — start OpenOCD from the project root and leave it running:**

```bash
openocd -f openocd.cfg
```

**Terminal 2 — start GDB from the project root:**

```bash
"$HOME/tools/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gdb"
```

At the `(gdb)` prompt, load the project-relative ELF, connect to OpenOCD, flash
the ELF, and begin debugging:

```gdb
file build/Debug/stm32-lwip-eth.elf  # Load the executable and debug symbols.
target extended-remote :3333         # Connect GDB to OpenOCD.
monitor reset halt                   # Reset and halt the MCU.
load                                 # Flash the ELF currently loaded by GDB.
monitor reset halt                   # Reset and halt the newly flashed program.
break main                           # Set a breakpoint at main().
continue                             # Run until the breakpoint.
```

From there, use `next`, `step`, `finish`, breakpoints, and inspection commands
as needed. The roles of the main commands are:

- GDB's `file` command reads the ELF and its debug symbols on the computer; it
  does not flash the MCU.
- `target extended-remote :3333` connects GDB to the OpenOCD server.
- `load` programs the loaded ELF into the MCU's flash memory.
- `monitor reset halt` resets and stops the MCU so debugging can start from a
  known state.

If GDB says `No symbol table is loaded`, run the `file` command above. Its
relative path assumes GDB was launched from the project root. Use `pwd` at the
GDB prompt to display GDB's current working directory.

```gdb
info address main
```

If the target has already passed the code you want to inspect, reset it before
continuing:

```gdb
monitor reset halt
break main
continue
```

The most useful day-to-day GDB commands are:

| Command | Purpose |
|---|---|
| `break main` or `b main` | Add a breakpoint at a function. |
| `break Core/Src/main.c:17` | Add a breakpoint at a source line. |
| `info breakpoints` or `i b` | List breakpoints and their numbers. |
| `delete 2` | Delete breakpoint number 2; `delete` removes all breakpoints. |
| `clear main` | Remove the breakpoint at that location. |
| `disable 2` / `enable 2` | Temporarily disable or re-enable breakpoint 2. |
| `continue` or `c` | Resume until a breakpoint or fault. Press `Ctrl+C` to halt again. |
| `next` or `n` | Execute one source line, stepping over function calls. |
| `step` or `s` | Execute one source line, stepping into function calls. |
| `finish` | Continue until the current function returns. |
| `nexti` / `stepi` | Step over / into one machine instruction. |
| `print expression` or `p expression` | Evaluate a variable or expression. |
| `info locals` | Show local variables in the current stack frame. |
| `info registers pc sp lr` | Show the key CPU registers requested above. |
| `x/i $pc` | Disassemble the instruction at the program counter. |
| `backtrace` or `bt` | Show the current call stack. |
| `list` | Show source code around the current line. |
| `monitor reset halt` | Reset the MCU and halt it; GDB breakpoints remain defined. |
| `quit` | Exit GDB. Then press `Ctrl+C` in the OpenOCD terminal. |

Do not use GDB's normal `run` command for this bare-metal target. Reset with
`monitor reset halt`, then use `continue`. GDB has many more commands; use
`help`, `help breakpoints`, or `apropos <word>` to discover them.

In VS Code, select **STM32F767ZI: OpenOCD Debug** and press `F5`. The launch
configuration runs the Debug build, starts OpenOCD, programs the ELF, resets the
target, and stops at `main()`.

If the toolchain is installed elsewhere, copy
`toolchain.local.conf.example` to `toolchain.local.conf` and set its absolute
path. The local file is ignored by Git. An exported `TOOLCHAIN_ROOT` takes
precedence over both the local override and the default location.

The root Makefile remains independent of the wrapper and can also be invoked
directly by supplying a GNU Arm tool prefix through `CROSS_COMPILE`.

## Status

🚧 In development — environment and project scaffolding.
