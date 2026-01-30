make clean
make julia

total_us=0

for i in {1..10}; do
  start_ns=$(date +%s%N)
  ./julia >/dev/null
  end_ns=$(date +%s%N)

  dur_ns=$((end_ns - start_ns))
  dur_us=$((dur_ns / 1000))

  echo "Run $i: ${dur_us} µs"
  total_us=$((total_us + dur_us))
done

avg_us=$((total_us / 10))
echo "Moyenne: ${avg_us} µs"
