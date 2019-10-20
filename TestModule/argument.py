#!/usr/bin/python

import getopt
import sys

if __name__ == '__main__':
    opts, args = getopt.getopt(sys.argv[1:], '-p:', ['port='])
    for key, value in opts:
        print key + ':' + value
    # if not opts:
    #     print "no args"
    # else:
    #     print args[0]
