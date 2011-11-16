#!/bin/sh
export KBUILD_BUILD_VERSION="Cyano@Onecosmic"
export ANDROID_BUILD_TOP=/Everything/Android/Cyanogenmod
./build.sh galaxysmtd

echo "creating boot.img"
$ANDROID_BUILD_TOP/device/samsung/aries-common/mkshbootimg.py release/boot.img arch/arm/boot/zImage1 $ANDROID_BUILD_TOP/out/target/product/galaxysbmtd/ramdisk.img $ANDROID_BUILD_TOP/out/target/product/galaxysbmtd/ramdisk-recovery.img

echo "launching packaging script"
. release/doit.sh
