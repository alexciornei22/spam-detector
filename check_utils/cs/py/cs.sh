#!/bin/bash

PYLINT="pylint"
DOS2UNIX="$(dirname "$0")/.dos2unix"

usage()
{
    echo "Usage: $0 file"
    echo "       $0 directory"
}

check_coding_style()
{
    "${DOS2UNIX}" "${1}" 1> /dev/null 2>&1

    "${PYLINT}" "${1}" | grep "^.*.py:.*"
    ret=$?
    return $ret
}

export PYLINT="${PYLINT}"
export DOS2UNIX="${DOS2UNIX}"
export -f check_coding_style

if [ $# -eq 0 ] || [ "$1" = "-h" ]; then
    usage 0
elif [ $# -eq 1 ] && [ -f "$1" ]; then
    check_coding_style "$1"
else
    TARGET_DIR="."
    if [ $# -eq 1 ] && [ -d "$1" ]; then
        TARGET_DIR="$1"
    fi

    find "${TARGET_DIR}" -type f -regex '.*\.py' -not -path "./check_utils/*" -exec bash -c 'check_coding_style "{}"' \;
fi
