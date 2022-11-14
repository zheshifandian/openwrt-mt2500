KERNEL_LOADADDR := 0x48080000

define Device/glinet_gl-mt2500
  DEVICE_VENDOR := GL.iNet
  DEVICE_MODEL := GL-MT2500
  DEVICE_DTS := mt7981-gl-mt2500
  SUPPORTED_DEVICES := glinet,mt2500-emmc
  DEVICE_DTS_DIR := $(DTS_DIR)/mediatek
  DEVICE_PACKAGES := depends-gl-mt2500
  IMAGE/sysupgrade.bin := sysupgrade-tar | append-metadata
endef
TARGET_DEVICES += glinet_gl-mt2500

define Device/glinet_gl-mt3000
  DEVICE_VENDOR := GL.iNet
  DEVICE_MODEL := GL-MT3000
  DEVICE_DTS := mt7981-gl-mt3000
  DEVICE_DTS_DIR := $(DTS_DIR)/mediatek
  SUPPORTED_DEVICES := glinet,mt3000-snand
  DEVICE_PACKAGES := kmod-hwmon-pwmfan
  UBINIZE_OPTS := -E 5
  BLOCKSIZE := 128k
  PAGESIZE := 2048
  IMAGE_SIZE := 65536k
  KERNEL_IN_UBI := 1
  IMAGES += factory.bin
  IMAGE/factory.bin := append-ubi | check-size $$$$(IMAGE_SIZE)
  IMAGE/sysupgrade.bin := sysupgrade-tar | append-metadata
endef
#TARGET_DEVICES += glinet_gl-mt3000

define Device/glinet_gl-x3000
  DEVICE_VENDOR := GL.iNet
  DEVICE_MODEL := GL-X3000
  DEVICE_DTS := mt7981-gl-x3000
  DEVICE_DTS_DIR := $(DTS_DIR)/mediatek
  SUPPORTED_DEVICES := glinet,x3000-snand
  DEVICE_PACKAGES := kmod-hwmon-pwmfan
  UBINIZE_OPTS := -E 5
  BLOCKSIZE := 128k
  PAGESIZE := 2048
  IMAGE_SIZE := 65536k
  KERNEL_IN_UBI := 1
  IMAGES += factory.bin
  IMAGE/factory.bin := append-ubi | check-size $$$$(IMAGE_SIZE)
  IMAGE/sysupgrade.bin := sysupgrade-tar | append-metadata
endef
#TARGET_DEVICES += glinet_gl-x3000
