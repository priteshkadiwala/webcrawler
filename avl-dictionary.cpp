
// Implementation of a dictionary using an AVL tree
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "avl-dictionary.h"

bool debug = false;

// Constructor
AVLDictionary::AVLDictionary()
{
	root = NULL;
	nElements = 0;
}

// Add a record to the dictionary. Returns false if key already exists
bool
AVLDictionary::addRecord( KeyType key, DataType record)
{
	
	if ( debug) {
		printf("------------------------------------\n");
		printf("addRecord(\"%s\",%ld)\n",  key, (long) record);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}
	
	// Add your implementation here
	//Find node to insert into
	//Node does not exist. Create it.
	//Height might not be valid anymore.
	//We need to restructure .
	
	AVLNode *current = root;
	AVLNode *previous = NULL;
	while(current != NULL) {
		previous = current;
		if(strcmp(key, current->key) < 0) {
			current = current->left;
		} else if(strcmp(key, current->key) > 0) {
			current = current->right;
		} else {
			current->data = record;
			return false;
		}
	}

	AVLNode *n = new AVLNode();
	n->key = strdup(key);
	n->data = record;
	n->left = NULL;
	n->right = NULL;
	n->parent = NULL;
	n->height = 1;

	if(previous == NULL) {
		root = n;
	} else {

		if(strcmp(key, previous->key) < 0) {
			previous->left = n;
		} else {
			previous->right = n;
		}

		n->parent = previous;

		AVLNode *m = n->parent;
	
		while(m != NULL) {
			int maxheight = 0;

			if(m->left != NULL) {
				maxheight = m->left->height;
			}

			if(m->right != NULL && m->right->height > maxheight) {
				maxheight = m->right->height;
			}
			m->height = maxheight + 1;
			m = m->parent;
		}
	}





	if ( debug) {
		printf("---------- Before Restructure -----------------\n");
		printNode("", root, 0);
	}

	restructure(n);
	// Call restructure

	if (debug) {
		checkRecursive(root);
		
		printf("---------- After Restructure -----------------\n");
		printNode("", root, 0);
	}
	nElements++;	
	return true;
}

