debian_squeeze_stora_kernel
===========================

Linux kernel for Netgear Stora MS2110.

* AUFS3 support.
* BFS and -ck patchsets for linux-3.9
* BFQ disk scheduler.
* NOOP I/O scheduler by default.
* ZRAM module enabled.

## Install Linux kernel (USB)

```sh
dpkg -i linux-image-3.9.6-ck1-huuhaa_1.0_armel.deb
mkimage -A arm -O linux -T kernel  -C none -a 0x00008000 -e 0x00008000 -n Linux-3.9.6-ck1-huuhaa -d /boot/vmlinuz-3.9.6-ck1-huuhaa /boot/uImage
mkimage -A arm -O linux -T ramdisk -C gzip -a 0x00000000 -e 0x00000000 -n initramfs-3.9.6-ck1-huuhaa -d /boot/initrd.img-3.9.6-ck1-huuhaa /boot/uInitrd
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
wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.9.6.tar.xz
tar Jxf linux-3.9.6.tar.xz
patch -p1 < 0001-Support-for-Netgear-Stora.patch
```

## Apply patches

### AUFS3

```sh
git clone git://aufs.git.sourceforge.net/gitroot/aufs/aufs3-standalone.git
cd aufs3-standalone/
git checkout origin/aufs3.x-rcN
rm include/uapi/linux/Kbuild 
cp -rp *.patch ../
cp -rp fs ../
cp -rp Documentation/ ../
cp -rp include/ ../
cd ..
patch -p1 < aufs3-kbuild.patch
patch -p1 < aufs3-base.patch
patch -p1 < aufs3-standalone.patch
```

### BFQ

```sh
patch -p1 < 0001-block-cgroups-kconfig-build-bits-for-BFQ-v6r1-3.9.patch 
patch -p1 < 0002-block-introduce-the-BFQ-v6r1-I-O-sched-for-3.9.patch 
patch -p1 < 0003-block-bfq-add-Early-Queue-Merge-EQM-to-BFQ-v6r1-for-3.9.0.patch
```

### BFS and -ck patchsets

```sh
patch -p1 < patch-3.9-ck1 
```

## Build Linux kernel

```sh
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- EXTRAVERSION=-ck1-huuhaa KDEB_PKGVERSION=1.0 KBUILD_DEBARCH=armel deb-pkg
```
