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

int main(int argc, char **argv)
{
	if (argc != 4) {
		fprintf(stderr, "usage: %s <host name> <port number> <file path>\n", argv[0]);
		fprintf(stderr, "   ex) http-client www.example.com 80 /index.html\n");
		exit(1);
	}

	// convert a host name into an IP address in dotted-quad notation
	//
	struct hostent *he;
	char *serverName = argv[1];

	// get server ip from server name
	//
	if ((he = gethostbyname(serverName)) == NULL) {
		die("gethostbyname failed");
	}
	char *ip = inet_ntoa(*(struct in_addr *)he->h_addr);

	unsigned short port = atoi(argv[2]);
	const char *filePath = argv[3];

	// pick a filename from the file path

	char *filename = strrchr(filePath, '/') + 1;

	// Create a socket for TCP connection

	int sock; // socket descriptor
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		die("socket failed");

	// Construct a server address structure

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr)); // must zero out the structure
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port        = htons(port); // must be in network byte order

	// Establish a TCP connection to the server

	if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		die("connect failed");

	// wrap the socket file descriptor with a FILE* using fdopen()
	
	FILE *input;

	if((input = fdopen(sock, "r")) == NULL)
		die("fdopen()");

	// send a request line

	int bytesSending = 0;
	char requestLine[1000];

	bytesSending = snprintf(requestLine + bytesSending, sizeof("GET "), "%s", "GET ");
	bytesSending += snprintf(requestLine + bytesSending, strlen(filePath) + 1, "%s",  filePath);
	bytesSending += snprintf(requestLine + bytesSending, sizeof(" HTTP/1.0\r\n"), "%s", " HTTP/1.0\r\n");
	bytesSending += snprintf(requestLine + bytesSending, sizeof("Host: "), "%s", "Host: ");
	bytesSending += snprintf(requestLine + bytesSending, strlen(serverName) + 2, "%s:", serverName);
	bytesSending += snprintf(requestLine + bytesSending, sizeof(unsigned short) + sizeof("\r\n\r\n"), "%d\r\n\r\n", port);

	send(sock, requestLine, bytesSending, 0);

	// Read responses until a blank line and detect if status code is not 200

	int counter = 0;
	char buf[4096];
	char *success = "200";

	while(1) {
		if(fgets(buf, sizeof(buf), input) == NULL)
			die("fgets returned NULL");

		if(counter == 0 && strncmp(buf + 9, success, 3) != 0) {
			printf("%s\n", buf);
			exit(1);
		}

		if(strcmp(buf, "\r\n") == 0)
			break;

		counter++;
	}

	FILE *fp;
	if((fp = fopen(filename, "wb")) == NULL)
			die(filename);

	unsigned int n;

	while((n = fread(buf, 1, sizeof(buf), input)) > 0) {
		fwrite(buf, 1, n, fp);
	}
	if(ferror(input))
		die("fread failed");

	// Clean-up

	fclose(fp);
	fclose(input);
	close(sock);
	return 0;
}
