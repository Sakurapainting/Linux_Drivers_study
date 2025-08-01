#!/bin/bash

# 内核源码路径
KERNEL_DIR="/home/you/linux/IMX6ULL/linux/linux-imx-rel_imx_4.1.15_2.1.0_ga/arch/arm/boot"

# 复制 dts
cp "$KERNEL_DIR/dts/imx6ull-alientek-emmc.dts" ./imx6ull-alientek-emmc.dts

# 复制 dtb
cp "$KERNEL_DIR/dts/imx6ull-alientek-emmc.dtb" ./imx6ull-alientek-emmc.dtb

# 复制 zImage
cp "$KERNEL_DIR/zImage" ./zImage

echo "dts, dtb, zImage 已复制到当前目录"