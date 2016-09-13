#include <stdio.h>
#include <string.h>
#include "webcrawler.h"

// Add your implementation here

WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots) {
        _urlArray = new URLRecord[maxUrls];
        for(int i = 0; i < nurlRoots; i++) {
                _urlArray[i]._url = strdup(urlRoots[i]);
                _urlArray[i]._description = NULL;
        }
        _headURL = 0;
        _tailURL = nurlRoots;
        _maxUrls = maxUrls;

        _urlToUrlRecord = new HashTableTemplate<int>();
        _wordToURLRecordList = new HashTableTemplate<URLRecordList *>();

	count = 0;
	description = new char[500];
	currentAnchor = false;
	prevAnchor = false;
	index = -1;
	content = true;
	prevSpace = true;
}



void WebCrawler::crawl() {

        while (_headURL <_tailURL) {
                int n;
                char * buffer = fetchHTML(_urlArray[_headURL]._url, &n);
		if(buffer != NULL) {
			parse(buffer, n);
			insertWords();
		}
		_headURL++;
        }//while
}

void WebCrawler::onContentFound(char c) {
	if(currentAnchor == false && prevAnchor == false) {
		count = 0;
	}
	if(prevAnchor == true && content == false) {
		description[count] = '\0';
		if(_urlArray[prevIndex]._description == NULL) {
			if(description[0] != '\0') {
				//if((description[0] >= 'a' &&  description[0] <= 'z') || (description[0] >= 'A' && description[0] <= 'Z')) {
					_urlArray[prevIndex]._description = strdup(description);
				//}
			}
		}
		count = 0;
	}
	if(count < 500) {
		if(currentAnchor == true) {
			if(c == ' ') {
				if(prevSpace == false) {
					description[count] = c;
					count++;
				}
				prevSpace = currentSpace;
				currentSpace = true;
			} else {
				description[count] = c;
				count++;
				prevSpace = currentSpace;
				currentSpace = false;
			}	
		}
	}
	content = true;
}

void WebCrawler::onAnchorFound(char *url) {
	prevAnchor = currentAnchor;
	bool match = false;
	if(index != -1) {
		prevIndex = index;
	}
	if(_tailURL <= _maxUrls) {
		if( strncmp(url, "http://www", 10) ) {
			currentAnchor = false;
		} else {
			currentAnchor = true;
			for(int i = 0; i < _tailURL; i++) {
				if( !strcmp(url, _urlArray[i]._url) ) {
					match = true;
					index = i;
					break;
				}
			}
			if(!match) {
				index = _tailURL;
				_urlArray[index]._url = strdup(url);
				_urlArray[index]._description = NULL;
				_tailURL++;
				int n;
				if( !_urlToUrlRecord->find(url, &n) ) {
					_urlToUrlRecord->insertItem(url, _headURL);
				}
			}
		}
	}
	content = false;
}


void WebCrawler::insertWords() {
    if(_urlArray[_headURL]._description == NULL) {
    	return;
    }
    int i = 0;
    int len = 0;
    char * word = new char[500];
    URLRecordList *w;
    while(_urlArray[_headURL]._description[i] != '\0') {
        if(_urlArray[_headURL]._description[i] == ' ') {
            word[len] = '\0';
	    if(word[0] != '\0') {
	    	if( (word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A' && word[0] <= 'Z')) {
	    		if(_wordToURLRecordList->find(word, &w)) {
	    			if(w->_urlRecordIndex != _headURL) {
                			URLRecordList * add = new URLRecordList();
                			add->_urlRecordIndex = _headURL;
               				add->_next = w;
            				_wordToURLRecordList->insertItem(word, add);
				}
	    		} else {
                			URLRecordList * add = new URLRecordList();
                			add->_urlRecordIndex = _headURL;
            				add->_next = NULL;
            				_wordToURLRecordList->insertItem(word, add);
	    		}
		}
	    }
	    len = 0;
        } else {
        	word[len] = _urlArray[_headURL]._description[i];
        	len++;
	}
        i++;
    }
}

void WebCrawler::writeURLFile(const char * urlFileName) {
	FILE *fp;
	fp = fopen(urlFileName, "w");
	int i = 0;
	while(i < _maxUrls) {
		if(_urlArray[i]._url != NULL) {
			char * url = strdup(_urlArray[i]._url);
			fprintf(fp, "%d %s\n", i, url);
			if(_urlArray[i]._description != NULL) {
				char * d = strdup(_urlArray[i]._description);
				fprintf(fp, "%s\n", d);
			} else {
				fprintf(fp, "NO DESCRIPTION\n");
			}
			fprintf(fp, "\n");
		}
		i++;
	}
	fclose(fp);
}

void WebCrawler::writeWordFile(const char * wordFileName) {
	FILE *fp;
	fp = fopen(wordFileName, "w");
	HashTableTemplateIterator<URLRecordList *> iterator(_wordToURLRecordList);
	const char *key;
	URLRecordList * data;
	while(iterator.next(key, data)) {
		if(key != NULL) {
			fprintf(fp, "%s ", key);
			while(data != NULL) {
				fprintf(fp, "%d ", data->_urlRecordIndex);
				data = data->_next;
			}
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void printUsage() {
	printf("  Usage: webcrawler [-u #ofURL] url [url url url...]\n");
	printf("  Example: webcrawler http://www.purdue.edu\n");
	printf("  Example: webcrawler -u 100  http://www.purdue.edu http://www.purdue.edu\n");
}

int main(int argc, char ** argv) {
	
	int maxUrls = 1000;
	const char ** url;
	const char * option;

	if(argc < 2) {
		printUsage();
		exit(1);
	} else if(argc < 4){
		option = "";	
	} else {
		if( !strcmp(argv[1], "-u") ) {
			maxUrls = atoi(argv[2]);
			option = "-u";
		} else {
			option = "";
		}
	}
		
	if ( !strcmp(option, "-u") ) {
		int nurlRoots = argc - 3;
		url = new const char*[nurlRoots];
		for(int i = 0; i < nurlRoots; i++) {
			url[i] = strdup(argv[i+3]);
		}
		WebCrawler wc = WebCrawler(maxUrls, nurlRoots, url);
		wc.crawl();
		wc.writeURLFile("url.txt");
		wc.writeWordFile("word.txt");
		printf("SUCESS\n");
	} else if (option, "") {
		int nurlRoots = argc - 1;
		url = new const char*[nurlRoots];
		for(int i = 0; i < nurlRoots; i++) {
			url[i] = strdup(argv[i+1]);
		}
		WebCrawler wc = WebCrawler(maxUrls, nurlRoots, url);
		wc.crawl();
		wc.writeURLFile("url.txt");
		wc.writeWordFile("word.txt");
		printf("SUCESS\n");
	}
}













