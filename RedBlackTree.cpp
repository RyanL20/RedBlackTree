#include <iostream>
#include <cstring>
#include <fstream>

/* A Red-Black Tree that can take use input (console) or from a file, then add value to tree & assign color,
 * search numbers, print tree, remove numbers, and quit program. 
 * Author: Ryan Le
 * Date: 1/20/2021
 */

struct node {
  int value;
  node* left;
  node* right;
  node* parent;
  char color;
  node (int newValue) {
    value = newValue;
    left = NULL;
    right = NULL;
    parent = NULL;
    color = 'R';
  }
};

void add(node* &root, node* parent, int value);
void printTree(node* &root, int space);
node* search(node* &root, int value);
node* node_to_replace(node* current, int value);
int findLargest(node* &root);
void remove_fix(node* &root, int value);
//void RBTree_remove(node* &root, int value);
void fixRedBlackTree(node* current);
void leftRotation(node* current);
void rightRotation(node* current);
node* getRoot(node* current);

using namespace std;
int input;

int main() {
  node* root = NULL;
  node* parent = NULL;
  node* current = NULL;

  char action[80];
  bool exit = 0;

  cout << "Welcome to Red-Black Tree" << endl;
  while (exit == 0) {
    cout << "Executable Actions FILE, ADD, SEARCH, PRINT, REMOVE, QUIT" << endl;
    cin >> action;

    //FILE ADD
    //from Heap assignment
    if (strcmp(action, "FILE") == 0 || strcmp(action, "file") == 0) {
      ifstream indata;
      int num;
      indata.open("numberFile.txt");
      indata >> num;
      while ( !indata.eof()) {
	add(root, parent, num);
	indata >> num;
      }
      indata.close();
      cout << "Read file" << endl;
    }

    //ADD
    else if (strcmp(action, "ADD") == 0 || strcmp(action, "add") == 0) {
      cout << "How many numbers do you want to enter? ";
      int numAmount;
      cin >> numAmount;
      cout << "Enter numbers between 1-999." << endl;
      for (int i = 1; i < numAmount+1; i++) {
	cout << "[" << i << "] Enter number: ";
	cin >> input;
	add(root, parent, input);
	root = getRoot(root);
      }
    }

    //SEARCH
    else if (strcmp(action, "SEARCH") == 0 || strcmp(action, "search") == 0) {
      cout << "Search number: ";
      cin >> input;
      search(root, input);
    }

    //PRINT
    else if (strcmp(action, "PRINT") == 0 || strcmp(action, "print") == 0) {
      root = getRoot(root);
      printTree(root, 0);
    }

    //DELETE
    else if (strcmp(action, "REMOVE") == 0 || strcmp(action, "remove") == 0) {
      cout << "Remove number: ";
      cin >> input;
      search(root, input); //tells user if the wanted delete number is in tree
      remove_fix(root, input);
    }

    //QUIT
    else if (strcmp(action, "QUIT") == 0 || strcmp(action, "quit") == 0) {
      exit = 1;
    }

    //If typed something else, try again
    else {
      cout << "Please try again." << endl;
    }
  }
  
  //cout << "Largest number" << findLargest(root->left) << endl;
  return 0;
}

//ADD Function
void add(node* &current, node* parent, int value) {
  if (current == NULL) {
    current = new node(value);
    current->parent = parent;
    fixRedBlackTree(current);
  }
  else if (value <= current->value) {  // add to the left of parent.
    add(current->left, current, value);
  }
  else if (value > current->value) {  // add to the right of parent.
    add(current->right, current, value);
  }
}

//prints Binary tree horizontally.
//code from https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
void printTree(node* &root, int space) {
  //cout << "ROOOT " << root << endl;
  if(root == NULL){
    return;
  }
  space += 10;

  printTree(root->right, space);
  cout << endl;
  
  for(int i = 10; i < space; i++){
    cout << " ";
  }
  cout << root->value  << root->color << " (P:";
  if (root->parent != NULL) {cout << root->parent->value << ")\n";}
  else {cout << " )\n";}
  printTree(root->left, space);
}

//SEARCH function if number in the tree is existing.
node* search(node* &root, int value) {
  if (root != NULL) {
    if (root->value == value) {
      cout << "Value " << value << " found" << endl;
      return root;
    }
    else {
      if (value < root->value) {
	return search(root->left, value); //search left
      }
      else {
	return search(root->right, value); //search right
      }
    }
  }
  else {
    cout << "Value " << value << " not found" << endl;
    return NULL;
  }
  
}

//finds largest number
int findLargest(node* &root) {
  node* currentPtr = root;
  while(currentPtr != NULL && currentPtr->right != NULL) {
    currentPtr = currentPtr->right;
  }
  return currentPtr->value;
}

