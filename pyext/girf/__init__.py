# This file is created automatically by setup.py
# Please do not modify, unless you exactly know what you do.
__all__ = ['GIRF', 'SwigPyIterator', 'SwigPyIterator', 'SwigPyIterator', 'ass _object : pass', 'ass _object : pass', 'ass _object : pass', 'ewclass = 0', 'ewclass = 0', 'ewclass = 0', 'ewclass = 1', 'ewclass = 1', 'ewclass = 1', 'f __init__', 'f __init__', 'f __init__']
from girf.convert import *
from girf.simpleTest import *
from girf.irf import *

bad_entries = [entry for entry in list(locals())
               if entry.endswith('_swigregister')]
for entry in bad_entries:
    del locals()[entry]
#del girf
