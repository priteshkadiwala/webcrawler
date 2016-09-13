
// Implementation of a dictionary using an array and binary search
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "bsearch-dictionary.h"

// Constructor
BinarySearchDictionary::BinarySearchDictionary()
{
	sorted = false;
}

bool
BinarySearchDictionary::addRecord( KeyType key, DataType record) {
	sorted = false;
	return ArrayDictionary::addRecord(key, record);
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
BinarySearchDictionary::findRecord( KeyType key)
{
	
	if (!sorted) {
		sort();
		sorted = true;
	}
	
	// Use binary search
	int low = 0;
	int high = ArrayDictionary::currentNumber - 1;
	while(high >= low) {
		int mid = (low + high) / 2;
		int result = strcmp(key, array[mid].key);
		if(result == 0) {
			return ArrayDictionary::array[mid].data;
		} else if(result < 0) {
			high = mid-1;
		} else {
			low = mid+1;
		}
	}
	// Add your implementation here
	return NULL;

}



// Sort array using heap sort
void
BinarySearchDictionary::sort()
{

//	printf("Before\n");
//	for (int i = 0; i < currentNumber; i++) {
//		printf("%s\n", array[i].key);
//	}

        // Add your code here
	int n = ArrayDictionary::currentNumber;
	ArrayDictionaryNode *heap = new ArrayDictionaryNode[n];
	int last = 0;
	for(int i = 0; i < n; i++) {
		assert(last < n);

		heap[i] = array[i];
		last++;

		int child = last -1;
		int parent = theParent(child);
		while(child > 0) {
			if(strcmp(heap[child].key, heap[parent].key) > 0) {
				break;
			}
			ArrayDictionaryNode tmp = heap[child];
			heap[child] = heap[parent];
			heap[parent] = tmp;
			
			child = parent;
			parent = theParent(child);
			
		}
	}

	for(int i = 0; i < n; i++) {
		assert(last > 0);

		ArrayDictionaryNode min = heap[0];
		heap[0] = heap[last-1];
		last--;

		int parent = 0;
		int ileft = left(parent);
		int iright = right(parent);
		
		while(ileft < last) {
			int minChild = ileft;

			if(iright < last && (strcmp(heap[iright].key, heap[ileft].key) < 0)) {
				minChild = iright;
			}

			if(strcmp(heap[parent].key, heap[minChild].key) < 0) {
				break;
			}

			ArrayDictionaryNode tmp = heap[minChild];
			heap[minChild] = heap[parent];
			heap[parent] = tmp;

			parent = minChild;
			ileft = left(parent);
			iright = right(parent);
		}
		array[i] = min;
	}

//	printf("After\n");
//	for (int i = 0; i < currentNumber; i++) {
//		printf("%s\n", array[i].key);
//	}

}

