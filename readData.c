/* readData.c
 *
 * Written by Selina (z5208109) & Yasmin (z5207093)
 * Group: duckduckgo
 * Start Date: 10/10/18
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "set.h"
#include "graph.h"
#include "BSTree.h"
#include "readData.h"
#include "mystring.h"

#define SEEN_ONCE       1
#define SEEN_TWICE      2
#define URL_LENGTH      55
#define MAX_LINE        1001
#define NULL_TERM_SPACE 1
#define START_TAG_LEN   16
#define END_TAG_LEN     14
#define CHAR_LEN        1
#define TRUE			1
#define FALSE			0

/* Trims leading and ending spaces 
 * Written by jas for 1521 mymysh.c 18s2
 */
void trim(char *str) 
{
	int first, last;
	first = 0;
	while (isspace(str[first])) first++;
	last  = strlen(str)-1;
	while (isspace(str[last])) last--;
	int i, j = 0;
	for (i = first; i <= last; i++) str[j++] = str[i];
	str[j] = '\0';
}


/* tokenise: split a string around a set of separators
 * create an array of separate strings
 * final array element contains NULL
 * Written by jas for 1521 mymysh.c 18s2
 */
char **tokenise(char *str, char *sep)
{
   // temp copy of string, because strtok() mangles it
   char *tmp;
   // count tokens
   tmp = mystrdup(str);
   int n = 0;
   strtok(tmp, sep); n++;
   while (strtok(NULL, sep) != NULL) n++;
   free(tmp);
   // allocate array for argv strings
   char **strings = malloc((n+1)*sizeof(char *));
   assert(strings != NULL);
   // now tokenise and fill array
   tmp = mystrdup(str);
   char *next; int i = 0;
   next = strtok(tmp, sep);
   strings[i++] = mystrdup(next);
   while ((next = strtok(NULL,sep)) != NULL)
      strings[i++] = mystrdup(next);
   strings[i] = NULL;
   free(tmp);
   free(next);
   return strings;
}


/* freeTokens: free memory associated with array of tokens */
void freeTokens(char **toks)
{
	for (int i = 0; toks[i] != NULL; i++)
		free(toks[i]);
	free(toks);
}


// frees the inlinks and outlinks from the URLNodes
void freeLinks(Link head)
{
    if ( head == NULL) return;
	Link temp = head;
	Link curr = head;
	while (curr != NULL) {
		temp = curr;
		free(temp->URLName);
		curr = curr->next;
		free(temp);
	}
}


// free entire graphADT
void freeGraph(Graph g)
{
	int i;
	for (i = 0; i < g->numURLs; i++) {
		freeLinks(g->listOfUrls[i]->inLink);
		freeLinks(g->listOfUrls[i]->outLink);
		free(g->listOfUrls[i]->text);
		free(g->listOfUrls[i]->URLName);
		free(g->listOfUrls[i]);
	}
	free(g->listOfUrls);
	free(g);
}


/* Removes trailing spaces and punctuation at the end of word
 * Also converts all letters to lowercase.
 */
char *normalise(char *str) 
{
	char *word = mystrdup(str);
	trim(word);
	// Converts to all lowercase.
	int i;
	for (i = 0; word[i] != '\0'; i++) 
		word[i] = tolower(word[i]);
	// Removes punctuation at the end.
	int lastLetter = strlen(word) - 1;
	if (word[lastLetter] == '.'
	 || word[lastLetter] == '?'
	 || word[lastLetter] == ','
	 || word[lastLetter] == ';') word[lastLetter] = '\0';
	
	return word;
}


/* Creates a set of all URLs in collection.txt. */
Set getCollection()
{
	FILE *file = fopen("collection.txt", "r");
	if (!file) { perror("fopen failed"); exit(EXIT_FAILURE); }

	// Gets every URL and adds it to set.
	Set URLList = newSet();
	char URL[URL_LENGTH];
	while (fscanf(file, "%s ", URL) != EOF) {
		trim(URL);
		insertInto(URLList, URL);
	}
	fclose(file);
	return URLList;
}


/* Places section 1 and section 2 of fileName into urls & texts */
void readPage(char *urls, char *text, char *fileName)
{
	int i;

	int seen = 0;
	char line[MAX_LINE] = {0};
	FILE *page = fopen(fileName, "r");
	while (fgets(line, MAX_LINE, page) != NULL) {
		// Increments seen at every start tag.
		if (strncmp(line, "#start Section-1", START_TAG_LEN) == 0 
		|| strncmp(line, "#start Section-2", START_TAG_LEN) == 0) { seen++; continue; }
		// Ignores end tags and nwln.
		if (strncmp(line, "#end Section-1", END_TAG_LEN) == 0
		|| strncmp(line, "#end Section-2", END_TAG_LEN) == 0
		|| strncmp(line, "\n", CHAR_LEN) == 0) continue;

		if (seen == SEEN_ONCE) { strcat(urls, line); }
		if (seen == SEEN_TWICE) { strcat(text, line); }
	}
	// Changes '\n's into space.
	for (i = 0; text[i] != '\0'; i++) if (text[i] == '\n') text[i] = ' ';
	for (i = 0; urls[i] != '\0'; i++) if (urls[i] == '\n') urls[i] = ' ';
	fclose(page);
}