// Remove function
void remove_fix(node* &root, int value) {
  node* node_found = search(root, value);
  //node* node_found = NULL;
  node* replacement_node = node_to_replace(root, value);
  //node* replacement_node = NULL;
  cout << "replacement_node " << replacement_node->value << endl;
  
  node* x = NULL;
  int part1 = 0;
  node* sibling = NULL;
  if (node_found->parent->left == node_found) {
    sibling = node_found->parent->right;
  } else {
    sibling = node_found->parent->left;
  }
  
  //delete root node
  if (node_found->parent == NULL && node_found->left == NULL && node_found->right == NULL) {
    cout << "Deleted root node" << endl;
    root = NULL;
    delete node_found;
  }
  //Part 1
  //if deleted node has 2 NULL children, replacement x is NULL
  else if (node_found->left == NULL && node_found->right == NULL) {
    cout << "Part 1a" << endl;
    x = NULL;
    //part1 = 1;
  }

  //if deleted node has 1 NULL child & 1 non-NULL child, replacement x is non-NULL child  
  else if (node_found->left != NULL && node_found->right == NULL) {
    cout << "part 1b" << endl;
    x = replacement_node->left;
    //part1 = 2;
  }
  else if (node_found->left == NULL && node_found->right != NULL) {
    cout << "part 1c" << "x" << x << endl;
    x = replacement_node->right;
    //part1 = 2;
  }

  //if deleted node has 2 non-NULL children, set x to replacement's right child
  else if (node_found->left != NULL && node_found->right != NULL) {
    cout << "part 1d" << endl;
    x = replacement_node->right;
    //part1 = 3;
  }

  cout << "begin part 2" << endl;
  //Part 2
  //if deleted node is red & replacement is NULL, done
  if (node_found->color == 'R' && replacement_node == NULL) {
    cout << "part 2" << endl;
    if (node_found->parent->right == node_found) {
      cout << "part2 if" << endl;
      node_found->parent->right = replacement_node;
    }
    else {
      cout << "part 2 else" << endl;
      node_found->parent->left = replacement_node;
    }
    return;
  }
  //deleted node is red, replacement is red, done
  else if (node_found->color == 'R' && replacement_node->color == 'R') {
    if (node_found->parent->right == node_found) {
      cout << "part2 if 2" << endl;
      node_found->value = replacement_node->value;
      replacement_node->parent->right = NULL;
      delete replacement_node;
    }
    else {
      cout << "part 2 else 2" << endl;
      node_found->value = replacement_node->value;
      replacement_node->parent->left = NULL;
      delete replacement_node;
    }
    return;
  }
  
  //if deleted node is red & replacement is black, color replacement red. proceed to case
  else if (node_found->color == 'R' && replacement_node->color == 'B') {
    node_found->value = replacement_node->value;
    cout << "Part 2 case 2" << endl;
    if (replacement_node->parent->right == replacement_node) {
      if (replacement_node->left == NULL) {
        replacement_node->parent->right = NULL;
      }
      else {//if have left child attached to replacement
        replacement_node->left->value = replacement_node->value;
        replacement_node->left = NULL;
      }
    }
    else {
      if (replacement_node->left == NULL) {
        replacement_node->parent->left = NULL;
      }
      else { //left child attached to replacement
        replacement_node->left->value = replacement_node->value;
        replacement_node->left = NULL;
      }
    }
  }

  //if deleted node is black & replacement is red, color replacement black, done
  else if (node_found->color == 'B' && replacement_node->color == 'R') {
    cout << "part 2 case 3" << endl;
    node_found->value = replacement_node->value;
    if (replacement_node->parent->right == replacement_node) {
      if (replacement_node->left == NULL) {
	replacement_node->parent->right = NULL;
      }
      else {//if have left child attached to replacement
	replacement_node->left->value = replacement_node->value;
	replacement_node->left = NULL;
      }
    }
    else {
      if (replacement_node->left == NULL) {
	replacement_node->parent->left = NULL;
      }
      else { //left child attached to replacement
	replacement_node->left->value = replacement_node->value;
	replacement_node->left = NULL;
      }
    }
    return;
  }
  
  //if deleted node is black & replacement is NULL or black, proceed to case
  else if (node_found->color == 'B' && replacement_node == NULL) {
    node_found->value = replacement_node->value;
    if (replacement_node->parent->right == replacement_node) {
      if (replacement_node->left == NULL) {
        replacement_node->parent->right = NULL;
      }
      else {//if have left child attached to replacement
        replacement_node->left->value = replacement_node->value;
        replacement_node->left = NULL;
      }
    }
    else {
      if (replacement_node->left == NULL) {
        replacement_node->parent->left = NULL;
      }
      else { //left child attached to replacement
        replacement_node->left->value = replacement_node->value;
        replacement_node->left = NULL;
      }
    }
  }
  else if (node_found->color == 'B' && replacement_node->color == 'B') {
    cout << "Node B, replace B" << endl;
    node_found->value = replacement_node->value;
    if (replacement_node->parent->right == replacement_node) {
      if (replacement_node->left == NULL) {
	replacement_node->parent->right = NULL;
      }
      else {//if have left child attached to replacement
        replacement_node->left->value = replacement_node->value;
        replacement_node->left = NULL;
      }
    }
    else {
      if (replacement_node->left == NULL) {
	replacement_node->parent->left = NULL;
      }
      else { //left child attached to replacement
	replacement_node->left->value = replacement_node->value;
        replacement_node->left = NULL;
      }
    } 
  }
    
  //Proceed Cases
  
  //Case 2 - Sibling is red
  
  cout << "sibling" << sibling << endl;
  cout << "sibling->parent" << sibling->parent << endl;
  if (sibling->parent->right == sibling) {
    cout << "case 2" << endl;
    if (sibling->color == 'R') {
      sibling->color = 'B';
      sibling->parent->color = 'R';
      leftRotation(sibling->parent);
      cout << "siblingparent" << sibling->parent << endl;
      sibling = sibling->left->right;
      cout << "new sibling" << sibling << endl;
      
    }
  }
  else {
    if (sibling->color == 'R') {
      cout << "Case 2 else " << endl;
      sibling->color = 'B';
      sibling->parent->color = 'R';
      rightRotation(sibling->parent);
      sibling = sibling->right->left;
    }
  }
  if (sibling->left != NULL && sibling->right != NULL) {
    //case 3
    cout << "Before case 3" << endl;
    if (sibling->color == 'B' && sibling->parent->color == 'B') {
      cout << "Case 3" << endl;
      sibling->color = 'R';
    }
  
    //case 4
    if (sibling->parent->color == 'R' && sibling->color == 'B' &&
	sibling->left->color == 'B' && sibling->right->color == 'B') {
      cout << "case 4" << endl;
      sibling->parent->color = 'B';
      sibling->color = 'R';
    }
    
    //Case 5
    if (sibling->parent->left == sibling && sibling->color == 'B' &&
	sibling->left->color == 'B' && sibling->right->color == 'R') {
      cout << "case 5a" << endl;
      rightRotation(sibling);
      sibling->color = 'R';
      sibling->left->color = 'B';
    }
    else if (sibling->parent->right == sibling && sibling->color == 'B' &&
	     sibling->right->color == 'B' && sibling->left->color == 'R') {
      cout << "case 5b" << endl;
      leftRotation(sibling);
      sibling->color = 'R';
      sibling->right->color = 'B';
    }
  }


     

//cout << "sibling" << sibling << endl;
// cout << "sibling-R" << sibling->right << endl;
// cout << "sibling-L" << sibling->left << endl;
// cout << "sibling->parent" << sibling->parent << endl;
// cout << "node_found->parent" << node_found->parent << endl;
// cout << "node_found" << node_found << endl;
}

     



