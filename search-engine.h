
#include "minihttpd.h"
#include "dictionary.h"

// Types of dictionaries used
enum DictionaryType {
  ArrayDictionaryType,
  HashDictionaryType,
  AVLDictionaryType,
  BinarySearchDictionaryType
};

// Inherits from MiniHTTPD
class SearchEngine : public MiniHTTPD {
  Dictionary * _wordToURLList;
  
  // Add any other member variables you need
  char *type;
  int counts;
  double sum;

 public:
  // Constructor for SearchEngine
  SearchEngine( int port, DictionaryType dictionaryType);

  // Called when a request arrives
  void dispatch( FILE * out, const char * documentRequested);	//requestLine );

};



