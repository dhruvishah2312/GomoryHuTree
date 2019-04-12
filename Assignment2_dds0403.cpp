// Derived from 
// http://www.aduni.org/courses/algorithms/handouts/Reciation_09.html#25630

// The Ford-Fulkerson Algorithm in C has been taken from Bob Weem's website
// For the buildGomoryHu tree function I've implemented the pseudocode.
// Reference also taken from https://github.com/abhishekm001/Gomory-Hu-Tree/blob/master/GomoryHuCutTreeAlgorithm



#include <stdio.h>


// Basic Definitions

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define MAX_NODES 3000
#define oo 1000000000


// Declarations

int n;  // number of nodes
int e;  // number of edges
int capacity[MAX_NODES][MAX_NODES]; // capacity matrix
int color[MAX_NODES]; // needed for breadth-first search               
int graph[MAX_NODES][MAX_NODES]; //final cut tree
int flowMatrix[MAX_NODES][MAX_NODES];     // flow matrix
int augmentingPathArray[MAX_NODES];  // array to store augmenting path
int flowMatrix1[MAX_NODES][MAX_NODES];


int min(int x, int y) {
    return x<y ? x : y;  // returns minimum of x and y
}

// A Queue for Breadth-First Search

int head, tail;
int q[MAX_NODES + 2];

void enqueue(int x) {
    q[tail] = x;
    tail++;
    color[x] = GRAY;
}

int dequeue() {
    int x = q[head];
    head++;
    color[x] = BLACK;
    return x;
}

// Breadth-First Search for an augmenting path

int bfs(int start, int target) {
    int u, v, i;
    for (u = 0; u<n; u++) {
        color[u] = WHITE;
    }
    head = tail = 0;
    enqueue(start);
    augmentingPathArray[start] = -1;
    while (head != tail) {
        u = dequeue();
        // Search all adjacent white nodes v. If the capacity
        // from u to v in the residual network is positive,
        // enqueue v.
        for (v = 0; v<n; v++) {
            if (color[v] == WHITE && capacity[u][v] - flowMatrix[u][v]>0) {
                enqueue(v);
                augmentingPathArray[v] = u;
            }
        }
    }

    // If the color of the target node is black now,
    // it means that we reached it.
    return color[target] == BLACK;
}

// Ford-Fulkerson Algorithm

int max_flow(int source, int sink) {
    int i, j, u;
    // Initialize empty flow.
    int max_flow = 0;
    for (i = 0; i<n; i++) {
        for (j = 0; j<n; j++) {
            flowMatrix[i][j] = 0;
        }
    }
    //if augmenting path exists, implement increments
    while (bfs(source, sink)) {
        // Determine the amount by which we can increment the flow.
        int increment = oo;
        for (u = sink; augmentingPathArray[u] != (-1); u = augmentingPathArray[u]) {
            increment = min(increment, capacity[augmentingPathArray[u]][u] - flowMatrix[augmentingPathArray[u]][u]);
        }
        // Now increment the flow.
        for (u = sink; augmentingPathArray[u] != (-1); u = augmentingPathArray[u]) {
            flowMatrix[augmentingPathArray[u]][u] += increment;
            flowMatrix[u][augmentingPathArray[u]] -= increment;  // Reverse in residual
        }
       
        max_flow += increment;
    }
    
    return max_flow;
}

// Reading the input file and the main program

void read_input_file() {
    int a, b, c, i, j;
    //FILE* input = fopen("mf.in", "r");
    // read number of nodes and edges
    scanf("%d %d", &n, &e);
    // initialize empty capacity matrix 
    for (i = 0; i<n; i++) {
        for (j = 0; j<n; j++) {
            capacity[i][j] = 0;
        }
    }
    // read edge capacities
    for (i = 0; i<e; i++) {
        scanf("%d %d %d", &a, &b, &c);
        capacity[a][b] = c;
        capacity[b][a] = c;// Could have parallel edges
    }

}

void BuildGomoryHuTree() {
    int i, j, s, p[MAX_NODES], t, f1[MAX_NODES], q[MAX_NODES], minCut;

    for (i = 0; i < n; i++) {
        p[i] = 0;
        f1[i] = 0;
        q[i] = 0;
        for (j = 0; j < n; j++) {
            flowMatrix1[i][j] = 0;
            graph[i][j] = 0;
        }
    }



    for (s = 1; s < n; s++) {

        t = p[s];

        minCut = max_flow(s, t);

        f1[s] = minCut;

        for (i = 0; i < n; i++) {
            q[i] = 0; //initialising array q to 0 
        }

        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                graph[i][j] = 0;
            }
        }

        for (i = 0; i < n; i++) {
            if (color[i] == BLACK) {
                q[i] = BLACK;  //checking for matches for coloring
            }
        }


        for (i = 0; i < n; i++) {
            if (i != s && p[i] == t && q[i] == BLACK) {

                p[i] = s;

            }
        }


        if (q[p[t]] == BLACK) {     //checking whether p[t] is in the nodes (colored black) then implementing the pseudo code.
           
            p[s] = p[t];
            p[t] = s;
            f1[s] = f1[t];
            f1[t] = minCut;
        }

       

        flowMatrix1[s][t] = flowMatrix1[t][s] = minCut;
        for (int i = 0; i < s; i++)
            if (i != t)
                flowMatrix1[s][i] = flowMatrix1[i][s] = min(minCut, flowMatrix1[t][i]);

        for (i = 1; i <= s; i++) {
            graph[i][p[i]] = f1[i];
        }

    }
    printf("\n");
    for (i = 0; i<n; i++){
        for (j = 0; j<n; j++){
            if(graph[i][j]>0){
                printf("%d %d %d\n", i, j, graph[i][j]);
            }    
        }
    }
}

int main() {

    read_input_file();
    BuildGomoryHuTree();
    return 0;
}
