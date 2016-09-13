
//
// CS251 Data Structures
// Hash Table
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Each hash entry stores a key, object pair
template <typename Data>
struct HashTableTemplateEntry {
  const char * _key;
  Data _data;
  HashTableTemplateEntry * _next;
};

// This is a Hash table that maps string keys to objects of type Data
template <typename Data>
class HashTableTemplate {
 public:
  // Number of buckets
  enum { TableSize = 2039};
  
  // Array of the hash buckets.
  HashTableTemplateEntry<Data> **_buckets;
  
  // Obtain the hash code of a key
  int hash(const char * key);
  
 public:
  HashTableTemplate();
  
  // Add a record to the hash table. Returns true if key already exists.
  // Substitute content if key already exists.
  bool insertItem( const char * key, Data data);

  // Find a key in the dictionary and place in "data" the corresponding record
  // Returns false if key is does not exist
  bool find( const char * key, Data * data);

  // Removes an element in the hash table. Return false if key does not exist.
  bool removeElement(const char * key);

  // Returns the data that corresponds to this index.
  // Data is initialized to 0s if the entry does not exist
  Data operator[] (const char * &key);
};

template <typename Data>
int HashTableTemplate<Data>::hash(const char * key)
{
  int h = 0;
  const char * p = key;
  while (*p) {
    h += *p;
    p++;
  }
  return h % TableSize;
}

template <typename Data>
HashTableTemplate<Data>::HashTableTemplate()
{
  // Add implementation here
	_buckets = new HashTableTemplateEntry<Data> * [TableSize];
	for(int i = 0; i < TableSize; i++) {
		_buckets[i] = NULL;
	}
}

template <typename Data>
bool HashTableTemplate<Data>::insertItem( const char * key, Data data)
{
  // Add implementation here
	int i = hash(key);
	HashTableTemplateEntry<Data> *list = _buckets[i];
	HashTableTemplateEntry<Data> *entry = new HashTableTemplateEntry<Data>();
	entry->_key = strdup(key);
	entry->_data = data;
	entry->_next = NULL;
	if(list == NULL) {
		_buckets[i] = entry;
		return false;
	}
	while(list->_next != NULL) {
		if(strcmp(list->_key, key) == 0) {
			break;
		}
		list = list->_next;
	}
	if(strcmp(list->_key, key) == 0) {
		list->_data = data;
		return true;
	}
	list->_next = entry;
	return false;
}

template <typename Data>
bool HashTableTemplate<Data>::find( const char * key, Data * data)
{
  // Add implementation here
  int i = hash(key);
  HashTableTemplateEntry<Data> * list = _buckets[i];
  while(list != NULL && strcmp(list->_key, key) != 0) {
  	list = list->_next;
  }
  if(list != NULL) {
  	*data = list->_data;
	return true;
  }
  return false;
}

template <typename Data>
Data HashTableTemplate<Data>::operator[] (const char * &key) {
	Data d;
	int i = hash(key);
	d = _buckets[i]->_data;
	return d;
}

template <typename Data>
bool HashTableTemplate<Data>::removeElement(const char * key)
{
  // Add implementation here
	int i = hash(key);
	HashTableTemplateEntry<Data> * list = _buckets[i];
	if(list != NULL) {
		if(strcmp(list->_key, key) == 0) {
			if(list ->_next != NULL) {
				_buckets[i] = list->_next;
				list = NULL;
				delete list;
				return true;
			} else {
				_buckets[i] = NULL;
				delete _buckets[i];
				return true;
			}
		}
	} else {
		return false;
	}
	
	while(list->_next != NULL && strcmp(list->_next->_key, key) != 0) {
		list = list->_next;
	}
	if(list->_next != NULL) {
		list->_next = NULL;
		list->_next = list->_next->_next;
		delete list->_next;
		return true;
	}
  return false;
}

template <typename Data>
class HashTableTemplateIterator {
  int _currentBucket;
  HashTableTemplateEntry<Data> *_currentEntry;
  HashTableTemplate<Data> * _hashTable;
 public:
  HashTableTemplateIterator(HashTableTemplate<Data> * hashTable);
  bool next(const char * & key, Data & data);
};

template <typename Data>
HashTableTemplateIterator<Data>::HashTableTemplateIterator(HashTableTemplate<Data> * hashTable)
{
  // Add implementation here
	_hashTable = hashTable;
	_currentBucket = -1;
	_currentEntry = NULL;
}

template <typename Data>
bool HashTableTemplateIterator<Data>::next(const char * & key, Data & data)
{
  // Add implementation here
  if(_currentBucket == -1) {
  	_currentBucket++;
	_currentEntry = _hashTable->_buckets[_currentBucket];
	if(_currentEntry != NULL) {
		key = _currentEntry->_key;
		data = _currentEntry->_data;
		return true;
	}
  }

  if(_currentEntry != NULL) {
  	if(_currentEntry->_next != NULL) {
		_currentEntry = _currentEntry->_next;
		key = _currentEntry->_key;
		data = _currentEntry->_data;
		return true;
	} else {
		_currentEntry = _currentEntry->_next;
	}
  }
  while(_currentEntry == NULL) {
	  _currentBucket++;
	  _currentEntry = _hashTable->_buckets[_currentBucket];
	  if(_currentBucket >= _hashTable->TableSize) {
	  	return false;
	  }
  }
  key = _currentEntry->_key;
  data = _currentEntry->_data;
  return true;
}

