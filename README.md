The ARM C++ 1.11 compiler requires some basic functions to be provided for
construction and destruction of objects and object arrays. The free'ing
of memory is removed from `delete` to deal with the lack of any other generic
'placement delete'. See the https://github.com/trapexit/3do-devkit for details.


## Build

1. source 3do-devkit
2. `make`