void
AVLDictionary::restructure(AVLNode * n) {
	//Balance the tree starting at n 
	//Go upwards until root is found
        // See class notes
	// Add your implementation here
	AVLNode *z = n;
	while(z != NULL) {
		int hleft = 0;
		if( z->left != NULL) {
			hleft = z->left->height;
		}
		int hright = 0;
		if(z->right != NULL) {
			hright = z->right->height;
		}

		if(hleft > hright) {
			z->height = hleft + 1;
		} else {
			z->height = hright + 1;
		}

		int hdiff = hright - hleft;
		if(hdiff < 0) {
			hdiff *= -1;
		}

		if(hdiff <= 1) {
			z = z->parent;
			continue;
		}

		AVLNode *y = NULL;
		int maxh = 0;
		if(z->left != NULL) {
			y = z->left;
			maxh = z->left->height;
		}
		if(z->right != NULL && maxh < z->right->height) {
			y = z->right;
		}
		assert(y != NULL);

		AVLNode *x = NULL;
		maxh = 0;
		if(y->left != NULL) {
			x = y->left;
			maxh = x->height;
		}
		if(y->right != NULL && y->right->height > maxh) {
			x = y->right;
		}
		assert(x != NULL);

		AVLNode  *a, *b, *c, *t0, *t1, *t2, *t3;
		if(z->right == y) {
			if(y->right == x) {		//case 1
				a=z;
				b=y;
				c=x;
				t0=z->left;
				t1=y->left;
				t2=x->left;
				t3=x->right;
			} else {				//case 2
				a=z;
				c=y;
				b=x;
				t0=z->left;
				t1=x->left;
				t2=x->right;
				t3=y->right;
			}
		} else {
			if(y->left == x) {		// case 3
				a=x;
				b=y;
				c=z;
				t0=x->left;
				t1=x->right;
				t2=y->right;
				t3=z->right;
			} else {				// case 4
				a=y;
				b=x;
				c=z;
				t0=y->left;
				t1=x->left;
				t2=x->right;
				t3=z->right;
			}
		}

		AVLNode *p = z->parent;
		if(p != NULL) {
			if(p->left == z) {
				p->left = b;
			} else {
				p->right = b;
			}
		} else {
			root = b;
		}

		b->parent = p;
		b->left = a;
		b->right = c;
		a->parent = b;
		a->left = t0;
		a->right = t1;
		c->parent = b;
		c->left = t2;
		c->right = t3;

		if(t0 != NULL) {
			t0->parent = a;
		}
		if(t1 != NULL) {
			t1->parent = a;
		}
		if(t2 != NULL) {
			t2->parent = c;
		}
		if(t3 != NULL) {
			t3->parent = c;
		}
		// fix height of a
		int maxheight = 0;
		if(a->left != NULL) {
			maxheight = a->left->height;
		}
		if(a->right != NULL && a->right->height > maxheight) {
			maxheight = a->right->height;
		}
		a->height = maxheight+1;
		// fix heihgt of c
		maxheight = 0;
		if(c->left != NULL) {
			maxheight = c->left->height;
		}
		if(c->right != NULL && c->right->height > maxheight) {
			maxheight = c->right->height;
		} 
		c->height = maxheight+1;
		// fix height of b
		maxheight = 0;
		if(b->left != NULL) {
			maxheight = b->left->height;
		}
		if(b->right != NULL && b->right->height > maxheight) {
			maxheight = b->right->height;
		} 
		b->height = maxheight+1;

		z=p;
	}
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
AVLDictionary::findRecord( KeyType key)
{
        // Add your implementation here
	AVLNode *n = root;
	while(n != NULL) {
		if( strcmp(key, n->key) < 0) {
			n = n->left;
		} else if( strcmp(key, n->key) > 0) {
			n = n->right;
		} else {
			return n->data;
		}
	}
	return NULL;
}

// Removes one element from the dictionary
bool
AVLDictionary::removeElement(KeyType key)
{


	if (debug) {
		printf("------------------------------------\n");
		printf("removeElement(\"%s\")\n",  key);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}


	// Add your implementation here
	
	if(nElements == 0) {
		return false;
	} else if(nElements == 1) {
		if( !strcmp(key, root->key) ) {
			root = NULL;
			nElements--;
		} else {
			return false;
		}
	} else {

		AVLNode * current = root;
		AVLNode * previous = NULL;
	
		while( current != NULL) {
			if( strcmp(key, current->key) < 0) {
				previous = current;
				current = current->left;
			} else if( strcmp(key, current->key) > 0) {
				previous = current;
				current = current->right;
			} else {
				break;
			}
		}
		if(current == NULL) {
			return false;
		}
		// current is remove node
		bool isLeft;
		if(previous != NULL) {
			if(previous->left == current) {
				isLeft = true;
			} else {
				isLeft = false;
			}
		}
		if(current->left == NULL && current->right == NULL) {
			if(isLeft == true) {
				previous->left = NULL;
			} else {
				previous->right =NULL;
			}
			nElements--;
			restructure(previous);

		} else if(current->left == NULL) {
			if(previous == NULL) {
				root = current->right;
				nElements--;
				restructure(root);
			} else {
				if(isLeft == true) {
					previous->left = current->right;
					current->right->parent = previous;
				} else {
					previous->right = current->right;
					current->right->parent = previous;
				}
				nElements--;
				restructure(previous);
			}
		
		} else {
			AVLNode * last = current->left;
			AVLNode * preLast = current;
			while(last->right != NULL) {
				preLast = last;
				last = last->right;
			}

			if(previous == NULL && current != preLast) {
				if(last->left != NULL) {
					preLast->right = last->left;
					preLast->right->parent = preLast;
				} else {
					preLast->right = NULL;
				}
				root = last;
				last->parent = NULL;
				last->left = current->left;
				last->left->parent = last;
				if(current->right != NULL) {
					last->right = current->right;
					last->right->parent = last;
				}
				nElements--;
				restructure(preLast);
			
			} else if(previous != NULL && current != preLast) {
				if(current->right != NULL) {
					last->right = current->right;
					last->right->parent = last;
				}
				if(last->left != NULL) {
					preLast->right = last->left;
					preLast->right->parent = preLast;
				} else {
					preLast->right = NULL;
				}
				if(isLeft) {
					previous->left = last;
				} else {
					previous->right = last;
				}
				last->parent = previous;
				last->left = current->left;
				last->left->parent = last;
				nElements--;
				restructure(preLast);
			
			} else if(previous == NULL && current == preLast) {
				root = last;
				last->parent = NULL;
				if(current->right != NULL) {
					last->right = current->right;
					last->right->parent = last;
				}
				nElements--;
				restructure(last);
			
			} else if(previous != NULL && current == preLast) {
				if(isLeft) {
					previous->left = last;
				} else {
					previous->right = last;
				}
				last->parent = previous;
				if(current->right != NULL) {
					last->right = current->right;
					last->right->parent = last;
				}
				nElements--;
				restructure(last);
			}
		}
	}






	if (debug) {
		printf("---------- After -----------------\n");
		printNode("", root, 0);

		checkRecursive(root);
	}
	
	return true;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
AVLDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));
	*n = 0;
	addKeysRecursive(root, a, n);

	return a;
}

// Add keys recursively
void
AVLDictionary::addKeysRecursive(AVLNode * node, KeyType * a, int * n) {
	if (node==NULL) {
		return;
	}
	
	a[*n] = node->key;
	(*n)++;
	addKeysRecursive(node->left, a, n);
	addKeysRecursive(node->right, a, n);
}


