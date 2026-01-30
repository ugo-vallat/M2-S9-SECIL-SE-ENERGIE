#!/usr/bin/env python3
import sys
import subprocess
import time
import pandas as pd


# Default values
output_file = "tmp/local_output/mojitos_output"
read_freq = 100000
cpu_freq = 3_900_000

def _read_csv(filename):
    df = pd.read_csv(filename, sep=' ', skipinitialspace=True)
    if df.columns[-1].startswith('Unnamed'):
        df.drop(columns=df.columns[-1:], axis=1, inplace=True)
    return df


def run_one(read_freq, cmd) :
    try:
        subprocess.run(cmd, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error: Command failed with exit code {e.returncode}")
        sys.exit(e.returncode)


    df = _read_csv(output_file)
    energy = df.iloc[:, 1:].sum().sum() / 1e6
    energy_max = (df.iloc[:, 1:].sum(axis=1)).max() / 1e6
    df["#timestamp_ns"] = (df["#timestamp"].astype(float) * 1e9).astype("int64")
    duration_ns = df["#timestamp_ns"].max() - df["#timestamp_ns"].min()
    pmax = energy_max / (1/read_freq)

    return (duration_ns, energy, pmax)

def run_test(cpu_freq, read_freq, cmd):
    global ITE

    freq_cmd = f"echo {cpu_freq} | tee /sys/devices/system/cpu/cpufreq/policy*/scaling_max_freq > /dev/null"
    try:
        subprocess.run(freq_cmd, shell=True, check=False)
    except subprocess.CalledProcessError as e:
        print(f"Error: Command failed with exit code {e.returncode}")
        sys.exit(e.returncode)

    duration_ns=0
    energy=0
    pmax=0
    
    for _ in range(ITE):
        tmp_duration_ns, tmp_energy, tmp_pmax = run_one(read_freq, cmd)
        duration_ns += tmp_duration_ns
        energy += tmp_energy
        pmax += tmp_pmax

    duration_ns = duration_ns / ITE
    energy = energy / ITE
    pmax = pmax / ITE

    print(f"{cpu_freq}|{duration_ns/1e9:.9f}|{energy:.6f}|{pmax:.6f}")

# Build the mojitos command
mojitos_cmd = ["mojitos", "-r"]
mojitos_cmd += ["-f", f"{read_freq}"]
mojitos_cmd += ["-o", output_file]
mojitos_cmd += ["--", "./src/julia"]


print("Freq (Hz) | Duration (s) | Energy (Joules) | Pmax (W)")
run_test(cpu_freq, read_freq, mojitos_cmd)
