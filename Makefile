
# Possible values for CFLAGS:
#  see cflags.mk.example
include ./cflags.mk


main: main.c cflags.mk
	#gcc -fno-alias -O3 -xHost -std=c99 -o main main.c
	echo "Using" $(CC) $(CFLAGS)
	$(CC) $(CFLAGS) -o main main.c

clean:
	rm main

results.csv: main
	./main > $@

dataviz: results.csv
	./plot_and_reformat_data.py $<
