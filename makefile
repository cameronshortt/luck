build:
	cc -w -o luck luck.c

install: build
	mkdir /usr/local/bin
	mv luck /usr/local/bin/luck
	mv fortune /usr/local/bin/fortune

uninstall:
	rm /usr/local/bin/luck
	rm /usr/local/bin/fortune

clean:
	rm luck
