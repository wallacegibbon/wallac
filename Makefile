CC=gcc
CFLAGS=-w
OUTDIR=./output
$(shell mkdir -p $(OUTDIR))

main.elf: main.o compile.o lexer.o misc.o assertch.o checkch.o
	cd output/ && $(CC) $(CFLAGS) -o $@ $^

main.o: src/main.c src/misc.h
	$(CC) $(CFLAGS) -c -o $(OUTDIR)/$@ $<

compile.o: src/compile.c
	$(CC) $(CFLAGS) -c -o $(OUTDIR)/$@ $<

lexer.o: src/lexer.c
	$(CC) $(CFLAGS) -c -o $(OUTDIR)/$@ $<

misc.o: src/misc.c src/misc.h
	$(CC) $(CFLAGS) -c -o $(OUTDIR)/$@ $<

assertch.o: src/assertch.c src/assertch.h
	$(CC) $(CFLAGS) -c -o $(OUTDIR)/$@ $<

checkch.o: src/checkch.c src/checkch.h
	$(CC) $(CFLAGS) -c -o $(OUTDIR)/$@ $<


.PHONY: clean

clean:
	rm -rf $(OUTDIR)
