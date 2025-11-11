#!/usr/bin/env python3
import sys
import subprocess
import time

# Default values
output_file = "tmp/local_output/mojitos_output"
freq = None
cmd_args = []

def print_usage():
    print("Usage: python3 script <freq> <cmd>")
    sys.exit(1)


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

print("Duration (ns) | Energy (Joules) | Pmax (W)")
print(f"{end_time-start_time}|TODO|TODO")
