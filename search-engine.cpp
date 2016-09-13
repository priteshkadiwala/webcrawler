
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "search-engine.h"
#include <time.h>
#include <math.h>

#include "array-dictionary.h"
#include "avl-dictionary.h"
#include "bsearch-dictionary.h"
#include "hash-dictionary.h"
#include "webcrawl.h"

SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
  MiniHTTPD(port)
{
	// Create dictionary of the indicated type
	// Populate dictionary and sort it if necessary

	URLRecord * _urlArray = new URLRecord[2000];

	FILE *urlFile;
	urlFile = fopen("url2.txt", "r");
	int c;
	char *words = new char[10000];
	const char *get = new char[10000];
	int i=0;
	size_t len = 0;

	while( getline(&words, &len, urlFile) != -1) {
		if(words[0] == '\n') {
			continue;
		}
		if( words[0] >= 48 && words[0] <= 57) {
			get = strchr(strdup(words), ' ');
			get++;
			_urlArray[i]._url = strdup(get);
		} else {
			_urlArray[i]._description = strdup(words);
			i++;
		}
	}

	type = new char[50];
	if(dictionaryType == ArrayDictionaryType) {
		_wordToURLList = new ArrayDictionary();
		type = strdup("ArrayDictionary");
	} else if(dictionaryType == HashDictionaryType) {
		_wordToURLList = new HashDictionary();
		type = strdup("HashDictionary");
	} else if(dictionaryType == AVLDictionaryType) {
		_wordToURLList = new AVLDictionary();
		type = strdup("AVLDictionary");
	} else if(dictionaryType == BinarySearchDictionaryType) {
		_wordToURLList = new BinarySearchDictionary();
		type = strdup("BinarySearchDictionary");
	} else {
		_wordToURLList = NULL;
		type = NULL;
	}

	FILE *wordFile;
	wordFile = fopen("word2.txt", "r");
	char str[10000];
	
	while(fgets(str, 10000, wordFile) != NULL) {
		char *word = strtok(str, " ");
		char *key = word;
		void *data;
		word = strtok(NULL, " ");
		URLRecordList *list = NULL;
		while(word != NULL) {
			if(strcmp(word, "\n") != 0) { 
				URLRecordList *head = list;
				if(head != NULL) {
					while(head->_next != NULL) {
						head = head->_next;
					}
				}
				URLRecordList *e = new URLRecordList;
				int h = atoi(word);
				e->_urlRecord = &_urlArray[h];
				e->_next = NULL;
				if(list == NULL) {
					list = e;
				} else {
					head->_next = e;
				}
			}
			word = strtok(NULL, " ");
		}
		_wordToURLList->addRecord(key, (void *)list);
	}

	// set for time
	counts = 0;
	sum = 0;
}

