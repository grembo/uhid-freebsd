all: py27 py36

test: py27-test py36-test

install: py27-install py36-install

py27:
	python2.7 setup.py build

py27-test: py27
	PYTHONPATH=build/lib.freebsd-`uname -r`-`uname -p`-2.7 \
		python2.7 test.py

py27-install: py27
	python2.7 setup.py install

py36:
	python3.6 setup.py build

py36-test: py36
	PYTHONPATH=build/lib.freebsd-`uname -r`-`uname -p`-3.6 \
		python3.6 test.py

py36-install: py36
	python3.6 setup.py install
