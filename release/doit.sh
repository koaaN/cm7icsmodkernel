#!/bin/bash

[[ -d release ]] || {
	echo "must be in kernel root dir"
	exit 1;
}

echo "packaging it up"

RELVER=$1

REL=CM7_ICSSystem350_${RELVER}_update.zip

rm -r release/system 2> /dev/null
mkdir -p release/system/lib/modules || exit 1
find . -name "*.ko" -exec cp {} release/system/lib/modules/ \; 2>/dev/null || exit 1

cd release && {
	zip -q -r ${REL} system boot.img META-INF bml_over_mtd bml_over_mtd.sh || exit 1
	sha256sum ${REL} > ${REL}.sha256sum
} || exit 1

rm system/lib/modules/*
echo ${REL}