//finds the replacement node
node* node_to_replace(node* current, int value) {
  if (current->value == value) {
    
    //Case 0 - 0 children
    if (current->left == NULL && current->right == NULL) {
      cout << "NOde CAse " << current->value << endl;
      return current;
    }
    //Case 1 - 1 child 
    else if (current->left != NULL && current->right == NULL) {
      cout << "111111" << endl;
      return (current->left);
    }
    else if (current->left == NULL && current->right != NULL) {
      cout << "1aaaaaaa" << endl;
      return (current->right);
    }
    //Case 2 - 2 children
    else {
      int largestInLeftSubTree = findLargest(current->left);
      cout << "else " << largestInLeftSubTree << endl;
      
      current->value = largestInLeftSubTree;
      return node_to_replace(current->left, largestInLeftSubTree);
      //cout << "bread" << endl;
    }
  }
  else {
    if (value < current->value && current->left != NULL) {
      cout << "Else selse " << endl;
      return node_to_replace(current->left, value);
    }
    else if (value > current->value && current->right != NULL) {
      cout << "nononon " << current->right << endl;
      return node_to_replace(current->right, value);
    }
  }
  return NULL;
}


void fixRedBlackTree(node* current) {

  node* uncle = NULL;
  //cout << "ENTERED FIXED" << endl;
  //Case 1 Root with no children
  if (current->parent == NULL) {
    current->color = 'B'; 
    //cout << "Case 1" << endl;
    return;
  }
  else {
    //gets uncle
    if (current->parent->parent != NULL) {
      if (current->parent == current->parent->parent->left) {
	uncle = current->parent->parent->right;
        //cout << "Right Uncle: " << uncle << endl;
      }
      else if (current->parent == current->parent->parent->right) {
	uncle = current->parent->parent->left;
	//cout << "Left Uncle: " << uncle << endl;
      }
    }
  }
  
  //Case 2 Parent is black
  if (current->parent->color == 'B') {
    //cout << "Case 2" << endl;
    return;
  }

  //Case 3 Parent and Uncle are red
  //if the parent = grandparent left, uncle is on right (comparing address)
  // makes sure parent and uncle exists
  //  else if (current->parent->parent->right != NULL && current->parent->parent->left != NULL) {

  else if (uncle != NULL) {
    //cout << "uncle is not null" << endl;

    if (current->parent->color == 'R' && uncle->color == 'R') {
      //cout << "Case 3a: parent & uncle are RED" << endl;
      current->parent->color = 'B';
      uncle->color = 'B';
      current->parent->parent->color = 'R';
      fixRedBlackTree(current->parent->parent);
    } 
        
    //Case 4 & 5 Uncle is black,
    else if (uncle->color == 'B') {
     
      if (current == current->parent->right) {
	//    12
	// 9
	//    10
	if (current->parent == current->parent->parent->left) {
	  //cout << "Case 4a: Parent L, node R - rotate left thru parent" << endl;
	  leftRotation(current->parent);
	  fixRedBlackTree(current->left);
	}
	// 9   
	//  10
	//    12
	else if (current->parent == current->parent->parent->right) {
	  //cout << "Case 5a: Grandparent, parent, & node on right - rotate left thru grandP " << endl;
	  current->parent->color = 'B';
	  current->parent->parent->color = 'R';
	  leftRotation(current->parent->parent);
	}
      }

      else if (current == current->parent->left) {
	// 12
	//    15
	// 13
	if (current->parent == current->parent->parent->right) { 
	  //cout << "Case 4b: Parent R, node L - rotate right thru parent" << endl;
	  rightRotation(current->parent);
	  fixRedBlackTree(current->right);
	}
	//    15
	//  10
	// 5
	else if (current->parent == current->parent->parent->left) {
	  //cout << "Case 5b: GP, P, & N on Left - rotate right thru grandP " << endl;
	  current->parent->color = 'B';
	  current->parent->parent->color = 'R';
	  rightRotation(current->parent->parent);
	}
      }
 	
    }
  }
  
  //if uncle to the right is empty
  else if (uncle == NULL) {

    //Case 4 & 5 uncle is black (NULL)
    if (current == current->parent->right) {
      //Case 4c
      if (current->parent == current->parent->parent->left) {
	//cout << "Case 4c: " << endl;
	leftRotation(current->parent);
	fixRedBlackTree(current->left);
      }
      //Case 5c
      else if (current->parent == current->parent->parent->right) {
        //cout << "Case 5c: P and N are right - rotate left thru grandP " << endl;
        current->parent->color = 'B';
        current->parent->parent->color = 'R';
	leftRotation(current->parent->parent);
      } 
    }   
 
    else if (current == current->parent->left) {
      if (current->parent == current->parent->parent->right) {
	//cout << "Case 4d: " << current->parent << endl;
	rightRotation(current->parent);
	fixRedBlackTree(current->right);
      }
      // case 5 R: parent and node are left and rotate right through grandP
      else if (current->parent == current->parent->parent->left) {
	//cout << "Case 5d: P and N are left - rotate grandp" << endl;
	current->parent->color = 'B';
	current->parent->parent->color = 'R';
	rightRotation(current->parent->parent);
      }
    }  
  }

    else {
      cout << "No Case:" << endl;
    }
}


