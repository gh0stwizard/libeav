#!/bin/sh

#
# Usage:
#     checkall.sh | IDNKIT_DIR=/path/to/idnkit checkall.sh
#
# Description:
#     Run make check for all libraries and all compile flags.
#

export IDNKIT_DIR="$HOME/local"
export LD_LIBRARY_PATH="${IDNKIT_DIR}/lib"

curdir=`dirname $0`
# assuming that this script is placed in "misc/" directory
srcdir=`realpath "${curdir}/../"`
cd "${srcdir}" || exit 1

for lib in idn2 idn idnkit;
do
    # because "make clean check clean" works not as we want...
    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=OFF \
        FORCE_IDN=${lib} IDNKIT_DIR="${IDNKIT_DIR}" clean || exit 1
    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=OFF \
        FORCE_IDN=${lib} IDNKIT_DIR="${IDNKIT_DIR}" check clean || exit 1

    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=OFF \
        FORCE_IDN=${lib} IDNKIT_DIR="${IDNKIT_DIR}" clean || exit 1
    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=OFF \
        FORCE_IDN=${lib} IDNKIT_DIR="${IDNKIT_DIR}" check clean || exit 1

    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=ON \
        FORCE_IDN=${lib} IDNKIT_DIR="${IDNKIT_DIR}" clean || exit 1
    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=ON \
        FORCE_IDN=${lib} IDNKIT_DIR="${IDNKIT_DIR}" check clean || exit 1

    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=ON \
        FORCE_IDN=${lib} IDNKIT_DIR="${IDNKIT_DIR}" clean || exit 1
    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=ON \
        FORCE_IDN=${lib} IDNKIT_DIR="${IDNKIT_DIR}" check clean || exit 1
done
