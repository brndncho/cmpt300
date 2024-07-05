all: array_test process_info_test

array_test:
	 gcc -std=c99 -D _GNU_SOURCE -static array_stats_test.c -o array_stats_test

process_info_test:
	 gcc -std=c99 -D _GNU_SOURCE -static cmpt_300_testprocessinfo.c -o cmpt_300_testprocessinfo

clean:
	rm -f core *.o array_stats_test cmpt_300_testprocessinfo

transfer:
	scp -P 10022 cmpt_300_testprocessinfo ubuntu@localhost:
	scp -P 10022 array_stats_test ubuntu@localhost: