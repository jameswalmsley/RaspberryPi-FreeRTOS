#!/usr/bin/python

import os, sys
import prettyformat
import fileinput

command     = "CP"
module      = "Unknown"
description = "Please fix this somebody!"
bCustom     = True
offset      = 0

if(len(sys.argv) >= 2):
    if(sys.argv[1] == "--rbuild"):
        offset = 1
        bCustom = False

if(len(sys.argv) >= 2+offset):
    command = sys.argv[1+offset]

if(len(sys.argv) >= 3+offset):
    module = sys.argv[2+offset]

for line in sys.stdin:
    source = line.split("`")[1].split("'")[0]
    dest = line.split("`")[2].split("'")[0]
    prettyformat.pretty(command, module, source + " -> " + dest, bCustom)
