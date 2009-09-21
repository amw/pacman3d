
first: bin

bin: pacman3d

pacman3d: src/Makefile force
	cd src && $(MAKE)

src/Makefile: src/pacman3d.pro force
	cd src && qmake -Wall pacman3d.pro

clean:
	cd src && $(MAKE) clean;
	rm -fr tmp
	rm src/Makefile

doc: doc/doxygen

doc/doxygen: src/* doc/documentation.php doc/Doxyfile
	@doxygen doc/Doxyfile
	@echo "Html documentation updated."

doc-pdf: doc/reference.pdf

doc/reference.pdf: doc/doxygen
	cp doc/latex-makefile doc/doxygen/latex/Makefile
	cp doc/latex-missing-pspicture.ps doc/doxygen/latex/pspicture.ps
	cd doc/doxygen/latex && $(MAKE)
	cp doc/doxygen/latex/refman.pdf doc/reference.pdf

force:

