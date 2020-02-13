# Barnes-Hut-C
Barnes-Hut algorithm for C. It contains both top-down and bottom-up approaches.

Second numerical method used for my dissertation on accelerating the force calculation for a cloud of electric charges with standard particle-particle model giving complexity O(n<sup>2</sup>) to this with O(nlogn).

[More on the theory here](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation)


# Usage
## Top-Down:
After cloning, to compile type in terminal "make -f bhmakefile"

then run "./td" 

## Bottom-up:

Just compile the file bhalgol.c and execute the output.

## Memory check:

if you have [valgrind](https://valgrind.org/) installed to check efficiency of this build in heap by typing:

"valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./td"
