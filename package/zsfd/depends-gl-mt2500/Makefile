#
# Copyright (C) 2015 OpenWrt-dist
#
# This is free software, licensed under the GNU General Public License v3.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=depends-gl-mt2500
PKG_VERSION:=0.1
PKG_RELEASE:=$(COMMITCOUNT)

PKG_LICENSE:=GPLv3
PKG_LICENSE_FILES:=LICENSE
PKG_MAINTAINER:=Zheshifandian

include $(INCLUDE_DIR)/package.mk

define Package/depends-gl-mt2500
  TITLE:=Depends for gl-mt2500
  MAINTAINER:=Zheshifandian
  DEPENDS:=@(aarch64||mt7981) \
  	+blkid +blockdev +gdisk \
  	+kmod-fs-f2fs +kmod-mediatek_hnat +kmod-mmc \
  	+mkf2fs +losetup +mii_mgr +mtk_factory_rw +regs +switch
  PKGARCH:=all
endef

define Package/depends-gl-mt2500/description
	Depends for gl-mt2500
endef

define Build/Compile
endef

define Package/depends-gl-mt2500/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DATA) ./etc/version.type $(1)/etc/version.type
endef

$(eval $(call BuildPackage,depends-gl-mt2500))
