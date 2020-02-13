# Barnes-Hut-C
Barnes-Hut algorithm for C. It contains both top-down and bottom-up approaches.

After cloning, to compile type in terminal "make -f bhmakefile"

then run "./td" or if you have valgrind installed to check efficiency of this build in heap by typing:

"valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./td"

or bhalgol.c which contains the bottom-up approach.

Complexity in calculating the force: O(nlogn).
