//Barnes-Hut Algorithm -- Implementation in 2D

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*
    Point structure, having for 2D 2 components
*/
struct point 
{
    double x;
    double y;
};

/*
    Body structure, having coordinates in 2D, mass and charge.
*/
struct body 
{
    struct point pos; // Position
    double mass;   // Mass
    double charge;  // Charge
};

/*
    Quad tree node structure
*/
struct quad  
{ 
    int data; // key value
    double s; // Original size corresponding to this quad, and for s/d calculation soon
    struct point centre; // Center of this node
    bool divided; // Check if it has divided

    struct body *b; // Pointer to body structure

    // Pointers to children of this node taken with respect to compass directions.
    struct quad *NW; 
    struct quad *NE;
    struct quad *SE;
    struct quad *SW; 
}; 
  
/* 
    Allocates a new node and respectively fills data, coordinates and size of square for that node.
*/
struct quad* newNode(int data, double s, double x, double y) 
{ 
    // Allocate memory for new quad  
    struct quad* quad = (struct quad*)malloc(sizeof(struct quad)); 

    // If out of memory abort:
    if(quad == NULL)
    {
        fprintf (stderr, "Out of memory!!! (create_node)\n");
        exit(1);
    }
    // Assign data to this quad, s size of square, centre point of it
    // b as NULL for not storing a pointer to a body yet, false for divided as it has not subdivided yet.
    quad->data = data;
    quad->s = s; 
    quad->centre.x = x; quad->centre.y = y;
    quad->b = NULL;
    quad->divided = false;

    // Initialize all children as NULL 
    quad->NE = NULL; 
    quad->SE = NULL;
    quad->SW = NULL;
    quad->NW = NULL;

    return(quad); 
} 

	
/*
    Recursively display tree or subtree
*/
void display_tree(struct quad* nd)
{
    if (nd == NULL)
        return;
    // Display the data of the node
    printf(" %*c(%d) \n %*c[%f, %f] \n %*c[%f]\n\n",50, ' ', nd->data, 42,' ', nd->centre.x, nd->centre.y, 47, ' ', nd->s);
    
    // Display data for each node plus some formating it looks better
    if(nd->NE != NULL)
        printf("%*c|NE:%d|  ",34,' ',nd->NE->data);
    if(nd->SE != NULL)
        printf("|SE:%d|  ",nd->SE->data);
    if(nd->SW != NULL)
        printf("|SW:%d|  ",nd->SW->data);
    if(nd->NW != NULL)
        printf("|NW:%d|",nd->NW->data);
    printf("\n\n");
    
    // Recurse through
    display_tree(nd->NE);
    display_tree(nd->SE);
    display_tree(nd->SW);
    display_tree(nd->NW);
}

/*
    Deconstruct quad tree (Postorder)
*/ 
void deconstruct_tree(struct quad* root)
{
    if(root != NULL)
    {
        deconstruct_tree(root->NE);
        deconstruct_tree(root->SE);
        deconstruct_tree(root->SW);  
        deconstruct_tree(root->NW);
        if(root->data<0){
            free(root->b);
        }
        free(root);
    }
}

/*
    Subdivide node to 4 quadrants and assign memory dynamically through newNode() function
*/
void subdivide(struct quad* nd, int* track){ 
    
    if(nd == NULL){ // If there is no node do not subdive. (safety measure)
        return;
    }

    // Call newNode function for each child node that was Null of the node at hand and assign a memory block of size (struct quad)
    // -n is assigned here if the node is a 'twig' meaning it is not a 'leaf' for now empty cells are also -n where n integers that
    // that are unique for each quad.
    //    _________________________
    //   |            |            |
    //   |    (NW)    |    (NE)    |
    //   |            |            |
    //   |     -+     |     ++     |
    //   |____________|____________|
    //   |            |            |
    //   |     --     |     +-     |
    //   |            |            |
    //   |    (SW)    |    (SE)    |
    //   |____________|____________|

    nd->NE = newNode(*track-1, nd->s/2, nd->centre.x + nd->s/4, nd->centre.y + nd->s/4); 
    nd->SE = newNode(*track-2, nd->s/2, nd->centre.x + nd->s/4, nd->centre.y - nd->s/4); 
    nd->SW = newNode(*track-3, nd->s/2, nd->centre.x - nd->s/4, nd->centre.y - nd->s/4); 
    nd->NW = newNode(*track-4, nd->s/2, nd->centre.x - nd->s/4, nd->centre.y + nd->s/4); 
    
      
    nd->divided = true; // The node subdivided ( safety for not subdividing again the same node )
    *track = *track-4;
    // printf("Track is: %i\n", *track);

}


