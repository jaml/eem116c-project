w=4 # init. w to 4 (KiB)

# 256 MiB = 2^18 KiB
for((i=1; i<=18; i++)); do
	let "w*=2"
	# for each of the available strides
	cmd1="/home/jaml/Xmem/bin/xmem-linux-x64 -w${w} -v -l -j1 -f part2q1_w${w}k.csv > part2q1_w${w}k_out.txt"
	eval $cmd1
done

# Merge the CSV files generated, for Excel graphing.
# after running this a few times, it couldn't find part2q1_w4k.csv ??
# I can cat the file right out, so I don't know what the problem is
# doesn't matter, though. Just using for structure.
#head -1 part2q1_w4k.csv > part2q1_merged.csv
head -1 part2q1_w8k.csv > part2q1_merged.csv
for filename in $(ls part2q1_w*k.csv); do sed 1d $filename >> part2q1_merged.csv; done