void leftRotation(node* current) {
  //cout << "Left Rotation" << endl;
  node* temp = current->right;
    
  current->right = temp->left;
  if (temp->left != NULL) {
    temp->left->parent = current;    
  }

  temp->parent = current->parent;
  if(current->parent == NULL) {
    //cout << "FPARENT" << endl;
  }
  else if (current == current->parent->left) {
    current->parent->left = temp;
  }
  else {
    current->parent->right = temp;
  }
  temp->left = current;
  current->parent = temp;
}

void rightRotation(node* current) {
  //cout << "Right Rotation" << endl;
  node* temp = current->left;
  //cout << " temp value " << temp->value << endl;

  current->left = temp->right;
  if (temp->right != NULL) {
    temp->right->parent = current;
  }
  temp->parent = current->parent;
  if (current->parent == NULL) {
    temp->right = current; // connect "current" node to right of its left node
    current->parent = temp; // assign "current" node parent to it left node.
    return;
  }
  else if (current == current->parent->left) {
    current->parent->left = temp;
  }
  else {
    current->parent->right = temp;
  }
  temp->right = current;
  current->parent = temp;
}

node* getRoot(node* currentNode) {
  node* current = currentNode;
  while(current->parent != NULL) {
    current = current->parent;
  }
  return current;
}
