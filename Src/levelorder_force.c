#include<stdio.h>
#include"bhHeaders.h"

/*
    Level Order Traversal for force summation ( Breadth first traversal)
*/
void levelorder_force(struct quad* n, struct body* bodies, struct point *Forces, int* N_PARTICLES, double theta){

    struct quad* curr; // Current node tracker
    double m = 0; // Magnitude component
    double d[2] = {0,0}; // Vector component for force calculation
    int if_leaf = 0; //Check if leaf node if it has 0 children
    int temporary_data_label = 0;
    double k, k2 = 0; 

    for(int i=0; i<*N_PARTICLES; i++){ // For all particles loop

        curr = NULL;
        enqueue(n); // Enqueue Root

    
        while (!queue_empty()) // While the queue has elements to be accessed
        {
            // printf("Body is %i\n", i);
            
            curr = begin->data; // Save at current node the begining of the queue ( FIFO )
            dequeue(); // Delete the node begin is pointing at so it moves to the next pointer
            
            d[0] = 0; d[1] = 0; m = 0;

            if(curr->b!=NULL){
                difference(&bodies[i].pos, &curr->b->pos, d); // Find vector component between i-th Body and the Pseudobody curr is pointing at
                mag(&m,d); // Magnitude of saiparticle_datad vector for Force calculation
                
                // printf("B [%f,%f] PB [%f,%f]\n", bodies[i].pos.x, bodies[i].pos.y, curr->b->pos.x, curr->b->pos.y);
                // printf("|d|:%f, [%f,%f]\n", m, d[0], d[1]);
                // printf("s/d = %f\n", (curr->s)/m);
                // printf("Node is: %d\n", curr->data);
                
            }

            if(curr->b!=NULL && (curr->s)/m <= theta){ // s/d=θ Barnes-Hut threshold! If its less or equal keep pseudobody force only for the ith particle
                // printf("True\n");

                k = (bodies[i].charge * curr->b->charge)/(m*m*m);
                k2 = (bodies[i].charge * curr->b->charge);

                // printf("k: %f\n", k); printf("d[0]: %f\n", d[0]);  printf("d[1]: %f\n", d[1]);

                Forces[i].x += k*d[0];
                Forces[i].y += k*d[1];

                

            } else{

                if(curr->b!=NULL){
                    
                    // printf("False\n");
                    if_leaf = 0;

                    if (curr->NE && curr->NE->b!=NULL){
                        enqueue(curr->NE);
                        if_leaf++;
                    }
                    if (curr->SE && curr->SE->b!=NULL){
                        enqueue(curr->SE);
                        if_leaf++;
                    }
                    if (curr->SW && curr->SW->b!=NULL){
                        enqueue(curr->SW);
                        if_leaf++;
                    }
                    if (curr->NW && curr->NW->b!=NULL){
                        enqueue(curr->NW);
                        if_leaf++;
                    }
                    // printf("Enqueue Children of %d \n",curr->data);
                    // printf("if_leaf: %i\n", if_leaf);
                    if(if_leaf==0 && m!=0){

                        // printf("Distance is: %f\n", m);

                        temporary_data_label = curr->data;

                        k = (bodies[i].charge * curr->b->charge)/(m*m*m);
                        k2 = (bodies[i].charge * curr->b->charge);
                        // printf("k: %f\n", k); printf("d[0]: %f\n", d[0]);  printf("d[1]: %f\n", d[1]);

                        Forces[i].x += k*d[0];
                        Forces[i].y += k*d[1];

                    }
                    
                }
            }

    
        }
        // printf("TF_[%i] = [%f,%f] \n", i,Forces[i].x, Forces[i].y);
    }
}