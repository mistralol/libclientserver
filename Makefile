
.PHONY: docs all clean test

all:
	$(MAKE) -C src
	$(MAKE) -C examples

docs:
	doxygen Doxyfile

test:
	$(MAKE) -C testing

clean:
	$(MAKE) -C examples clean
	$(MAKE) -C testing clean
	$(MAKE) -C src clean
	$(RM) -rf docs/html


