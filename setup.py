#!/usr/bin/env python

from setuptools import setup, Extension

def readme():
    with open('README.rst') as f:
        return f.read()

setup(
    name='uhid-freebsd',
    version='1.1.0',
    author='Michael Gmelin',
    author_email='freebsd@grem.de',
    ext_modules=[Extension('uhid_freebsd', ['uhid_freebsd.cpp'])],
    maintainer='Michael Gmelin',
    maintainer_email='freebsd@grem.de',
    url='https://github.com/grembo/uhid-freebsd',
    description='Get information on FreeBSD uhid devices.',
    long_description=readme(),
    long_description_content_type='text/x-rst',
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
