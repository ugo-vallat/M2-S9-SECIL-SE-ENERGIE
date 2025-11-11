# Execute this script in his directory !

sudo cp mojitos/bin/mojitos /usr/bin/
sudo sh -c 'echo 0 >/proc/sys/kernel/perf_event_paranoid'
sudo chmod o+r /sys/class/powercap/intel-rapl/*/*
sudo chmod o+r /sys/class/powercap/intel-rapl/*/*/*


