## Process this file with automake to produce Makefile.in

VERS = 0.1.10

INCLUDES += @GTK_CFLAGS@ @MYSQL_INCLUDE@
LIBS += @MYSQL_LIB@

bin_PROGRAMS = gentry

gentry_SOURCES = \
	gladesrc.c gladesrc.h \
	gladesig.c gladesig.h
gentry_LDADD = @GTK_LIBS@

# a rule to make snapshots
snapshot: $(DISTFILES)
	@echo
	@echo "->Note: The version for now is hacked into Makefile.in as"
	@echo "->" $(VERS)
	@echo
	@echo "->copying all release files to the directory " gentry-$(VERS)
	@echo
	tar cf - $(DISTFILES) | gzip > gentry-$(VERS).tar.gz
	-mkdir gentry-$(VERS)
	gzcat gentry-$(VERS).tar.gz | (cd gentry-$(VERS); tar xf -)
	/bin/rm -f gentry-$(VERS).tar.gz
	@echo
	@echo "->making the compressed tar file " gentry-$(VERS).tar.gz
	@echo
	tar cf - gentry-$(VERS) | gzip > gentry-$(VERS).tar.gz
	@echo
	#	@echo "->placing the snapshot for anonymous ftp in " $(FTPDIR)
	#	@echo
	#	rcp gentry-$(VERS).tar.gz $(FTPDIR)
	echo "->removnig the temporary directory " gentry-$(VERS)
	/bin/rm -rf gentry-$(VERS)             # remove the old directory


