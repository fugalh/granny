test:
	scons
	./tests

clean:
	scons -c

loop:
	while true; do echo; make test; fswatch .; done

.PHONY: test clean
