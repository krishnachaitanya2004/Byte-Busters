/**
 * bfs.cpp -- Simple program that does a breadth-first search over a binary-tree 
 * 
 * NOTE: Increase the N_NODES value for more computations.
 *       Higher values are preferred as you'll get to see the results in VTune more precisely.
 *       This will depend on what processor your system has, so tweak accordingly. As long as it
 *       takes kind of 10+ seconds, you're good to go.
 *
 */

#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

/* =============================================================== */
#define N_NODES  (1 << 20)      /* The number of nodes in the tree */
#define N_LOOPS  1000           /* Number of times to run BFS */
/* =============================================================== */


/* Class to represent a tree node */
class Node {
public:
    
    int val;
    Node *left;
    Node *right;
    
    Node(int v) {
        this->val = v;
        this->left = nullptr;
        this->right = nullptr;
    }

};


/* Create the tree */
void plant_a_tree(Node *root) {
    queue<Node *> node_Q;
    Node *curr_node;
    
    int q_size;
    int left_child_val;
    int right_child_val;
    
    node_Q.push(root);
    
    while(!node_Q.empty()) {
        q_size = node_Q.size();
        
        for(int i=0; i<q_size; i++) {
            curr_node = node_Q.front();
            node_Q.pop();

            left_child_val = ((curr_node->val) << 1) + 1;
            right_child_val = ((curr_node->val) << 1) + 2;
            
            curr_node->left = new Node(left_child_val);
            curr_node->right = new Node(right_child_val);

            if(left_child_val < N_NODES) node_Q.push(curr_node->left);
            if(right_child_val < N_NODES) node_Q.push(curr_node->right);
        }
    }   
}


/* Destroy the tree */
void bulldoze_the_tree(Node *root) {
    if(root == nullptr) return;
    
    bulldoze_the_tree(root->left);
    bulldoze_the_tree(root->right);
    free(root);
}


inline void bfs(Node *root) {
    queue<Node *> node_Q;
    Node *curr_node;
    Node *left_child, *right_child;
    int q_size;
    
    node_Q.push(root);
    
    while(!node_Q.empty()) {
        q_size = node_Q.size();
        
        for(int i=0; i<q_size; i++) {
            curr_node = node_Q.front();
            node_Q.pop();
            
            left_child = curr_node->left;
            right_child = curr_node->right;

            if(left_child) node_Q.push(left_child);
            if(right_child) node_Q.push(right_child);
        }
    }
    
    /* All done (~_~) */
}


int main() {
    Node *root = new Node(0);

    /* The tree is filled linearly, i.e. [0, 1, 2, ..., N_NODES-1] */
    /* For i'th node, 
     * Left child has a value of (2*i + 1)
     * Right child has a value (2*i + 2)
     */
    plant_a_tree(root);

    /* Do the BFS traversal */
    for(int i=0; i<N_LOOPS; i++)
        bfs(root);
        
    /* Clean up the mess using post-order traversal */
    bulldoze_the_tree(root);
    
}