/*
    Check if the body is residing inside the node through its coordinates
*/
bool contains(struct quad* nd, struct point p){
        return (p.x < (nd->centre.x+nd->s/2) &&
        p.x > (nd->centre.x-nd->s/2) &&
        p.y < (nd->centre.y+nd->s/2) &&
        p.y > (nd->centre.y-nd->s/2));
        
}


/*
    Construct quad tree from bottom-up by putting bodies inside it.
*/
int insert(struct quad* nd, struct body* b, int *index, int* track){

    // If current quad cannot contain it 
    if (!contains(nd,b->pos)){ 
        return 0; // Not found yet so return 0 and go in the function again
    } 

    if(nd->b==NULL){ // If there is no pointer to body assign it (Essentially capacity is kept at 1 here with this method)
        nd->b = b;
        nd->data = *index; //Assign the number of the body from the Bodies array, this is for getting back with data where the body is stored as a leaf
        // printf("Pointer to %i\n", nd->data);
        return 0; // Found so return 0 so we can exit the recursion
    } 
    else{
    
        if(nd->divided!=true){ // Check if the quad quad has subdivided
            subdivide(nd, track); // If not, subdivide!
            // Check where to put nodes body and then after the if statement check for the current index body
            int temp = nd->data;
            nd->data = *track+4;
            *track = *track-1;
            insert(nd->NE, nd->b, &temp, track);  
            insert(nd->SE, nd->b, &temp, track);  
            insert(nd->SW, nd->b, &temp, track); 
            insert(nd->NW, nd->b, &temp, track);
        }
    }

    return insert(nd->NE, b, index, track)|| // Since insert is an int function, the return statement here returns 0 if 
    insert(nd->SE, b, index, track)||  // a node is found to point the body and thus save the body at. So for example if we
    insert(nd->SW, b, index, track)|| // have 2 bodies and the first one is always saved at root ( being empthy and not divided)
    insert(nd->NW, b, index, track); // then the second Body 1 if it is at NW subcell after division when it goes to the return state-
        // -ment it will go through the OR terms as the contain function will not let the first 3 OR terms ( being insert(nd->NE,...) || ... 
        // to insert(nd->SW,...) and pick the insert(nd->NW,...) to assign. The same process is repeated every time the function calls itself
        // and checks for where to put the body.
}

/*
    Queue datastructure
*/
struct linkedList
{
  struct quad* data;
  struct linkedList* next;
  
}; 
 
struct linkedList* begin;
struct linkedList* end;

/*
    Push element inside the end part of the queue by allocating new memory dynamically
*/ 
void enqueue(struct quad* nd)
{
    struct linkedList* temp = malloc(sizeof(struct linkedList));
    temp->data = nd;
    temp->next = NULL;
    if(begin==NULL && end==NULL){
        begin = end = temp;
    }
    end->next = temp;
    end = temp;
}
 /*
    Pop element from queue and free its memory
 */
void dequeue()
{
    struct linkedList* temp = begin;
    if(begin==NULL){return;}
    if(begin==end){
        begin = NULL;
        end = NULL;
    }
    else{
        begin = begin->next;
    }
    free(temp);
}
 
bool queue_empty()
{
  return (begin==NULL && end==NULL);
}
/*
    Level Order Traversal for force summation ( Breadth first traversal)
*/
void print_level_order(struct quad* n)
{
    enqueue(n);
    enqueue(NULL); // Extra NUll parameter for checking when the tree goes to the next level after enquing all children in one level.
    struct quad* curr = NULL;
    while (!queue_empty())
    {
    
        curr = begin->data;
        dequeue();
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
            printf("\n");
            if(!queue_empty()){
                enqueue(NULL);
            }
        }
  
    }
}

