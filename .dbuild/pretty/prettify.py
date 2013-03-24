#!/usr/bin/env python

import os, sys
import prettyformat
import fileinput

bCustom = True
opcode = "??"
module = "Unknown"
description = "Somebody please fix this!"
offset = 0

if(sys.argv[1] == "--rbuild"):
    bCustom = False
    offset = 1;

if(len(sys.argv) > 1+offset):
    opcode = sys.argv[1+offset]

if(len(sys.argv) > 2+offset):
    module = sys.argv[2+offset]

if(len(sys.argv) > 3+offset):
    description = sys.argv[3+offset]

for line in sys.stdin:
    prettyformat.pretty(opcode, module, line.split("\n")[0], bCustom)
