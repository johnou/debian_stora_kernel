debian_squeeze_stora_kernel
===========================

Vanilla Linux kernel for Netgear Stora MS2110.

* AUFS3 support.
* BFS and -ck patchsets for linux-3.8
* BFQ disk scheduler.
* NOOP I/O scheduler by default.
* ZRAM module enabled.

## Install Linux kernel (USB)

```sh
dpkg -i linux-image-3.8.9-huuhaa_1.0_armel.deb
mkimage -A arm -O linux -T kernel  -C none -a 0x00008000 -e 0x00008000 -n Linux-3.8.9-huuhaa -d /boot/vmlinuz-3.8.9-huuhaa /boot/uImage
mkimage -A arm -O linux -T ramdisk -C gzip -a 0x00000000 -e 0x00000000 -n initramfs-3.8.9-huuhaa -d /boot/initrd.img-3.8.9-huuhaa /boot/uInitrd
```

## Accessing NAND

```sh
mkdir /mnt/nand
ubiattach /dev/ubi_ctrl -m 2 -O 2048
mount /dev/ubi0_0 /mnt/nand
```

## Build environment

### Install packages

```sh
echo "deb http://www.emdebian.org/debian squeeze main" >> /etc/apt/sources.list
apt-get update
apt-get install build-essential
apt-get install emdebian-archive-keyring
apt-get install g++-4.4-arm-linux-gnueabi gcc-4.4-arm-linux-gnueabi
```

### Remove architecture validation from /usr/bin/dpkg-gencontrol

    if (field_get_dep_type($_)) {
        # Delay the parsing until later
    } elsif (m/^Architecture$/) {
        my $host_arch = get_host_arch();
        $fields->{$_} = $v;
        # validation removed.. :)
    } else {
        field_transfer_single($pkg, $fields);
    }

## Download and prepare Linux kernel sources

```sh
wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.8.9.tar.xz
tar Jxf linux-3.8.9.tar.xz
patch -p1 < 0001-Support-for-Netgear-Stora.patch
```

## Apply patches

### AUFS3

```sh
git clone git://aufs.git.sourceforge.net/gitroot/aufs/aufs3-standalone.git
cd aufs3-standalone/
git checkout origin/aufs3.8
..
```

### BFQ

```sh
patch -p1 < 0001-block-cgroups-kconfig-build-bits-for-BFQ-v6-3.8.patch
patch -p1 < 0002-block-introduce-the-BFQ-v6-I-O-sched-for-3.8.patch
```

### BFS and -ck patchsets

```sh
patch -p1 < patch-3.8-ck1
```

## Build Linux kernel

```sh
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- EXTRAVERSION=-huuhaa KDEB_PKGVERSION=1.0 KBUILD_DEBARCH=armel deb-pkg
```
