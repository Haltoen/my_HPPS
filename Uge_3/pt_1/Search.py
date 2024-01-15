import sys

fname = sys.argv[1]
if b"A" in (open(fname, "rb").read()) :
    print("A")
else:
    print("No A")