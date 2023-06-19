build:
	cc -w -o luck luck.c

install: build
	mkdir /usr/local/bin
	mv luck /usr/local/bin/luck
        chmod 755 fortune
	mkdir /usr/local/games/fortune
	cp fortune /usr/local/bin/fortune
	cp files/* /usr/local/games/fortune/

uninstall:
	rm /usr/local/bin/luck
	rm /usr/local/bin/fortune
	rm -rf /usr/local/games/fortune

clean:
	rm luck
