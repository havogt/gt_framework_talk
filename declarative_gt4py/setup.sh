#!/bin/sh

python3.10 -m venv .venv
source .venv/bin/activate
pip install --upgrade pip setuptools wheel
pip install git+https://github.com/havogt/gt4py.git@35f2f307069f802a21b4373644a14e15ebc20d5c

