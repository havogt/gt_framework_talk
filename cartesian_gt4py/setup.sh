#!/bin/sh

python3 -m venv .venv
source .venv/bin/activate
pip install git+https://github.com/GridTools/gt4py.git

