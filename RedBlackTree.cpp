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
int findLargest(node* &root);
void remove(node* &root, int value);
void fixRedBlackTree(node* &current);
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
    if (strcmp(action, "FILE") == 0) {
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
	cout << "current " << current << endl;
	add(root, parent, input);
	cout << "root : " << root << endl;
	root = getRoot(root);
      }
    }

    //SEARCH
    else if (strcmp(action, "SEARCH") == 0) {
      cout << "Search number: ";
      cin >> input;
      search(root, input);
    }

    //PRINT
    else if (strcmp(action, "PRINT") == 0 || strcmp(action, "print") == 0) {
      //cout << "Print Min to Max (MIN), Max to Min (MAX), or tree (TREE) ?" << endl;
      //cin >> action;
      printTree(root, 0);
    }

    //DELETE
    else if (strcmp(action, "REMOVE") == 0) {
      cout << "Remove number: ";
      cin >> input;
      search(root, input); //tells user if the wanted delete number is in tree
      remove(root, input);
    }

    //QUIT
    else if (strcmp(action, "QUIT") == 0) {
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
    cout << "CURRENT ADD " << current << endl;
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
  if(root == NULL){
    return;
  }
  space += 10;

  printTree(root->right, space);
  cout << endl;
  
  for(int i = 10; i < space; i++){
    cout << " ";
  }
  cout << root->value  << root->color << " " << root << " P " << root->parent << "\n";
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

//Remove function
void remove(node* &root, int value) {
  if (root->value == value) {
    
    //Case 0 - 0 children
    if (root->left == NULL && root->right == NULL) {
      root = NULL;
      //  cout << "meatballs" << endl;
    }
    //Case 1 - 1 child 
    else if (root->left != NULL && root->right == NULL) {
      root = root->left;
      //cout << "pasta" << endl;
    }
    else if (root->left == NULL && root->right != NULL) {
      root = root->right;
      //cout << "eggrolls" << endl;
    }
    //Case 2 - 2 children
    else {
      //cout << "crackers" << endl;
      int largestInLeftSubTree = findLargest(root->left);
      root->value = largestInLeftSubTree;
      remove(root->left, largestInLeftSubTree);
      //cout << "bread" << endl;
    }
  }
  else {
    if (value < root->value && root->left != NULL) {
      remove(root->left, value);
    }
    else if (value > root->value && root->right != NULL) {
      remove(root->right, value);
    }
  }  
}

void fixRedBlackTree(node* &current) {

  node* uncle = NULL;
  //Case 1 Root with no children
  cout << "ENTERED FIXED" << endl;
  if (current->parent == NULL) {
    current->color = 'B'; 
    cout << "Case I" << endl;
    cout << " Case 1 Current " << current << endl;
    return;
  }
  else {
    cout << "uncle of current node: " << current->parent->parent << endl;
    if (current->parent->parent != NULL) {
      if (current->parent == current->parent->parent->left) {
	uncle = current->parent->parent->right;
        cout << "Right Uncle: " << uncle << endl;
      }
      else if (current->parent == current->parent->parent->right) {
	uncle = current->parent->parent->left;
	cout << "Left Uncle: " << uncle << endl;
      }
    }
  }
  
  // case 2
  if (current->parent->color == 'B') {
    cout << "case 2 " << endl;
    return;
  }

  //Case 3 Parent and Uncle are red
  //if the parent = grandparent left, uncle is on right (comparing address)
  // makes sure parent and uncle exists
  //  else if (current->parent->parent->right != NULL && current->parent->parent->left != NULL) {

  else if (uncle != NULL) {
    cout << "uncle is not null" << endl;

    if (current->parent->color == 'R' && uncle->color == 'R') {
      cout << "Case 3a: parent & uncle are RED" << endl;
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
	  cout << "Case 4a: Parent L, node R - rotate left thru parent" << endl;
	  leftRotation(current->parent);
	  fixRedBlackTree(current->left);
	}
	// 9   
	//  10
	//    12
	else if (current->parent == current->parent->parent->right) {
	  cout << "Case 5a: Grandparent, parent, & node on right - rotate left thru grandP " << endl;
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
	cout << "Case 4b: Parent R, node L - rotate right thru parent" << endl;
	cout << "current->parent " << current->parent->value << endl;
	rightRotation(current->parent);
	cout << "current 4b " << current << endl;
	cout << "current->parent 4b " << current->parent << endl;
	fixRedBlackTree(current->right);
	}
	//    15
	//  10
	// 5
	else if (current->parent == current->parent->parent->left) {
	  cout << "Case 5b: GP, P, & N on Left - rotate right thru grandP " << endl;
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
      if (current->parent == current->parent->left) {
	cout << "Case 4c: " << endl;
	leftRotation(current->parent);
	fixRedBlackTree(current->left);
      }
      //Case 5c
      else if (current->parent == current->parent->parent->right) {
        cout << "Case 5c: P and N are right - rotate left thru grandP " << endl;
        current->parent->color = 'B';
        current->parent->parent->color = 'R';
        leftRotation(current->parent->parent);
      } 
    }   
 
    else if (current == current->parent->left) {
      if (current->parent == current->parent->parent->right) {
	cout << "Case 5d: " << endl;
	rightRotation(current->parent);
	fixRedBlackTree(current->right);
      }
      // case 5 R: parent and node are left and rotate right through grandP
      else if (current->parent == current->parent->parent->left) {
	cout << "Case 5d: P and N are left - rotate grandp" << endl;
	current->parent->color = 'B';
	current->parent->parent->color = 'R';
	rightRotation(current->parent->parent);
      }
    }  
  }

    else {
      cout << "No CASE:" << endl;
    }
}




void leftRotation(node* current) {
  cout << "Left Rotation" << endl;
  node* temp = current->right;
  cout << "temp " << temp << endl;
  cout << " temp value " << temp->value << endl;
  
  current->right = temp->left;
  if (temp->left != NULL) {
    temp->left->parent = current;
  }


  temp->parent = current->parent;
  if(current->parent == NULL) {
    cout << "FPARENT" << endl;
    
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
  cout << "Right Rotation" << endl;
  node* temp = current->left;
  cout << "temp " << temp << endl;
  cout << " temp value " << temp->value << endl;

  current->left = temp->right;
  cout << "hi" << endl;
  if (temp->right != NULL) {
    temp->right->parent = current;
  }
  cout << "hello" << endl;
  temp->parent = current->parent;
  cout << "bf if " << temp->parent << endl;
  if (current->parent == NULL) {
    cout << "IMG grand " << current << endl;
    cout << "temp->right " << temp->right << endl; 

    temp->right = current; // connect "current" node to right of its left node
    current->parent = temp; // assign "current" node parent to it left node.

    cout << "IMG af grandp " << current->parent << endl;
    cout << "temp->right "<< temp->right->value << " temp->left " << temp->left->value << endl;
    cout << "temp->parent " << temp->parent << endl;
    
    return;
  }
  else if (current == current->parent->left) {
    cout << "if" << endl;
    current->parent->left = temp;
  }
  else {
    cout << "else" << endl;
    current->parent->right = temp;
  }
  cout << "BROKEN" << endl;
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
