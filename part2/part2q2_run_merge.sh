# Part 2, question 2 script

# try running this 3 times so we get better representation of data
for((i=1; i<=3; i++)); do
	# for each of the available strides
	for stride in -1 1 -2 2 4 -4 8 -8 16 -16; do
		cmd64="/home/jaml/Xmem/bin/xmem-linux-x64 -c64 -w4 -j1 -s -S$stride -v -f part2q2_c64_stride${stride}_run${i}.csv > part2q2_c64_${stride}_run${i}.out"
		cmd32="/home/jaml/Xmem/bin/xmem-linux-x64 -c32 -w4 -j1 -s -S$stride -v -f part2q2_c32_stride${stride}_run${i}.csv > part2q2_c32_${stride}_run${i}.out"
		echo "running: $cmd64"
		eval $cmd64
		echo "running: $cmd32"
		eval $cmd32
	done
done

# merge run 1 results
# for chunk 64
head -1 part2q2_c64_stride1_run1.csv > part2q2_c64_run1_merged.csv
for filename in $(ls part2q2_c64_stride*_run1.csv); do sed 1d $filename >> part2q2_c64_run1_merged.csv; done
#note: run1 gave bad results for chunk 32
#head -1 part2q2_c32_stride1_run1.csv > part2q2_c32_run1_merged.csv
#for filename in $(ls part2q2_c32_stride*_run1.csv); do sed 1d $filename >> part2q2_c32_run1_merged.csv; done
# try run2
head -1 part2q2_c32_stride1_run2.csv > part2q2_c32_run2_merged.csv
for filename in $(ls part2q2_c32_stride*_run2.csv); do sed 1d $filename >> part2q2_c32_run2_merged.csv; done


# This ended up being messy to plot. Left it out.
# merge all chunk size 64 run results
#head -1 part2q2_c64_stride1_run1.csv > part2q2_c64_3runs_merged.csv
#for filename in $(ls part2q2_c64_stride*_run?.csv); do sed 1d $filename >> part2q2_c64_3runs_merged.csv; done
# merge all chunk size 32 run results
#head -1 part2q2_c32_stride1_run1.csv > part2q2_c32_3runs_merged.csv
#for filename in $(ls part2q2_c32_stride*_run?.csv); do sed 1d $filename >> part2q2_c32_3runs_merged.csv; done
