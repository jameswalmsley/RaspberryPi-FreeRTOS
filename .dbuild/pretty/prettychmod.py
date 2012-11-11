#!/usr/bin/python

import os, sys
import prettyformat
import fileinput

action = "CHMOD"
module = sys.argv[1]

for line in sys.stdin:
    description = line.split("\n")[0]
    prettyformat.pretty(action, module, description)

