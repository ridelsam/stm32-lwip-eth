#!/usr/bin/env bash

set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
project_root="$(cd -- "${script_dir}/.." && pwd)"

# shellcheck source=../toolchain.conf
source "${project_root}/toolchain.conf"

# An exported TOOLCHAIN_ROOT takes precedence over the optional local file.
if [[ -z "${TOOLCHAIN_ROOT:-}" && -f "${project_root}/toolchain.local.conf" ]]; then
    # shellcheck source=../toolchain.local.conf.example
    source "${project_root}/toolchain.local.conf"
fi

toolchain_root="${TOOLCHAIN_ROOT:-${HOME}/tools/${TOOLCHAIN_DIRECTORY}}"
if [[ "${toolchain_root}" != /* ]]; then
    toolchain_root="${project_root}/${toolchain_root}"
fi

tool_prefix="${toolchain_root}/bin/${TOOLCHAIN_TRIPLE}-"
gcc="${tool_prefix}gcc"

fail() {
    printf 'toolchain error: %s\n' "$*" >&2
    exit 1
}

[[ "$(uname -m)" == "${TOOLCHAIN_HOST_ARCH}" ]] || \
    fail "expected host architecture ${TOOLCHAIN_HOST_ARCH}, found $(uname -m)"

command -v make >/dev/null 2>&1 || fail "GNU Make is not available on PATH"

for tool in gcc size objdump objcopy; do
    [[ -x "${tool_prefix}${tool}" ]] || \
        fail "missing executable ${tool_prefix}${tool}"
done

gcc_version="$("${gcc}" -dumpfullversion -dumpversion)"
[[ "${gcc_version}" == "${TOOLCHAIN_GCC_VERSION}" ]] || \
    fail "expected GCC ${TOOLCHAIN_GCC_VERSION}, found ${gcc_version} at ${gcc}"

gcc_output="$("${gcc}" --version)"
gcc_banner="${gcc_output%%$'\n'*}"
[[ "${gcc_banner}" == *"${TOOLCHAIN_RELEASE}"* ]] || \
    fail "expected ${TOOLCHAIN_RELEASE}, found: ${gcc_banner}"

for specs_file in nano.specs nosys.specs; do
    specs_path="$("${gcc}" -print-file-name="${specs_file}")"
    [[ "${specs_path}" != "${specs_file}" && -f "${specs_path}" ]] || \
        fail "${specs_file} is unavailable in ${toolchain_root}"
done

multilib="$("${gcc}" \
    -mcpu=cortex-m7 \
    -mfpu=fpv5-d16 \
    -mfloat-abi=hard \
    -mthumb \
    -print-multi-directory)"
[[ "${multilib}" == "${TOOLCHAIN_MULTILIB}" ]] || \
    fail "expected multilib ${TOOLCHAIN_MULTILIB}, found ${multilib}"

if [[ "${1:-}" == "--check" ]]; then
    printf 'Toolchain: %s\n' "${gcc_banner}"
    printf 'Root:      %s\n' "${toolchain_root}"
    printf 'Multilib:  %s\n' "${multilib}"
    exit 0
fi

exec make \
    -C "${project_root}" \
    CROSS_COMPILE="${tool_prefix}" \
    "$@"
