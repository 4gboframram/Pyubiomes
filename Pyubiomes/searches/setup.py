from distutils.core import setup, Extension
setup(name = 'searches', version = '0.1',author="", url="", description="search functions for Pyubiomes",
   ext_modules = [Extension('searches', ['wrap.c'])])