/*
    Assign new Pseudobody to node of quad tree
*/
void newBody(struct quad* nd, struct point pos, double mass, double charge)
{
    struct body* b = malloc(sizeof(struct body)); //= malloc(sizeof(struct body));
    b->pos.x = pos.x;
    b->pos.y = pos.y;
    b->mass = mass;
    b->charge = charge;
    nd->b = b;
    // free(b);
};

/*
    Create Pseudobodies quad tree (Postorder)
*/ 
void sum(struct quad* nd)
{
    if(nd != NULL){

        sum(nd->NE);
        sum(nd->SE);
        sum(nd->SW);
        sum(nd->NW);

        double centre_x = 0; // x component of pseudobody
        double centre_y = 0; // y component of pseudobody
        double centre_mass = 0; // Mass of Pseudobody
        double total_charge = 0; // extra term since we have charges!!

        // printf("Sum recursing through: %i\n", nd->data);

        if(nd->NE!=NULL && nd->NE->b != NULL){

            centre_mass += nd->NE->b->mass;
            centre_x += ((nd->NE->b->mass)*(nd->NE->b->pos.x));
            centre_y += ((nd->NE->b->mass)*(nd->NE->b->pos.y));
            total_charge += nd->NE->b->charge;
        
        }
        if(nd->SE!=NULL && nd->SE->b!= NULL){
        
            centre_mass += nd->SE->b->mass;
            centre_x += ((nd->SE->b->mass)*(nd->SE->b->pos.x));
            centre_y += ((nd->SE->b->mass)*(nd->SE->b->pos.y));
            total_charge += nd->SE->b->charge;
        
        }
        if(nd->SW!=NULL && nd->SW->b != NULL){
        
            centre_mass += nd->SW->b->mass;
            centre_x += ((nd->SW->b->mass)*(nd->SW->b->pos.x));
            centre_y += ((nd->SW->b->mass)*(nd->SW->b->pos.y));
            total_charge += nd->SW->b->charge;
        
        }
        if(nd->NW!=NULL && nd->NW->b != NULL){
        
            centre_mass += nd->NW->b->mass;
            centre_x += ((nd->NW->b->mass)*(nd->NW->b->pos.x));
            centre_y += ((nd->NW->b->mass)*(nd->NW->b->pos.y));
            total_charge += nd->NW->b->charge;
        
        }
        if(centre_mass != 0){
        
            // printf("Centre mass: %f Body Mass %f\n", centre_mass, nd->b->mass);
            centre_x = centre_x/ centre_mass; centre_y = centre_y/ centre_mass;
            struct point p = {.x = centre_x, .y= centre_y};
            // printf("PMass: %f, PCharge: %f, Pxy: [%f, %f]\n", centre_mass, total_charge, centre_x, centre_y);
            newBody(nd, p, centre_mass,  total_charge); //Assign pseudobody
        
        } 
        
    }
} 

/*
    Search quad tree for node with specific data in inorder format
*/
struct quad* Search(struct quad* root, int data) {
	// base condition for recursion
	// if tree/sub-tree is empty, return and exit
	if(root == NULL){return NULL;}

	// printf("%i \n",root->data); // Print data
    if(root->data == data){return root;}
	
    Search(root->NW, data);     // Visit NW subtree
	Search(root->SW, data);    // Visit SW subtree
    Search(root->SE, data);   // Visit SE subtree
    Search(root->NE, data);  // Visit NE subtree
}

/*
    Get Vector difference from points
*/
void difference(struct point* p1, struct point* p2, double* d){
    // printf("d = [%f,%f]\n", d[0], d[1]);
    d[0] = p2->x - p1->x;
    d[1] = p2->y - p1->y;
    // printf("d = [%f,%f]\n", d[0], d[1]);
    // printf("P2 [%f,%f], P1 [%f,%f]\n", p2->x, p2->y, p1->x, p1->y); 
}

/*
    Get the magnitude of the 2D vector
*/
void mag(double* m, double* d){
    *m = sqrt(d[0]*d[0]+d[1]*d[1]);
}

// Faster lever order 
void ordertraversal(struct quad* root){
    struct quad* curr = NULL;
    enqueue(root);
    
    while(!queue_empty()){
        curr = begin->data;
        dequeue();

        printf("%i\n", curr->data);
        
        if(curr->NE){ enqueue(curr->NE);}
        if(curr->SE){ enqueue(curr->SE);}
        if(curr->SW){ enqueue(curr->SW);}
        if(curr->NW){ enqueue(curr->NW);}
    }
}

