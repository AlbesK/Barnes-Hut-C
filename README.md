# Barnes-Hut-C
Barnes-Hut algorithm for C. It contains both top-down and bottom-up approaches.

Second numerical method used for my dissertation on accelerating the force calcuation for a cloud of electric charges with standard particle-particle model from complexity O(n<sup>2</sup>) to O(nlogn).

[More on the theory here](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation)


# Usage

After cloning, to compile type in terminal "make -f bhmakefile"

then run "./td" or if you have valgrind installed to check efficiency of this build in heap by typing:

"valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./td"

or bhalgol.c which contains the bottom-up approach.


