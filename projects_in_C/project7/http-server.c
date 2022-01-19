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
#include <netdb.h>

static void die(const char *s) { perror(s); exit(1); }

static void printUsage() {
	fprintf(stderr, "usage: ./http-server <server_port> <web_root> <mdb-lookup-host> <mdb-lookup-port>\n");
	exit(1);
}

static void recoverable(FILE *input, int clntsock) {
	fclose(input);
	close(clntsock);
}

int main(int argc, char **argv)
{
	// ignore SIGPIPE so that we don't terminate when we call
	// send() on a disconnected socket.
	
	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		die("signal() failed");

	if(argc != 5) {
		printUsage();
	}

	unsigned short s_port = atoi(argv[1]);
	const char *htmlPath = argv[2];
	const char *serverName = argv[3];
	unsigned short mdb_port = atoi(argv[4]);
	char *p = strrchr(htmlPath, '/');
	if(!p)
		printUsage();

	/* ******************************************* *
	 * beginning of mdb-lookup-server construction *
	 * ******************************************* */

	int sock;
	char *serverIP;

	// get mdb-lookup-server ip from server name 
	
	struct hostent *he;
	if((he = gethostbyname(serverName)) == NULL) {
		die("gethostbyname failed");
	}
	serverIP = inet_ntoa(*(struct in_addr *)he->h_addr);

	// create socket for mdb-lookup-server
	
	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		die("socket failed");
	}

	// construct mdb-lookup-server address
	
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(serverIP);
	serverAddr.sin_port = htons(mdb_port);

	// connect
	
	if(connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
		die("connect failed");
	}

	/* ************************************* *
	 * end of mdb-lookup-server construction *
	 * ************************************* */

	/* ************************************* *
	 * beginning of http-server construction *
	 * ************************************* */

	// Create a listening socket (also called server socket)
	
	int servsock;
	if((servsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		die("socket failed");

	// Construct local address structure
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // any network interface
	servaddr.sin_port = htons(s_port);

	// Bind to the local address
	
	if(bind(servsock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		die("bind failed");

	// Start listening for incoming connetions
	
	if(listen(servsock, 5 /*queue size of connection requests */ ) < 0)
		die("listen failed");
	
	/* ******************************* *
	 * end of http-server construction *
	 * ******************************* */

	int clntsock;
	socklen_t clntlen;
	struct sockaddr_in clntaddr;

	FILE *fp;
	char toRead[1000];

	int r;
	char buf[4096];
	uint32_t size, remaining, limit;
	struct stat st;

	FILE *input2 = fdopen(sock, "r");
	if(input2 == NULL)
		die("sock fdopen failed");

	while(1)
	{
		// Accept an incoming connection

		clntlen = sizeof(clntaddr); // intialize the in-out parameter

		if((clntsock = accept(servsock, (struct sockaddr *) &clntaddr, &clntlen)) < 0)
			die("accept failed");

		// accept() returned a connected socket (also called client socket)
		// and filled in the client's address into clntaddr

		FILE *input = fdopen(clntsock, "r");
		if(input == NULL)
			die("clntsock fdopen failed");

		char requestLine[1000];

		char *code200 = "HTTP/1.0 200 OK\r\n\r\n";
		char *code400 = "HTTP/1.0 400 Bad Request\r\n\r\n"
			"<html><body><h1>400 Bad Request</h1></body></html>";
		char *code403 = "HTTP/1.0 403 Forbidden\r\n\r\n"
			"<html><body><h1>403 Forbidden</h1></body></html>";
		char *code404 = "HTTP/1.0 404 Not Found\r\n\r\n"
			"<html><body><h1>404 Not Found</h1><body></html>";
		char *code501 = "HTTP/1.0 501 Not Implemented\r\n\r\n"
			"<html><body><h1>501 Not Implemented</h1></body></html>";

		fgets(requestLine, sizeof(requestLine), input);

		// parse the request

		char *token_separators = "\t \r\n"; // tab, space, new line
		char *method = strtok(requestLine, token_separators);
		char *requestURI = strtok(NULL, token_separators);
		char *httpVersion = strtok(NULL, token_separators);

		char p_requestLine[1000];
		snprintf(p_requestLine, sizeof(p_requestLine), "\"%s %s %s\"", method, requestURI, httpVersion);

		if(method == NULL || requestURI == NULL || httpVersion == NULL) {
			send(clntsock, code501, strlen(code501), 0);
			printf("%s %s 501 Not Implemented\n", inet_ntoa(clntaddr.sin_addr), p_requestLine);
			recoverable(input, clntsock);
			continue;
		}

		// request for mdb-lookup

		char *mdb_lookup = "/mdb-lookup";
		const char *form =
			"<h1>mdb-lookup</h1>\n"
			"<p>\n"
			"<form method=GET action=/mdb-lookup>\n"
			"lookup: <input type=text name=key>\n"
			"<input type=submit>\n"
			"</form>\n"
			"<p>\n";

		if(strcmp(requestURI, mdb_lookup) == 0) {
			send(clntsock, code200, strlen(code200), 0);
			send(clntsock, form, strlen(form), 0);
			printf("%s %s 200 OK\n", inet_ntoa(clntaddr.sin_addr), p_requestLine);
			recoverable(input, clntsock);
			continue;
		}

		const char *tableOpen =
			"<table border=\"\">\n"
			"<tbody>\n";
		const char *tableClose =
			"</tbody>\n"
			"</table>\n";
		const char *whiteTable1 = "<tr>\n<td>";
		const char *whiteTable2 = "</td>\n</tr>\n";
		const char *yellowTable1 = "<tr>\n<td bgcolor=yellow>";
		const char *yellowTable2 = "</td>\n</tr>\n";

		char *querySubmitted = strstr(requestURI, "/mdb-lookup?key=");

		if(querySubmitted != NULL) {
			char *key = strchr(requestURI, '=') + 1;

			printf("looking up [%.5s]: %s %s 200 OK\n", key, inet_ntoa(clntaddr.sin_addr), p_requestLine);

			key = strcat(key, "\n");
			if(send(sock, key, strlen(key), 0) < 0)
				die("mdb-lookup-server has been disconnected\n");
			send(clntsock, code200, strlen(code200), 0);
			send(clntsock, form, strlen(form), 0);
			send(clntsock, tableOpen, strlen(tableOpen), 0);
			
			int white = 1;
			while(fgets(buf, sizeof(buf), input2) != NULL)
			{
				if(strcmp(buf, "\n") == 0) break;
				if(white) {
					send(clntsock, whiteTable1, strlen(whiteTable1), 0);
					send(clntsock, buf, strlen(buf), 0);
					send(clntsock, whiteTable2, strlen(whiteTable2), 0);
					white = 0;
				} else {
					send(clntsock, yellowTable1, strlen(yellowTable1), 0);
					send(clntsock, buf, strlen(buf), 0);
					send(clntsock, yellowTable2, strlen(yellowTable2), 0);
					white = 1;
				}
			}
			send(clntsock, tableClose, strlen(tableClose), 0);
			recoverable(input, clntsock);
			continue;
		}
		
		// only support the GET method
		// if a browser sends other methods,
		// the server responds with the status code 501

		if(method == NULL || strcmp(method, "GET")) {
			send(clntsock, code501, strlen(code501), 0);
			printf("%s %s 501 Not Implemented\n", inet_ntoa(clntaddr.sin_addr), p_requestLine);
			recoverable(input, clntsock);
			continue;
		}
		
		// reject any other protocol and/or version,
		// responding with 501 status code

		char *v1 = "HTTP/1.0";
		char *v2 = "HTTP/1.1";
		
		if(httpVersion == NULL) {
			send(clntsock, code501, strlen(code501), 0);
			printf("%s %s 501 Not Implemented\n", inet_ntoa(clntaddr.sin_addr), p_requestLine);
			recoverable(input, clntsock);
			continue;
		} else if(strcmp(httpVersion, v1) == 0 || strcmp(httpVersion, v2) == 0) {
		} else {
			send(clntsock, code501, strlen(code501), 0);
			printf("%s %s 501 Not Implemented\n", inet_ntoa(clntaddr.sin_addr), p_requestLine);
			recoverable(input, clntsock);
			continue;
		}

		// check that the request URI starts with "/"
		// if not, respond with 400 status code

		if(strncmp(requestURI, "/", 1) != 0) {
			printf("testing1\n");
			send(clntsock, code400, strlen(code400), 0);
			printf("%s %s 400 Bad Request\n", inet_ntoa(clntaddr.sin_addr), p_requestLine);
			recoverable(input, clntsock);
			continue;
		}

		// make sure that the request URI does not contain "/../"
		// and does not end with "/.."

		char *securityRisk = strstr(requestURI, "..");

		if(securityRisk != NULL) {
			send(clntsock, code400, strlen(code400), 0);
			printf("%s %s 400 Bad Request\n", inet_ntoa(clntaddr.sin_addr), p_requestLine);
			recoverable(input, clntsock);
			continue;
		}

		// open the requested file

		strcpy(toRead, argv[2]);
		strcat(toRead, requestURI);

		stat(toRead, &st);
		if(S_ISDIR(st.st_mode) && strcmp(&toRead[strlen(toRead) -1], "/") != 0)
		{
			send(clntsock, code403, strlen(code403), 0);
			printf("%s %s 403 Forbidden\n", inet_ntoa(clntaddr.sin_addr), p_requestLine);
			recoverable(input, clntsock);
			continue;
		}

		// if the request URI ends with "/", append "index.html" to it

		if(strcmp(&toRead[strlen(toRead) - 1], "/") == 0)
			strcat(toRead, "index.html");

		// send 404 Not Found if cannot open the file

		if((fp = fopen(toRead, "rb")) == NULL)
		{
			send(clntsock, code404, strlen(code404), 0);
			printf("%s %s 404 Not Found\n", inet_ntoa(clntaddr.sin_addr), p_requestLine);
			recoverable(input, clntsock);
			continue;
		}

		// the file successfully opened

		send(clntsock, "HTTP/1.0 200 OK\r\n\r\n", sizeof("HTTP/1.0 200 OK\r\n\r\n") - 1, 0);
		printf("%s %s 200 OK\n", inet_ntoa(clntaddr.sin_addr), p_requestLine);

		// send the file

		stat(toRead, &st);
		size = st.st_size;
		remaining = size;
		while(remaining > 0)
		{
			limit = remaining > sizeof(buf) ? sizeof(buf) : remaining;
			r = fread(&buf, 1, limit, fp);
			remaining -= r;
			send(clntsock, buf, r, 0);
		}

		// Client closed the connection (r==0) or there was an error
		// Either way, close the client socket and go back to accept()

		fclose(fp);
		fclose(input);
		close(clntsock);
	}
	close(sock);
	return 0;
}
