#!/bin/bash

# 1. Make shared library object
make lib

# 2. Run test cases
pushd wrapper/python
cp ../../kats/sphincs-shake-{128,192,256}{s,f}-{robust,simple}.kat .
python3 -m pytest -v -x
rm sphincs-shake-{128,192,256}{s,f}-{robust,simple}.kat
popd