/*
    Level Order Traversal for force summation ( Breadth first traversal)
*/
void levelorder_force(struct quad* n, struct body* bodies, struct point *Forces, int* N_PARTICLES, double theta){

    struct quad* curr; // Current node tracker
    double m = 0; // Magnitude component
    double d[2] = {0,0}; // Vector component for force calculation
    int if_leaf = 0; //Check if leaf node if it has 0 children
    int temporary_data_label = 0;
    double k = 0; 

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
                        // printf("k: %f\n", k); printf("d[0]: %f\n", d[0]);  printf("d[1]: %f\n", d[1]);

                        Forces[i].x += k*d[0];
                        Forces[i].y += k*d[1];

                        // Forces[temporary_data_label].x -= k*d[0];
                        // Forces[temporary_data_label].y -= k*d[1];
                    }
                    
                }
            }

    
        }
        // printf("TF_[%i] = [%f,%f] \n", i,Forces[i].x, Forces[i].y);
    }
}


/*
    Function to save X, Y coordinates of bodies and Time taken to run build the tree
*/
void particle_data(struct body* bodies, struct point* Forces, int* N_PARTICLES, double t, double t2, double theta){
    FILE * f; 
    f = fopen("/home/albes/Desktop/bodiesbu.txt", "w"); /* open the file for writing*/
    printf("Writting...\n");
    /* write 10 lines of text into the file stream*/    
    fprintf(f, "N,X,Y,M,C,Fx,Fy,TBuilt,TForce\n");

    for(int i = 0; i < *N_PARTICLES;i++){
        fprintf (f, "%d,%f,%f,%f,%f,%f,%f\n", i, bodies[i].pos.x, bodies[i].pos.y, bodies[i].mass, bodies[i].charge, Forces[i].x, Forces[i].y);
    }
    fprintf(f,",,,,,,,%f,%f,%f",t, t2, theta);

    /* close the file*/  
    fclose (f);
    printf("Closed file.\n");
}

/*
    Helper function for xy_tree.c to print data to csv file so it can be processed later
*/
void printdata(struct quad* nd, FILE* f){
    if (nd == NULL){return;}
    // Display the data of the node
    fprintf(f,"%d,%f,%f,%f\n",nd->data, nd->centre.x, nd->centre.y, nd->s);
    
    // Recurse through
    printdata(nd->NE,f);    
    printdata(nd->SE,f);
    printdata(nd->SW,f);
    printdata(nd->NW,f);
}

/*
    Function to print quad tree data into csv files
*/
void xy_trees(struct quad* nd){
    FILE * f; 
    f = fopen("/home/albes/Desktop/nodesbu.txt", "w"); /* open the file for writing*/
    printf("Writting...\n");
    /* write 10 lines of text into the file stream*/    
    fprintf(f, "N,X,Y,S\n");
    printdata(nd, f);
    /* close the file*/  
    fclose (f);
    printf("Closed file.\n");
}

/*
    Helper Function to save the Pseudobodies data for pseudo_particles
*/
void pseudo_data(struct quad* nd, FILE* f){
    // Recurse through
    if(nd!=NULL){
        
        
        pseudo_data(nd->NE,f);
        pseudo_data(nd->SE,f);
        pseudo_data(nd->SW,f);
        pseudo_data(nd->NW,f);
        
        double id = 0;

        if(nd->NE !=NULL && nd->NE->b != NULL)
            id += nd->NE->b->mass;
        if(nd->SE !=NULL && nd->SE->b != NULL)
            id += nd->SE->b->mass;
        if(nd->SW !=NULL && nd->SW->b != NULL)
            id += nd->SW->b->mass;
        if(nd->NW !=NULL && nd->NW->b != NULL)
            id += nd->NW->b->mass;
        if(id!=0){
            fprintf(f,"%d,%f,%f,%f,%f,%f\n",nd->data, nd->b->pos.x, nd->b->pos.y, nd->b->mass, nd->b->charge, id);
        }
    }

}

