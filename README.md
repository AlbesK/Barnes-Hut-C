# Barnes-Hut-C
Barnes-Hut algorithm for C. It contains both top-down and bottom-up approaches.

Second numerical method used for my dissertation on accelerating the force calculation for a cloud of electric charges with standard particle-particle model giving complexity O(n<sup>2</sup>) to this with O(nlogn).

# Theory

The Barnes-Hut algorithm works by first hierarchically segmenting the space the particles reside in by dividing the space into 4 equal squares for 2D or by dividing in 8 for 3D cases. This can be done by using quad-trees and oct-trees, data structures in computer science that are used to keep track of 2D and 3D space and have plenty of uses with some notable ones being on rasterising images, better compression rates of images, keeping track of multiple bodies collision by checking only the immediate neighbours.

After subdivision the particles are aggregated accordingly to the squares they reside in up to the original square containing the whole region in the root of the tree. Their masses, positions and charges are summed as we go up the tree to calculate the pseudo-particles that reside in each square with increasing size. 

<img src="https://github.com/AlbesK/Barnes-Hut-C/blob/master/images/BU30PB.png" alt="Pseudobodies in quad trees" class="center" width=80%/>

These Pseudo-particles are nothing more but an abstract representation of the lump of particles they encompass. The green pseudo-particle shown in the above figure represents the root level sum of the 30 particles in the system. Sizes are for qualitative purposes only to show the aggregation process.

Then by using a threshold in the force calculation as shown below of s/d=&theta;, where s is the size of the square and d is the distance apart from the real particle and the pseudo-particle, we can pick &theta; as any value but usually picked with values between 0 and 1[2,3] that if the value for one body in the force calculation is less than the threshold then the force calculation is only summing the contribution of the pseudo-body's (PB) effect on the current body.

<img src="https://github.com/AlbesK/Barnes-Hut-C/blob/master/images/bh.png" alt="S/D" class="center" width=70%/>

If the value is bigger then we have to go down the tree and get more particles that are making that pseudo-body and then iterate through those to find the force. 

This ends up lowering the number of particle contributions and one can see from the figure above and that by choosing a greater value of &theta; we can get force summation at different levels with higher values dropping the number of particles faster in the system and thus one can see physically and not just computationally how it can scale by &#x2248; O(nlogn) < O(n<sup>2</sup>).

## References

[1] Barnes J, Hut P. A hierarchical O (N log N) force-calculation algorithm. nature. 1986 Dec;324(6096):446-9.

[2] Pfalzner S, Gibbon P. Many-body tree methods in physics. Cambridge University Press; 2005 Sep 8.

[3] Pfalzner S, Gibbon P. A 3D hierarchical tree code for dense plasma simulation. Computer physics communications. 1994 Feb 1;79(1):24-38.

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
