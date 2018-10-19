/*
 * 
 * NOTES:
 * - Use adjacency list.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "graph.h"




struct urlLink *newGraphLinks(char *urlNum)
{
    struct urlLink *newLink = calloc(1, sizeof(struct urlLink));
    newLink->URLName = malloc(strlen(urlNum)+1);
    newLink->URLName = strdup(urlNum);
    newLink->next = NULL;
    return newLink;
}

struct urlNode *newGraphNode(char *urlNum, char *text)
{
    struct urlNode *newURL = calloc(1, sizeof(struct urlNode));
    newURL->URLName = malloc(strlen(urlNum)+1);
    newURL->URLName = strdup(urlNum);
    newURL->numOutLinks = 0;newURL->numInLinks = 0;
    newURL->text = malloc(strlen(text)+1);
    newURL->text = strdup(text);
    newURL->inLink = NULL;
    newURL->outLink = NULL;
    return newURL;
}

struct urlGraph *newGraph()
{
    struct urlGraph *newGraph = calloc(1, sizeof(struct urlGraph));
    newGraph->numURLs = 0;
    newGraph->listOfUrls = NULL;
    return newGraph;
}

void insertOutLinks(URL URLNode, char *URL)
{
    if (URLNode->outLink == NULL) {
        URLNode->outLink = newGraphLinks(URL);
    } else {
        Link curr = URLNode->outLink;
        while (curr->next != NULL) curr = curr->next;
        curr->next = newGraphLinks(URL);
    }
}


void insertInLinks(URL URLNode, char *URL)
{
    if (URLNode->inLink == NULL) {
        URLNode->inLink = newGraphLinks(URL);
    } else {
        Link curr = URLNode->inLink;
        while (curr->next != NULL) curr = curr->next;
        curr->next = newGraphLinks(URL);
    }
}


/*
void insertInto(Set s, char *str)
{
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if (found) return; // already in Set
	Link new = newNode(str);
	s->nelems++;
	if (prev == NULL) {
		// add at start of list of elems
		new->next = s->elems;
		s->elems = new;
	}
	else {
		// add into list of elems
		new->next = prev->next;
		prev->next = new;
	}
*/