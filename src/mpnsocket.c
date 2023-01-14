#include "../include/mpn.h"

/* Create a new socket */
mpn_socket mpn_socket_new()
{
	mpn_socket Socket = calloc(0, sizeof(mpn_socket));

	Socket->family = MPN_IPV4;
	Socket->socktype = MPN_SOCK_STREAM;
	Socket->protocol = MPN_PROTO_TCP;
	Socket->maxcon = MPN_MAXCON;
	asprintf(&Socket->laddress, "0.0.0.0");
	Socket->lport = 0;

	return Socket;
}

/* Accept a new client */
mpn_socket mpn_socket_accept(mpn_socket Server)
{
	return NULL;
}

/* Close a socket object */
void mpn_socket_close(mpn_socket Socket)
{
	close(Socket->fd);
}

/* Free a socket object */
void mpn_socket_free(mpn_socket Socket)
{
	free(Socket);
}

/* Bind to address and port */
int mpn_socket_bind(mpn_socket Socket)
{
	if(Socket->family == MPN_IPV4) {
		mpn_sockaddr_in address;
		address.sin_family = MPN_IPV4;
		address.sin_addr.s_addr = inet_addr(Socket->laddress);
		address.sin_port = htons(atoi(Socket->lport));
	} else if(Socket->family == MPN_IPV6) {
		mpn_sockaddr_in6 address;
		address.sin6_family = MPN_IPV6;
		address.sin6_addr = inet_addr(Socket->laddress); // Change to inet_pton
		address.sin6_port = htons(atoi(Socket->lport));
	}

	bind(Socket->fd, )
}

/* Connect to address and port */
int mpn_socket_connect(mpn_socket Socket)
{	
	Socket->fd = socket(Socket->family, Socket->socktype, Socket->protocol);
	if (Socket->fd == -1) {
		perror("Socket");
	}

	return connect(Socket->fd, Socket->remote_sockaddr, Socket->remote_sockaddr_len);	
}

int mpn_socket_listen(mpn_socket Socket)
{
	return 0;
}

int mpn_socket_options_set(mpn_socket Socket, enum mpn_socket_options_e option, void *value)
{
	if(option == MPN_SOCKET_OPTIONS_FAMILY) {
		int *family = (int*)value;
		Socket->family = *family;
		return 0;
	}
	if(option == MPN_SOCKET_OPTIONS_SOCKTYPE) {
		const int *socktype = (int*)value;
		Socket->socktype = *socktype;
		return 0;
	}
	if(option == MPN_SOCKET_OPTIONS_PROTOCOL) {
		const int *protocol = (int*)value;
		Socket->protocol = *protocol;
		return 0;
	}
	if(option == MPN_SOCKET_OPTIONS_MAXCON) {
		const int *maxcon = (int*)value;
		Socket->maxcon = *maxcon;
		return 0;
	}
	if(option == MPN_SOCKET_OPTIONS_LADDRESS) {
		char* laddress = value;
		asprintf(&Socket->laddress, "%s", laddress);
		return 0;
	}
	if(option == MPN_SOCKET_OPTIONS_LPORT) {
		char* lport = value;
		asprintf(&Socket->lport, "%s", lport);
		return 0;
	}
	if(option == MPN_SOCKET_OPTIONS_RADDRESS) {
		char* raddress = value;
		asprintf(&Socket->raddress, "%s", raddress);
		return 0;
	}
	if(option == MPN_SOCKET_OPTIONS_RPORT) {
		char* rport = value;
		asprintf(&Socket->rport, "%s", rport);
		return 0;
	}

	return 0;
}

void mpn_socket_getaddrinfo(mpn_socket Socket)
{
	int rc;
	struct addrinfo hints, *address;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = Socket->family;
	hints.ai_socktype = Socket->socktype;

	rc = getaddrinfo(Socket->raddress, Socket->rport, &hints, &address);
	if (rc != 0) {
		perror("mpn_socket_getaddrinfo");
		fprintf(stderr, "- %s\n", gai_strerror(rc));
		mpn_socket_free(Socket);
		exit(-1);
	}

	Socket->remote_sockaddr = address->ai_addr;
	Socket->remote_sockaddr_len = address->ai_addrlen;

	free(address);
}