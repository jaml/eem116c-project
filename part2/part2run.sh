# Part 2, question 2 script
# try running this 5 times so we get better representation of data
for((i=1; i<=5; i++)); do
	# for each of the available strides
	for stride in -1 1 -2 2 4 -4 8 -8 16 -16; do
		cmd64="/home/jaml/Xmem/bin/xmem-linux-x64 -c64 -w4 -j1 -s -S$stride -v -f part2q2_c64_stride${stride}_run${i}.csv > part2q2_c64_${stride}_run${i}.csv"
		cmd32="/home/jaml/Xmem/bin/xmem-linux-x64 -c32 -w4 -j1 -s -S$stride -v -f part2q2_c32_stride${stride}_run${i}.csv > part2q2_c32_${stride}_run${i}.csv"
		eval $cmd64
		eval $cmd32
	done
done

