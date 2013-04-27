debian_squeeze_stora_kernel
===========================

Vanilla Linux kernel for Netgear Stora.

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

## Download and prepare Kernel sources

```sh
wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.8.9.tar.xz
tar Jxf linux-3.8.9.tar.xz
patch -p1 < 0001-Support-for-Netgear-Stora.patch
```

### Apply aufs3 patches

```sh
git clone git://aufs.git.sourceforge.net/gitroot/aufs/aufs3-standalone.git
cd aufs3-standalone/
git checkout origin/aufs3.8
..
```

## Build Kernel

```sh
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- EXTRAVERSION=-huuhaa KDEB_PKGVERSION=1.0 KBUILD_DEBARCH=armel deb-pkg
```

## Install Kernel (USB)

```sh
dpkg -i linux-image-3.8.9-huuhaa_1.0_armel.deb
mkimage -A arm -O linux -T kernel  -C none -a 0x00008000 -e 0x00008000 -n Linux-3.8.9-huuhaa -d /boot/vmlinuz-3.8.9-huuhaa /boot/uImage
mkimage -A arm -O linux -T ramdisk -C gzip -a 0x00000000 -e 0x00000000 -n initramfs-3.8.9-huuhaa -d /boot/initrd.img-3.8.9-huuhaa /boot/uInitrd
```
