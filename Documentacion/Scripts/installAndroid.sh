
#!/bin/sh

cd ~/RaspAndroid712
echo "Copiando todo a System"
sudo dd if=out/target/product/rpi3/system.img of=/dev/sdb2 bs=1M
cd kernel/rpi
sudo rm -r temp
mkdir temp
sudo mount /dev/sdb2 temp/
sudo make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=temp modules_install
sudo umount temp/
sudo rm -r temp/
cd ~/RaspAndroid712
sudo rm -r temp
mkdir temp
sudo mount /dev/sdb1 temp/
sudo cp device/brcm/rpi3/boot/* temp/
sudo cp kernel/rpi/arch/arm/boot/zImage temp/
sudo cp kernel/rpi/arch/arm/boot/dts/bcm2710-rpi-3-b.dtb temp/
sudo mkdir temp/overlays
sudo cp kernel/rpi/arch/arm/boot/dts/overlays/vc4-kms-v3d.dtbo temp/overlays/vc4-kms-v3d.dtbo
sudo cp out/target/product/rpi3/ramdisk.img temp/
sudo umount temp/
rm -r temp