/* Calculates space required for section 1 and 2 */
void spaceRequired(char *fileName, int *url_size, int *text_size)
{
	int seen = 0;
	char line[MAX_LINE] = {0};
	FILE *page = fopen(fileName, "r");
	*url_size = NULL_TERM_SPACE;
	*text_size = NULL_TERM_SPACE;
	while (fgets(line, MAX_LINE, page) != NULL) {
		if (strncmp(line, "#start Section-1", START_TAG_LEN) == 0 
		|| strncmp(line, "#start Section-2", START_TAG_LEN) == 0) { seen++; continue; }
		if (strncmp(line, "#end Section-1", END_TAG_LEN) == 0
		|| strncmp(line, "#end Section-2", END_TAG_LEN) == 0
		|| strncmp(line, "\n", CHAR_LEN) == 0) continue;
		// if in section 1 incremeent the size required to store the URLS
		if (seen == SEEN_ONCE) *url_size = *url_size + strlen(line);
		// if in section 2 incremeent the size required to store the text
		if (seen == SEEN_TWICE) *text_size = *text_size + strlen(line);
	}
	fclose(page);
}


/* Creates a list of url for each word found in urls. */
BSTree getInvertedList(Set URLList)
{
	BSTree invList = newBSTree();
	char fileName[URL_LENGTH] = {0};

	// Iterate through set to get urls.
	SetNode curr = URLList->elems;
	while (curr != NULL) {
		sprintf(fileName, "%s.txt", curr->val);
		// Gets information from txt file.
		int url_size; int text_size;
		spaceRequired(fileName, &url_size, &text_size);
		char *urls = calloc(url_size, sizeof(char));
		char *text = calloc(text_size, sizeof(char));
		readPage(urls, text, fileName);

		char *dump = text; // Keeps pointer to text to free because strsep ruins it.
		char *found;
		// For every word in every url.
		while((found = strsep(&text, " ")) != NULL) {
			char *word = normalise(found);
			if (strcmp(word, "") != 0)
				invList = BSTreeInsert(invList, word, curr->val);
			free(word);
		}
		free(dump); free(urls); free(text); free(found);
		curr = curr->next;
	}
	return invList;
}

// check if an link to that node already exists
int linkAlreadyExists(Link start, char *name) {
	Link curr = start;
	for (; curr != NULL; curr = curr->next) {
		if (strcmp(curr->URLName, name) == 0) return TRUE;
	}
	return FALSE;
}

/* Creates a graph of URLs. */
Graph getGraph(Set URLList)
{
	int j;
	Graph g = newGraph();
	g->listOfUrls = malloc(sizeof(URL) * URLList->nelems);
	char fileName[URL_LENGTH] = {0};
	int i = 0;
	SetNode curr;
	for (curr = URLList->elems; curr != NULL; curr = curr->next) {
		strcpy(fileName, curr->val);
		strcat(fileName, ".txt");
		int url_size; int text_size;
		// reading in url outlinks and text from webpage (textfile)
		spaceRequired(fileName, &url_size, &text_size);
		char *urls = calloc(url_size + 1, sizeof(char));
		char *text = calloc(text_size + 1, sizeof(char));
		readPage(urls, text, fileName);
		trim(urls); trim(text);
		g->listOfUrls[i] = newGraphNode(curr->val, text);
		//insert outlinks
		if (strlen(urls) != 0) {
			char **urlsTokenised = tokenise(urls, " ");
			for (j = 0; urlsTokenised[j] != NULL; j++) {
				// dont add an outlink to itself, no loops
				if (strcmp(g->listOfUrls[i]->URLName, urlsTokenised[j]) == 0) continue;
				// dont add another link that already exists, no parallel edges
				if (linkAlreadyExists(g->listOfUrls[i]->outLink, urlsTokenised[j])) continue;
				insertOutLinks(g->listOfUrls[i], urlsTokenised[j]);
				g->listOfUrls[i]->numOutLinks++;
			}
			freeTokens(urlsTokenised);
		}
		free(urls); free(text);
		i++;
		g->numURLs++;
	}
	// for each node
	for (i = 0; i < g->numURLs; i++) {
		// go through its outlinks
		for (Link curr = g->listOfUrls[i]->outLink; curr != NULL; curr = curr->next) {
			// set the outlink pointer to point to an actual node
			for (j = 0; j < g->numURLs; j++) {
				if (strcmp(g->listOfUrls[j]->URLName, curr->URLName) == 0) {
					curr->URLPointer = g->listOfUrls[j];
				}
			}
		}
	}

	// insert inLinks
	// for each node
	for (i = 0; i < g->numURLs; i++) {
		// go through all the other nodes
		for (j = 0; j < g->numURLs; j++) {
			// go through their outlinks
			if (strcmp(g->listOfUrls[i]->URLName, g->listOfUrls[j]->URLName) == 0) continue;
			for (Link curr = g->listOfUrls[j]->outLink; curr != NULL; curr = curr->next) {
				// if they have an outlink to the og node, there should be an outlink for the og node
				if (strcmp(g->listOfUrls[i]->URLName, curr->URLName) == 0) {
					insertInLinks(g->listOfUrls[i], g->listOfUrls[j]);
					g->listOfUrls[i]->numInLinks++;
				}
			}
		}
	}
	return g;
}