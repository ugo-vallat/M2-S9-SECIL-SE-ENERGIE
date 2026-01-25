#!/usr/bin/env python3
import sys
import subprocess
import time
import pandas as pd


# Default values
output_file = "tmp/local_output/mojitos_output"
FREQ = 100
ITE = 5
cmd_args = []

# def print_usage():
#     print("Usage: python3 script <cmd>")
#     sys.exit(1)

def _read_csv(filename):
    df = pd.read_csv(filename, sep=' ', skipinitialspace=True)
    if df.columns[-1].startswith('Unnamed'):
        df.drop(columns=df.columns[-1:], axis=1, inplace=True)
    return df

def run_one(read_freq, cmd) :
    print(cmd)
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



def run_test(freq, read_freq, algo, cmd):
    global ITE

    freq_cmd = f"echo {freq} | tee /sys/devices/system/cpu/cpufreq/policy*/scaling_max_freq > /dev/null"
    try:
        subprocess.run(freq_cmd, shell=True, check=False)
    except subprocess.CalledProcessError as e:
        print(f"Error: Command failed with exit code {e.returncode}")
        sys.exit(e.returncode)

    print("Freq config")
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

    print(f"{freq}|{algo}|{duration_ns/1e9:.9f}|{energy:.6f}|{pmax:.6f}")

# if len(sys.argv) < 2:
#     print_usage()

# args = sys.argv[1:]
# freq = args[0]
# cmd_args=args[1:]

# Build the mojitos command
base_cmd = ["mojitos", "-r"]
base_cmd += ["-f", f"{FREQ}"]
base_cmd += ["-o", output_file]
base_cmd += ["--", "./app"]


# print(f"Executing: {' '.join(mojitos_cmd)}")

algos = [
    "ALGO_STATIC_1D_CHAR",
    "ALGO_STATIC_1D_U32",
    "ALGO_STATIC_3D_CHAR",
    "ALGO_ALLOC_1D_CHAR",
    "ALGO_ALLOC_1D_U32",
    "ALGO_ALLOC_3D_CHAR"
]


print("Freq (Hz) | Algo | Duration (s) | Energy (Joules) | Pmax (W)")

for run_freq in range (1_100_000, 3_900_001, 700_000):
    for algo in range(len(algos)):
        run_test(run_freq, int(FREQ), algos[algo], base_cmd + [f"{algo}"])
