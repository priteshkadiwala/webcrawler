
// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include <stdlib.h>
#include <string.h>

#include "array-dictionary.h"

// Constructor
ArrayDictionary::ArrayDictionary()
{
  // Add your code here
  	currentNumber = 0;
  	maxNumber = 100;
	array = new ArrayDictionaryNode[maxNumber];
}

// Add a record to the dictionary. Returns false if key already exists
bool
ArrayDictionary::addRecord( KeyType key, DataType record)
{
        // Add your code here
	if(currentNumber == maxNumber) {
		maxNumber = 2*maxNumber;
		ArrayDictionaryNode *newarray = new ArrayDictionaryNode[maxNumber];
		for(int i = 0; i < currentNumber; i++) {
			newarray[i] = array[i];
		}
		delete []array;
		array = newarray;
	}
	for(int i = 0; i < currentNumber; i++) {
		if( !strcmp(key, array[i].key) ) {
			array[i].data = record;
			return false;
		}
	}
	array[currentNumber].key = strdup(key);
	array[currentNumber].data = record;
	currentNumber++;
	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
ArrayDictionary::findRecord( KeyType key)
{
        // add your code here
	for(int i = 0; i < currentNumber; i++) {
		if( !strcmp(key, array[i].key) ) {
			return array[i].data;
		}
	}
	return NULL;
}

// Removes one element from the table
bool
ArrayDictionary::removeElement(KeyType key)
{
        // Add your code here
	
	// empty array
	if(currentNumber == 0) {
		return false;
	}
	for(int i = 0; i < currentNumber; i++) {
		if( !strcmp(key, array[i].key) ) {
			for(int j = i; j < currentNumber - 1; j++) {
				array[j] = array[j+1];
			}
			array[currentNumber-1].key = NULL;
			array[currentNumber-1].data = NULL;
			currentNumber--;
			return true;
		}
	}
	// unable to find
	return false;
		
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
ArrayDictionary::keys(int * n)
{
        // Add yoru code here
	*n = currentNumber;
	const char **keys = new const char*[*n];
	for(int i = 0; i < *n; i++) {
		keys[i] = strdup(array[i].key);	
	}
	return keys;
}
