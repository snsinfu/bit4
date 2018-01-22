VERSION = 1.8.9.1
DISTFILE = doxygen-$(VERSION).src.tar.gz
SRCDIR = doxygen-$(VERSION)
URL = https://ftp.stack.nl/pub/users/dimitri/$(DISTFILE)

# macOS Homebrew
LLVM_PREFIX = /usr/local/opt/llvm

.PHONY: clean distclean

bin/doxygen: $(SRC)/Makefile
	@make -C $(SRCDIR)
	@make -C $(SRCDIR) install

$(SRC)/Makefile: $(SRCDIR)
	@cd $(SRCDIR); \
	 PATH="$$PATH:$(LLVM_PREFIX)/bin" ./configure --prefix=.. --with-libclang

$(SRCDIR):
	@tar xzf $(DISTFILE)

$(DISTFILE):
	@curl -fsSLO "$(URL)"

clean: srcclean uninstall
	@:

uninstall:
	@rm -rf man bin

srcclean:
	@rm -rf $(SRCDIR)

distclean:
	@rm -f $(DISTFILE)
