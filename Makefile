
.PHONY: docs all clean

all:
	$(MAKE) -C src
	$(MAKE) -C examples

docs:
	doxygen Doxyfile

clean:
	$(MAKE) -C examples clean
	$(MAKE) -C src clean
	$(RM) -rf docs/html


