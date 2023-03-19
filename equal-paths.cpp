#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int equalpathsHelper(Node* root);
int equalpathsHelper(Node* root){

  if (root == nullptr) return 0; //base case
  int leftHeight = equalpathsHelper(root->left);
  int rightHeight = equalpathsHelper(root->right);
  if(leftHeight <= rightHeight) { 
    return 1+rightHeight; 
  }
  else { 
    return 1+leftHeight; 
  }

}


bool equalPaths(Node * root)
{
  if(root == nullptr){return true;}
  if(equalPaths(root->left) && equalPaths(root->right)){
    int leftHeight = equalpathsHelper(root->left);
    int rightHeight = equalpathsHelper(root->right);
    int height = leftHeight - rightHeight;
    if(height == 0) return true;
    if(leftHeight < 2 && rightHeight < 2) return true;
  }
  
  return false;
}

