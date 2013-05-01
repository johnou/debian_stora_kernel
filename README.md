debian_squeeze_stora_kernel
===========================

Vanilla Linux kernel for Netgear Stora.

* BFS and -ck patchsets for linux-3.8
* BFQ disk scheduler.
* NOOP I/O scheduler by default.
* ZRAM module enabled.

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
        #if (debarch_eq('all', $v)) {
        #    $fields->{$_} = $v;
        #} else {
        #    my @archlist = split(/\s+/, $v);
        #    my @invalid_archs = grep m/[^\w-]/, @archlist;
        #    warning(ngettext("`%s' is not a legal architecture string.",
        #                    "`%s' are not legal architecture strings.",
        #                    scalar(@invalid_archs)),
        #           join("' `", @invalid_archs))
        #       if @invalid_archs >= 1;
        #    grep(debarch_is($host_arch, $_), @archlist) ||
        #       error(_g("current host architecture '%s' does not " .
        #                "appear in package's architecture list (%s)"),
        #             $host_arch, "@archlist");
        #    $fields->{$_} = $host_arch;
        #}
    } else {
        field_transfer_single($pkg, $fields);
    }

## Download and prepare Linux Kernel sources

```sh
wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.8.9.tar.xz
tar Jxf linux-3.8.9.tar.xz
patch -p1 < 0001-Support-for-Netgear-Stora.patch
```

### Apply patches

## AUFS3

```sh
git clone git://aufs.git.sourceforge.net/gitroot/aufs/aufs3-standalone.git
cd aufs3-standalone/
git checkout origin/aufs3.8
..
```

## BFQ

```sh
patch -p1 < 0001-block-cgroups-kconfig-build-bits-for-BFQ-v6-3.8.patch
patch -p1 < 0002-block-introduce-the-BFQ-v6-I-O-sched-for-3.8.patch
```

## BFS and -ck patchsets

```sh
patch -p1 < patch-3.8-ck1
```

## Build Linux Kernel

```sh
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- EXTRAVERSION=-huuhaa KDEB_PKGVERSION=1.0 KBUILD_DEBARCH=armel deb-pkg
```

## Install Linux Kernel (USB)

```sh
dpkg -i linux-image-3.8.9-huuhaa_1.0_armel.deb
mkimage -A arm -O linux -T kernel  -C none -a 0x00008000 -e 0x00008000 -n Linux-3.8.9-huuhaa -d /boot/vmlinuz-3.8.9-huuhaa /boot/uImage
mkimage -A arm -O linux -T ramdisk -C gzip -a 0x00000000 -e 0x00000000 -n initramfs-3.8.9-huuhaa -d /boot/initrd.img-3.8.9-huuhaa /boot/uInitrd
```

### Accessing NAND

```sh
mkdir /mnt/nand
ubiattach /dev/ubi_ctrl -m 2 -O 2048
mount /dev/ubi0_0 /mnt/nand
```
