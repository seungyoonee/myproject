#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "mylist.h"
#include "mdb.h"

static void die(const char *s) { perror(s); exit(1); }

int main(int argc, char **argv)
{
	// ignore SIGPIPE so that we don't terminate when we call
	// send() on adisconnected socket.
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		die("signal() failed");

	if (argc != 3) {
		fprintf(stderr, "usage: %s <database_file> <Server Port>\n", argv[0]);
		exit(1);
	}

	const char *filename = argv[1];
	unsigned short port = atoi(argv[2]);

	// Create a listening socket (also called server socket) 

	int servsock;
	if ((servsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		die("socket failed");

	// Construct local address structure

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // any network interface
	servaddr.sin_port = htons(port);
	
	// Bind to the local address

	if (bind(servsock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		die("bind failed");

	// Start listening for incoming connections

	if (listen(servsock, 5 /* queue size for connection requests */ ) < 0)
		die("listen failed");

	FILE *input;

	int clntsock;
	socklen_t clntlen;
	struct sockaddr_in clntaddr;

	while (1) {

		// Accept an incoming connection

		clntlen = sizeof(clntaddr); // initialize the in-out parameter

		if ((clntsock = accept(servsock, (struct sockaddr *) &clntaddr, &clntlen)) < 0)
			die("accept failed");

		// accept() returned a connected socket (also called client socket)
		// and filled in the client's address into clntaddr
		
		fprintf(stderr, "connection started from: %s\n", inet_ntoa(clntaddr.sin_addr));

		// wrap the socket file descriptor with a FILE* using fdopen()
		
		if((input = fdopen(clntsock, "r")) == NULL)
			die("fdopen returned NULL");

		// open the database file
		
		FILE *fp = fopen(filename, "rb");
		if(fp == NULL)
			die(filename);

		// read all records into memory

		struct List list;
		initList(&list);

		int loaded = loadmdb(fp, &list);
		if(loaded < 0)
			die("loadmdb");

		fclose(fp);

		// lookup loop

		char line[1000];
		char key[6];

		while(fgets(line, sizeof(line), input) != NULL) {
			
			strncpy(key, line, sizeof(key) - 1);
			key[sizeof(key) - 1] = '\0';

			size_t last = strlen(key) - 1;
			if(key[last] == '\n')
				key[last] = '\0';

			struct Node *node = list.head;
			int recNo = 1;

			while(node) {
				struct MdbRec *rec = (struct MdbRec *)node->data;
				int bytesSending = 0;
				char buf[100] = "";
				if(strstr(rec->name, key) || strstr(rec->msg, key)) {
					bytesSending = snprintf(buf + bytesSending,
							sizeof("%4d: ") + 1,
							"%4d: ", recNo);
					bytesSending += snprintf(buf + bytesSending,
							sizeof(rec->name) + sizeof("{%s} said "),
							"{%s} said ", rec->name);
					bytesSending += snprintf(buf + bytesSending,
							sizeof(rec->msg) + sizeof("{%s}\n"),
							"{%s}\n", rec->msg);
				}
				if(send(clntsock, buf, bytesSending, 0) != bytesSending) {
					fprintf(stderr, "ERR: send failed\n");
					break;
				}
				node = node->next;
				recNo++;
			}
			send(clntsock, "\n", sizeof("\n"), 0);
		}
		if(ferror(input)) {
			die("input");
		}

		// Client closed the connection (r==0) or there was an error
		// Either way, close the client socket and go back to accept()

		fprintf(stderr, "connection terminated from: %s\n", inet_ntoa(clntaddr.sin_addr));
		freemdb(&list);
		fclose(input);
		close(clntsock);
	}
}
