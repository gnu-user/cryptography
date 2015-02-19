#!/usr/bin/env python2
##############################################################################
#
# Script which creates a SHA-1 rainbow table of the passwords provided
#
# Copyright (C) 2014, Jonathan Gillett (100437638)
# All rights reserved.
#
##############################################################################
from hashlib import sha1
from base64 import b64encode
import sys

infile, outfile = sys.argv[1], sys.argv[2]

# Open the input and output files
with open(infile, 'r') as inf, open(outfile, 'w') as out:
    # Store the hash and password for each entry in the file
    hashes = [(b64encode(sha1(line.strip()).digest()), line.strip(),) for line in inf]
    # Write out the hash and password
    out.writelines(hash[0] + "\t" + hash[1] + "\n" for hash in hashes)
