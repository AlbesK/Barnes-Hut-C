#include<stdio.h>
#include"bhHeaders.h"

/*
    Level Order Traversal for force summation ( Breadth first traversal)
*/
void levelorder_force(struct quad* n, struct body* bodies, struct point *Forces, int* N_PARTICLES){

    enqueue(n); // Enqueue Root
    enqueue(NULL); // Extra NUll parameter for checking when the tree goes to the next level after enquing all children in one level.
    struct quad* curr = NULL;
    
    double m = 0;
    double d[2] = {0,0};

    for(int i=0; i<*N_PARTICLES; i++){
    while (!queue_empty())
    {
    
        curr = begin->data;
        dequeue();
        if(curr->data==i){dequeue(); break;}
        difference(&bodies[i].pos, &curr->b->pos, d);
        m = mag(d);
        printf("Data is: %d\n", curr->data);
        printf("|d|:%f, [%f,%f]\n",m,d[0],d[1]);
        printf("s/d = %f\n", curr->s/m);
        if(curr->s/m<=5){ // s/d=θ Barnes-Hut threshold! If its less or equal keep pseudoboy force only for the ith particle
            printf("True\n");
            Forces[i].x = (bodies[i].charge * curr->b->charge)/(m*m*m)*d[0];
            Forces[i].y = (bodies[i].charge * curr->b->charge)/(m*m*m)*d[1];
            printf("TF_[%i] = [%f,%f] \n", i,Forces[i].x, Forces[i].y);
            dequeue(); 

        }
        
        if(curr!=NULL){
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
        else{
            // printf("\n");
            if(!queue_empty()){
                enqueue(NULL);
            }
        }

  
    }
    }
}