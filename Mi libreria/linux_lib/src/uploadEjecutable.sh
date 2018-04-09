
#!/bin/sh

cd ~/TFG/Mi\ libreria/linux_lib/src/
arm-linux-gnueabihf-gcc *.c -o main.o -Wl,--dynamic-linker=/data/local/lib/ld-linux.so.3 -Wl,-rpath=/data/local/lib -fPIE -pie
adb connect 10.42.0.190
adb push main.o /data/local/tmp

