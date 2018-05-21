#!/bin/sh
#mount -t cpuset cpuset /dev/cpuset/
cd /dev/cpuset
mkdir sys
echo 0-2 > sys/cpus
echo 1 > sys/cpu_exclusive
echo 0 > sys/mems
mkdir rt
echo 3 > rt/cpus
echo 1 > rt/cpu_exclusive
echo 0 > rt/mems
echo 0 > rt/sched_load_balance
echo 1 > rt/mem_hardwall
for i in $(cat tasks); do echo $i > sys/tasks; done

#Disable interrupts on CPU 3 (affinity to CPU 0)
for i in $(find /proc/irq -name "smp_affinity"); do echo 1 > $i; done
echo 1 > /proc/irq/default_smp_affinity

#Fix frequency CPU 3
echo userspace >  /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor
echo 1200000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_setspeed

echo "Haz 'echo $$ > /dev/cpuset/rt/tasks' en el terminal donde quieras aislar los procesos"
