#!/usr/bin/python

import os, sys
import prettyformat
import fileinput

module = sys.argv[1]

line = sys.stdin.readline()
while(line):
    action = line.strip().split(" ")[0]
    newline = line.strip().split("\n")[0]
    newline = newline.split(" ", 1)[1].strip();
    description = newline
    prettyformat.pretty(action, module, description, False)
    line = sys.stdin.readline()

