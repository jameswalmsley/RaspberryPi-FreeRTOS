#!/usr/bin/python

import os, sys
import prettyformat
import fileinput

action = sys.argv[1]
module = sys.argv[2]

line = sys.stdin.readline()
while(line):
    if("Compiling " in line):
        action = "CC"
    elif("Linking " in line):
        action = "LN"
    elif("checking " in line):
        action = "CHECK"
    elif("Building " in line):
        action = "BUILD"
    else:
        action = "CONF"

    newline = line.split("\n")[0]
    if(action == "CC"):
        newline = newline.split("Compiling ")[1]
    elif(action == "LN"):
        newline = newline.split("Linking ")[1]
    elif(action == "BUILD"):
        newline = newline.split("Building ")[1]

    prettyformat.pretty(action, module, newline, False)
    line = sys.stdin.readline()

