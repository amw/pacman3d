
first: bin

bin: pacman3d.app

pacman3d.app: src/Makefile force
	cd src && $(MAKE)

src/Makefile: src/pacman3d.pro force
	cd src && qmake pacman3d.pro

clean:
	cd src && $(MAKE) clean;
	rm -fr tmp
	rm src/Makefile

force:
