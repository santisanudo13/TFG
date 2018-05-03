
#!/bin/sh

cd /home/santi/TFG/Mi_Libreria/Mains/finales
arm-linux-gnueabihf-gcc main_stress_cpu.c -o stressCpu.o -Wl,--dynamic-linker=/data/local/lib/ld-linux.so.3 -Wl,-rpath=/data/local/lib -fPIE -pie
adb connect 10.42.0.190
adb push stressCpu.o /data/local/tmp

