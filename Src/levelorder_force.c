#include<stdio.h>
#include"bhHeaders.h"

/*
    Level Order Traversal for force summation ( Breadth first traversal)
*/
void levelorder_force(struct quad* n, struct body* bodies, struct point *Forces, int* N_PARTICLES){

    enqueue(n); // Enqueue Root
    enqueue(NULL); // Extra NUll parameter for checking when the tree goes to the next level after enquing all children in one level.
    struct quad* curr = NULL; // Current node tracker
    
    double m = 0; // Magnitude component
    double d[2] = {0,0}; // Vector component for force calculation

    for(int i=0; i<*N_PARTICLES; i++){ // For all particles loop
    while (!queue_empty()) // While the queue has elements to be accessed
    {
    
        curr = begin->data; // Save at current node the begining of the queue ( FIFO )
        dequeue(); // Delete the node begin is pointing at so it moves to the next pointer

        // if(curr->data==i){dequeue(); break;}
        difference(&bodies[i].pos, &curr->b->pos, d); // Find vector component between i-th Body and the Pseudobody curr is pointing at
        m = mag(d); // Magnitude of said vector for Force calculation

        printf("Data is: %d\n", curr->data);
        printf("|d|:%f, [%f,%f]\n",m,d[0],d[1]);
        printf("s/d = %f\n", curr->s/m);

        if((curr->s/m)<=5){ // s/d=θ Barnes-Hut threshold! If its less or equal keep pseudoboy force only for the ith particle
            printf("True\n");
            Forces[i].x = (bodies[i].charge * curr->b->charge)/(m*m*m)*d[0];
            Forces[i].y = (bodies[i].charge * curr->b->charge)/(m*m*m)*d[1];
            printf("TF_[%i] = [%f,%f] \n", i,Forces[i].x, Forces[i].y);
            dequeue(); // Dequeue Node 

        }
        
        if(curr!=NULL){ // If Curr is not NULL the this node is pointing at has children to be added
            if (curr->NE)
                enqueue(curr->NE);
            if (curr->SE)
                enqueue(curr->SE);
            if (curr->SW)
                enqueue(curr->SW);
            if (curr->NW)
                enqueue(curr->NW);
            printf("%d ",curr->data);
        }
        else{ // Else it does not, thus put NULL to next reference to show where this level ends.
            // printf("\n");
            if(!queue_empty()){
                enqueue(NULL);
            }
        }

  
    }
    }
}