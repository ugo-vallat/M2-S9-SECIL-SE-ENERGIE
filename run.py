#!/usr/bin/env python3
import sys
import subprocess
import time
import pandas as pd


# Default values
output_file = "tmp/local_output/mojitos_output"
freq = None
cmd_args = []

def print_usage():
    print("Usage: python3 script <freq> <cmd>")
    sys.exit(1)

def _read_csv(filename):
    df = pd.read_csv(filename, sep=' ', skipinitialspace=True)
    if df.columns[-1].startswith('Unnamed'):
        df.drop(columns=df.columns[-1:], axis=1, inplace=True)
    return df

def run_test(freq, read_freq, cmd):
    # Execute the command
    start_time = time.time_ns()

    freq_cmd = f"echo {freq} | tee /sys/devices/system/cpu/cpufreq/policy*/scaling_max_freq > /dev/null"

    try:
        # print(f"Executing: {freq_cmd}")
        subprocess.run(freq_cmd, shell=True, check=False)
        subprocess.run(cmd, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error: Command failed with exit code {e.returncode}")
        sys.exit(e.returncode)

    end_time = time.time_ns()

    df = _read_csv(output_file)
    energy = df.iloc[:, 1:].sum().sum() / 1e6
    energy_max = (df.iloc[:, 1:].sum(axis=1)).max() / 1e6
    df["#timestamp_ns"] = (df["#timestamp"].astype(float) * 1e9).astype("int64")
    duration_ns = df["#timestamp_ns"].max() - df["#timestamp_ns"].min()
    pmax = energy_max / (1/read_freq)


    print(f"{freq}|{duration_ns/1e9:.9f}|{energy:.6f}|{pmax:.6f}")

if len(sys.argv) < 2:
    print_usage()

args = sys.argv[1:]
freq = args[0]
cmd_args=args[1:]

# Build the mojitos command
mojitos_cmd = ["mojitos", "-r"]
mojitos_cmd += ["-f", freq]
mojitos_cmd += ["-o", output_file]
mojitos_cmd += ["--"] + cmd_args

# print(f"Executing: {' '.join(mojitos_cmd)}")



print("Freq (Hz) | Duration (s) | Energy (Joules) | Pmax (W)")

for run_freq in range (1_000_000, 3_900_000, 100_000):
    run_test(run_freq, int(freq), mojitos_cmd)