/*
    Save the data for the Pseudobodies
*/
void pseudo_particles(struct quad* nd){
    FILE * f; 
    f = fopen("/home/albes/Desktop/pseudoBodiesbu.txt", "w"); /* open the file for writing*/
    printf("Writting...\n");
    /* write 10 lines of text into the file stream*/    
    fprintf(f, "N,PX,PY, PMass, PCharge, PMassSize\n");
    pseudo_data(nd, f);
    /* close the file*/  
    fclose (f);
    printf("Closed file.\n");
}
   
/*
    Main function to run the program
*/
int main() {

    int N_DIMENSIONS = 2; int seed=1;
    int N_PARTICLES;
    char term;
    clock_t ts, te;
    double theta = 0.5;

    printf("How many particles?\n");
    if (scanf("%d%c", &N_PARTICLES, &term) != 2 || term != '\n') {
        printf("Failure: Not an integer. Try again\n");
        exit(-1);
    } 

    printf("Calculating\n");
    if (N_PARTICLES < 2) {
        printf("Insufficent number of particles %i\n", N_PARTICLES);
        exit(-1);
    } 

    struct body* bodies = malloc(sizeof(struct body)*N_PARTICLES);
    struct point* Forces = malloc(sizeof(struct point)*N_PARTICLES);

    //Initialise values:
    char x[2]={'x', 'y'};
    srand(seed);

    for (int i=0; i < N_PARTICLES; i++){

            double mass = 5 * ((double) rand() / (double) RAND_MAX ); // 1,5
            double charge = 10 * ((double) rand() / (double) RAND_MAX ) - 5; // -5, 5
            struct point p = {.x = 100 * ((double) rand() / (double) RAND_MAX ) - 50, // -50, 50
            .y = 100 * ((double) rand() / (double) RAND_MAX ) - 50};

            struct body b = {.mass = mass, .charge = charge, .pos = p };
            Forces[i].x = 0; Forces[i].y = 0;
            bodies[i] = b;
            // printf("%c:[%f], %c:[%f] \n", x[0], bodies[i].pos.x, x[1], bodies[i].pos.y );
            // printf("Mass: %f, Charge: %f\n", mass, charge);
    
    }

    struct quad *root = newNode(0, 100, 0, 0); //Size of s=100 and pint of reference being (0,0) equiv. to (x_root, y_root)  
    //printf("Root square size is: %f\n", root->s);

    int track = 0;

    ts = clock(); // Start timer
    for(int i=0; i<N_PARTICLES; i++){
        insert(root, &bodies[i], &i, &track);
    }
    te = clock(); // End timer
    double d = (double)(te-ts)/CLOCKS_PER_SEC; // Bottom-up tree construction time
    
    // display_tree(root);
    ts = clock();
    // print_level_order(root);
    // printf("Faster method\n");
    // ordertraversal(root);
    te = clock();
    double d2 = (double)(te-ts)/CLOCKS_PER_SEC; // Level Order construction
    
    ts = clock();
    sum(root);
    te = clock();
    double d3 = (double)(te-ts)/CLOCKS_PER_SEC;
    // printf("Going up the tree took: %f\n", d2);
    
    ts = clock();
    levelorder_force(root, bodies, Forces, &N_PARTICLES, theta);
    te = clock();
    double d4 = (double)(te-ts)/CLOCKS_PER_SEC;

    // deconstruct the tree
    char c;
    printf("Do you want to save the data? Y/n \n");
    scanf("%c", &c);
    
    if(c=='Y'){
        printf("Saving bodies data...\n");
        particle_data(bodies, Forces, &N_PARTICLES, d, d4, theta);
        printf("Saving tree data...\n");
        xy_trees(root);
        printf("Saving Pseudobodies data...\n");
        pseudo_particles(root);
        printf("Done\n");
    } else
    {
        printf("Continuing\n");   
    }

    free(bodies);
    free (Forces);
    free(root->b); // Since root with min 2 Bodies in it will always have a pseudobody in it, all others are at negative (<0) integers.
    deconstruct_tree(root);

    printf("Released memory succesfuly\n");
    printf("Program took %f\n", d);
    printf("Sum took %f\n", d3);
    printf("Levelorder took %f\n", d2);
    printf("Force calculation took: %f\n", d4);

    return 0; 
}