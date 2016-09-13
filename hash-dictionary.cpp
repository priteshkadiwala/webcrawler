
// Implementation of a dictionary using a hash table

#include <stdlib.h>
#include <string.h>
#include "hash-dictionary.h"

// Constructor
HashDictionary::HashDictionary()
{
  // Add your code here
	nElements = 0;
	for(int i = 0; i < MaxBuckets; i++) {
		buckets[i] = NULL;
	}
}

int
HashDictionary::hash(KeyType key) {
  // Add your code here
  int sum = 0;
  int len = strlen(key);
  for (int i = 0; i < len; i++) {
		sum += i*key[i];
  }
  return sum%MaxBuckets;
}

// Add a record to the dictionary. Returns false if key already exists
bool
HashDictionary::addRecord( KeyType key, DataType record)
{
  // Add your code here
 	int h = hash(key);
	HashNode *list = buckets[h];
	HashNode *entry= new HashNode();
	entry->key = strdup(key);
	entry->data = record;
	entry->next = NULL;
	if(list == NULL) {
		buckets[h] = entry;
		nElements++;
		return true;
	}
	while(list->next != NULL) {
		if( !strcmp(list->key, key) ) {
			break;
		}
		list = list->next;
	}
	if( !strcmp(list->key, key) ) {
		list->data = record;
		return false;
	} else {
		list->next = entry;\
 		return true;
	}
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
HashDictionary::findRecord( KeyType key)
{
	int h = hash(key);
	HashNode *list = buckets[h];
	while(list != NULL && (strcmp(list->key, key) != 0) ) {
		list = list->next;
	}
	if(list != NULL) {
		return list->data;
	}
	return NULL;
}

// Removes one element from the table
bool
HashDictionary::removeElement(KeyType key)
{
  // Add your code here
	int h = hash(key);
	HashNode * list = buckets[h];
	if(list != NULL) {
		if( !strcmp(list->key, key) ) {
			if(list->next != NULL) {
				buckets[h] = list->next;
				list = NULL;
				//delete list;
				nElements--;
				return true;
			} else {
				buckets[h] = NULL;
				//delete buckets[h];
				nElements--;
				return true;
			}
		}
	} else {
		return false;
	}
	while(list->next != NULL && strcmp(list->next->key, key) != 0) {
		list = list->next;
	}
	if(list->next != NULL) {
		list->next = NULL;
		list->next = list->next->next;
		//delete list->next;
		nElements--;
		return true;
	}
	return false;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
HashDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));

	*n = nElements;
	int i = 0;
	for (int h = 0; h < MaxBuckets; h++) {
		HashNode * n = buckets[h];
		while (n!=NULL) {
			a[i] = n->key;
			i++;
			n = n->next;
		}
	}
	*n = nElements;
	return a;
}

