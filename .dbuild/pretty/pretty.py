#!/usr/bin/env python

import sys
import prettyformat

bCustom = True
opcode = "??"
module = "Unknown"
description = "Please fix this somebody!"
offset = 0

if(sys.argv[1] == "--dbuild"):
    bCustom = False
    offset = 1;

if(len(sys.argv) > 1+offset):
    opcode = sys.argv[1+offset]

if(len(sys.argv) > 2+offset):
    module = sys.argv[2+offset]

if(len(sys.argv) > 3+offset):
    description = sys.argv[3+offset]

prettyformat.pretty(opcode, module, description, bCustom)
