#!/bin/sh

#
# Usage:
#     checkall.sh | IDNKIT_DIR=/path/to/idnkit checkall.sh
#
# Description:
#     Run make check for all libraries and all compile flags.
#

[ -s "${IDNKIT_DIR}" ] || export IDNKIT_DIR="$HOME/local"
[ -s "$LD_LIBRARY_PATH" ] \
    && export LD_LIBRARY_PATH="${IDNKIT_DIR}/lib:${LD_LIBRARY_PATH}" \
    || export LD_LIBRARY_PATH="${IDNKIT_DIR}/lib"

curdir=`dirname $0`
# assuming that this script is placed in "misc/" directory
srcdir=`realpath "${curdir}/../"`
cd "${srcdir}" || exit 1

for lib in idn2 idn idnkit;
do
    # because "make clean check clean" works not as we want...
    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=OFF LABELS_ALLOW_UNDERSCORE=OFF \
        FORCE_IDN=${lib} clean || exit 1
    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=OFF LABELS_ALLOW_UNDERSCORE=OFF \
        FORCE_IDN=${lib} check clean || exit 1

    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=OFF LABELS_ALLOW_UNDERSCORE=ON \
        FORCE_IDN=${lib} clean || exit 1
    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=OFF LABELS_ALLOW_UNDERSCORE=ON \
        FORCE_IDN=${lib} check clean || exit 1

    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=OFF LABELS_ALLOW_UNDERSCORE=OFF \
        FORCE_IDN=${lib} clean || exit 1
    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=OFF LABELS_ALLOW_UNDERSCORE=OFF \
        FORCE_IDN=${lib} check clean || exit 1

    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=OFF LABELS_ALLOW_UNDERSCORE=ON \
        FORCE_IDN=${lib} clean || exit 1
    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=OFF LABELS_ALLOW_UNDERSCORE=ON \
        FORCE_IDN=${lib} check clean || exit 1

    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=ON LABELS_ALLOW_UNDERSCORE=OFF \
        FORCE_IDN=${lib} clean || exit 1
    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=ON LABELS_ALLOW_UNDERSCORE=OFF \
        FORCE_IDN=${lib} check clean || exit 1

    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=ON LABELS_ALLOW_UNDERSCORE=ON \
        FORCE_IDN=${lib} clean || exit 1
    make RFC6531_FOLLOW_RFC5322=OFF RFC6531_FOLLOW_RFC20=ON LABELS_ALLOW_UNDERSCORE=ON \
        FORCE_IDN=${lib} check clean || exit 1

    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=ON LABELS_ALLOW_UNDERSCORE=OFF \
        FORCE_IDN=${lib} clean || exit 1
    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=ON LABELS_ALLOW_UNDERSCORE=OFF \
        FORCE_IDN=${lib} check clean || exit 1

    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=ON LABELS_ALLOW_UNDERSCORE=ON \
        FORCE_IDN=${lib} clean || exit 1
    make RFC6531_FOLLOW_RFC5322=ON RFC6531_FOLLOW_RFC20=ON LABELS_ALLOW_UNDERSCORE=ON \
        FORCE_IDN=${lib} check clean || exit 1
done
