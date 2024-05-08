# Makefile
MANPREFIX = /usr/local/share/man/man1

.PHONY: install-man

install-man:
	@echo "Installing manpage in $(DESTDIR)$(MANPREFIX)"
	@install -m 0755 docs/dotsig.1 $(DESTDIR)$(MANPREFIX)
	@gzip $(DESTDIR)$(MANPREFIX)/dotsig.1
