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


if len(sys.argv) < 2:
    print_usage()

args = sys.argv[1:]
freq = args[0]
cmd_args=args[1:]

# Build the mojitos command
mojitos_cmd = ["mojitos", "-u"]
mojitos_cmd += ["-f", freq]
mojitos_cmd += ["-o", output_file]
mojitos_cmd += ["--"] + cmd_args

print(f"Executing: {' '.join(mojitos_cmd)}")

# Execute the command
start_time = time.time_ns()

try:
    subprocess.run(mojitos_cmd, check=True)
except subprocess.CalledProcessError as e:
    print(f"Error: Command failed with exit code {e.returncode}")
    sys.exit(e.returncode)

end_time = time.time_ns()

df = _read_csv(output_file)
df["#timestamp_ns"] = (df["#timestamp"].astype(float) * 1e9).astype("int64")
duration_ns = df["#timestamp_ns"].max() - df["#timestamp_ns"].min()
energy = df.iloc[:, 1:].sum().sum() / 1_000_000
pmax = energy / (duration_ns / 1e9)


print("Duration (ns) | Energy (Joules) | Pmax (W)")
print(f"{duration_ns:.6f}|{energy:.6f}|{pmax:.6f}")
