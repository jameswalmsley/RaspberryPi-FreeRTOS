#!/usr/bin/python

import sys
import prettyformat

if(len(sys.argv) != 3):
    sys.exit(0)

myfile = open(sys.argv[1], "a")

for line in sys.stdin:
    splits = line.split(":", 2)
    filename = splits[0]
    lineno   = splits[1]
    description = splits[2].strip()
    info = "%-25s : %-5s : %-15s : %s" % (filename, lineno, sys.argv[2][:15], description)
    prettyformat.pretty("TODO", sys.argv[2], info);
    myfile.write(info + "\n")

myfile.close()
