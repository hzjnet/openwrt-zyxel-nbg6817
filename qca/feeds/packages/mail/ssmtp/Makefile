#
# Copyright (C) 2007-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ssmtp
PKG_VERSION:=2.64
PKG_RELEASE:=3

PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).orig.tar.bz2
#PKG_SOURCE_URL:=http://ftp.debian.org/debian/pool/main/s/ssmtp
PKG_SOURCE_URL:=$(SVN_REPOSITORY_ROOT)/openwrt/dl
PKG_SOURCE_PROTO:=svn_pkg
PKG_SOURCE_VERSION:=HEAD
PKG_MD5SUM:=65b4e0df4934a6cd08c506cabcbe584f

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS += $(TARGET_CPPFLAGS)

define Package/ssmtp
  SECTION:=mail
  CATEGORY:=Mail
  DEPENDS:=+libopenssl
  TITLE:=A minimal and secure mail sender
  URL:=http://packages.debian.org/ssmtp
endef

define Package/ssmtp/description
 A secure, effective and simple way of getting mail off a system to your
 mail hub. It contains no suid-binaries or other dangerous things - no
 mail spool to poke around in, and no daemons running in the background.
 mail is simply forwarded to the configured mailhost. Extremely easy
 configuration.
endef

define Package/ssmtp/conffiles
/etc/ssmtp/ssmtp.conf
/etc/ssmtp/revaliases
endef

CONFIGURE_VARS += \
	LIBS="$(TARGET_LDFLAGS) -lcrypto -lssl"

CONFIGURE_ARGS += \
	--enable-ssl

define Package/ssmtp/install
	$(INSTALL_DIR) $(1)/etc/ssmtp $(1)/etc/init.d
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/ssmtp.conf $(1)/etc/ssmtp/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/revaliases $(1)/etc/ssmtp/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ssmtp $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/sendmail.init $(1)/etc/init.d/sendmail
endef

define Package/ssmtp/postinst
	ln -sf ssmtp $${IPKG_INSTROOT}/usr/sbin/sendmail
endef

$(eval $(call BuildPackage,ssmtp))
