#!/usr/bin/env python

from distutils.core import setup, Extension
setup(
    name='uhid-freebsd',
    version='1.0',
    author='Michael Gmelin',
    author_email='freebsd@grem.de',
    ext_modules=[Extension('uhid_freebsd', ['uhid_freebsd.cpp'])],
    maintainer='Michael Gmelin',
    maintainer_email='freebsd@grem.de',
    url='https://github.com/grembo/uhid-freebsd',
    description='Get information on FreeBSD uhid devices.',
    license='BSD 2-Clause',
    platforms='FreeBSD',
    classifiers=[
        'License :: OSI Approved :: BSD License',
        'Operating System :: POSIX :: BSD :: FreeBSD',
        'Programming Language :: Python',
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Topic :: Software Development'
    ]
)
