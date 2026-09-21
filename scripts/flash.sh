#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
project_root="$(cd -- "${script_dir}/.." && pwd)"

config="${CONFIG:-Debug}"
target="${TARGET:-stm32-lwip-eth}"
elf="${project_root}/build/${config}/${target}.elf"

command -v openocd >/dev/null 2>&1 || {
    printf 'flash error: OpenOCD is not available on PATH\n' >&2
    exit 1
}

[[ -f "${elf}" ]] || {
    printf 'flash error: %s does not exist; build it first\n' "${elf}" >&2
    exit 1
}

exec openocd \
    -f "${project_root}/openocd.cfg" \
    -c "program {${elf}} verify reset exit"
