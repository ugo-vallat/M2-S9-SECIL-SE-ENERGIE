# Execute this script in his directory !
sudo-g5k echo ""


# config mojitos
cd mojitos
./configure.sh
make
sudo cp bin/mojitos /usr/bin/
sudo sh -c 'echo 0 >/proc/sys/kernel/perf_event_paranoid'
sudo chmod o+r /sys/class/powercap/intel-rapl/*/*
sudo chmod o+r /sys/class/powercap/intel-rapl/*/*/*
cd -




