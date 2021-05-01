from setuptools import setup, find_packages
setup(name = 'Pyubiomes', version = '0.0.21', description="a (probably bad wip) python wrapper for the C library Cubiomes", author="4gboframram", url="",author_email="<zachawesomeness411@gmail.com>", long_description="description not implemented yet", include_package_data=True,
packages=find_packages(),
package_data={'': ['searches.so']},)