void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{
  if (strcmp(documentRequested, "/")==0) {
    // Send initial form
    fprintf(fout, "<TITLE>CS251 Search</TITLE>\r\n");
    fprintf(fout, "<CENTER><H1><em>Boiler Search</em></H1>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "Search:\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
    fprintf(fout, "</H2>\n");
    fprintf(fout, "</FORM></CENTER>\n");
    return;
  }

  // TODO: The words to search in "documentRequested" are in the form
  // /search?word=a+b+c
  //
  // You need to separate the words before search
  // Search the words in the dictionary and find the URLs that
  // are common for al the words. Then print the URLs and descriptions
  // in HTML. Make the output look nicer.

  // Here the URLs printed are hardwired
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
    int n;

	char * get = strchr(strdup(documentRequested), '=');
	get++;


	if(strstr(documentRequested, "favicon.ico") != '\0') {
		return;
	}

	char * s = strtok(strdup(get), "+");

	int nurls = 0;

	char *words = new char[10000];
	words[0] = '\0';
	char ** urls = new char * [10000];
    char ** URL = new char * [10000];
	char ** description = new char * [10000];
    char ** DES = new char * [10000];
    int count = 0;
    int T = 0;
	while(s != NULL) {
		strcat(words, s);
		URLRecordList * data = (URLRecordList*) _wordToURLList->findRecord(s);
		while(data != NULL) {
            //printf("how many times :%d\n", T);
            URL[count] = data->_urlRecord->_url;
            DES[count]=data->_urlRecord->_description;
            count++;
            data = data->_next;
		}
		  s = strtok(NULL, "+");
          T = 0;
		if(s != NULL) {
			strcat(words, " ");
		}
	}

    int sum = 0;
    for(int i = 0; i < count; i++) {
        int tmp = 0;
        for(int j = 0; j < count; j++) {
            if( !strcmp(URL[i], URL[j]) ) {
                tmp++;
            }
        }
        if(sum < tmp) {
            sum = tmp;
        }
    }
    bool a;
    printf("sum: %d\n", sum);
    for(int i = 0; i < count; i++) {
        int tmp = 0;
        for(int j = 0; j < count; j++) {
            if( !strcmp(URL[i], URL[j]) ) {
                tmp++;
            }
        }
        if(sum == tmp) {
            a = true;
            if(nurls > 0) {
                for(int j = 0; j < nurls; j++) {
                    if( !strcmp(urls[j], URL[i]) ) {
                        a = false;
                    }
                }
            }
            if(a == true) {
                urls[nurls] = URL[i];
                description[nurls] = DES[i];
                nurls++;
            }
        }
    }


/*
  const int nurls=2;

  const char * words = "data structures";

  const char * urls[] = {
    "http://www.cs.purdue.edu",
    "http://www.cs.purdue.edu/homes/cs251"
  };

  const char * description[] = {
    "Computer Science Department. Purdue University.",
    "CS251 Data Structures"
  };
*/
  fprintf( stderr, "Type of search: \"%s\"\n",type);

  fprintf( stderr, "Search for words: \"%s\"\n", words);

  double time = ts.tv_nsec;
  counts++;
  sum += time;
  double ave = sum/counts;
  fprintf( stderr, "time is: %lf nanoseconds\n", time);
  fprintf( stderr, "Average time is: %lf nanoseconds\n", ave);
  fprintf( stderr, "time is: approximately %lf milliseconds\n", time* pow(10, -6));
  fprintf( stderr, "Average time is: approximately %lf milliseconds\n", ave*pow(10,-6));


  fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  fprintf( fout, "<H1> <Center><em>Boiler Search</em></H1>\n");
  fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", words );

  for ( int i = 0; i < nurls; i++ ) {
    fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", i+1, urls[i], urls[i] );
    fprintf( fout, "<blockquote>%s<p></blockquote>\n", description[i] );
  }

  // Add search form at the end
  fprintf(fout, "<HR><H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "Search:\n");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM>\n");
}

void
printUsage()
{
  const char * usage =
    "Usage: search-engine port (array | hash | avl | bsearch)\n"
    "  It starts a search engine at this port using the\n"
    "  data structure indicated. Port has to be larger than 1024.\n";

  fprintf(stderr, "%s", usage);
}

int main(int argc, char ** argv)
{
  if (argc < 3) {
    printUsage();
    return 1;
  }

  // Get port
  int port;
  sscanf( argv[1], "%d", &port);

  // Get DictionaryType
  const char * dictType = argv[2];
  DictionaryType dictionaryType;
  if (!strcmp(dictType, "array")) {
    dictionaryType = ArrayDictionaryType;
  }
  else if (!strcmp(dictType, "hash")) {
    dictionaryType = HashDictionaryType;
  }
  else if (!strcmp(dictType, "avl")) {
    dictionaryType = AVLDictionaryType;
  }
  else if (!strcmp(dictType, "bsearch")) {
    dictionaryType = BinarySearchDictionaryType;
  }
  else {
    printUsage();
    return 0;
  }
  
  SearchEngine httpd(port, dictionaryType);
  
  httpd.run();

  return 0;
}
