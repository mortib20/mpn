#ifndef _MPN_H
#define _MPN_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>

#define MPN_ERRNO errno

/* IP versions */
#define MPN_IPV4 AF_INET
#define MPN_IPV6 AF_INET6

/* Socket types */
#define MPN_SOCK_STREAM SOCK_STREAM
#define MPN_SOCK_DGRAM SOCK_DGRAM

/* Max connections */
#define MPN_MAXCON 10

/* Protocols */
#define MPN_PROTO_TCP IPPROTO_TCP
#define MPN_PROTO_UDP IPPROTO_UDP

#define MPN_ADDR4_ANY INADDR_ANY
#define MPN_ADDR4_LOOPBACK INADDR_LOOPBACK
#define MPN_ADDR4_BROADCAST INADDR_BROADCAST

#define MPN_ADDR6_ANY in6addr_any
#define MPN_ADDR6_LOOPBACK in6addr_loopback

typedef size_t mpn_socket_t;
typedef unsigned int mpn_socklen_t;
typedef struct sockaddr mpn_sockaddr;
typedef struct sockaddr_in mpn_sockaddr_in;
typedef struct sockaddr_in6 mpn_sockaddr_in6;
typedef struct sockaddr_storage mpn_sockaddr_storage;



/*********************************************
**                Error Handling             *
**********************************************/

/* Error types */
enum mpn_error_types_e {
	MPN_ERROR_OK,
	MPN_ERROR_SOCKET,
	MPN_ERROR_BIND,
	MPN_ERROR_CONNECT
};

/* Holding information about error */
typedef struct mpn_error_struct {
	enum mpn_error_types_e type;
	char* text;
} *mpn_error;

/* Create a new error object */
mpn_error mpn_error_new(void);

/* Free a error object */
void mpn_error_free(mpn_error error);

/* Set error */
void mpn_error_set(mpn_error error, enum mpn_error_types_e type);

/* Returns last error */
char* mpn_error_get(mpn_error error);



/*********************************************
**                Socket                     *
**********************************************/

/* Socket flags */
enum mpn_socket_options_e {
	MPN_SOCKET_OPTIONS_FAMILY,
	MPN_SOCKET_OPTIONS_SOCKTYPE,
	MPN_SOCKET_OPTIONS_PROTOCOL,
	MPN_SOCKET_OPTIONS_MAXCON,
	MPN_SOCKET_OPTIONS_LADDRESS,
	MPN_SOCKET_OPTIONS_LPORT,
	MPN_SOCKET_OPTIONS_RADDRESS,
	MPN_SOCKET_OPTIONS_RPORT
};

/* Holding information of a socket */
typedef struct mpn_socket_struct {
	mpn_socket_t fd;    // Socket Descriptor
	int family;         // MPN_IPV4 | MPN_IPV6
	int socktype;       // MPN_SOCK_STREAM | MPN_SOCK_DGRAM
	int protocol;       // MPN_PROTO_TCP | MPN_PROTO_UDP
	int maxcon;			// Max connections for listening
	
	/* local */
	char* laddress;
	char* lport;
	mpn_sockaddr *local_sockaddr;
	mpn_socklen_t local_sockaddr_len;

	/* remote */
	char* raddress;
	char* rport;
	mpn_sockaddr *remote_sockaddr;
	mpn_socklen_t remote_sockaddr_len;
} *mpn_socket;

/* Create a new socket object */
mpn_socket mpn_socket_new();

/* Accept a new client */
mpn_socket mpn_socket_accept(mpn_socket Socket);

/* Close a socket object socket */
void mpn_socket_close(mpn_socket Socket);

/* Free a socket object */
void mpn_socket_free(mpn_socket Socket);

/* Bind session to local address */
int mpn_socket_bind(mpn_socket Socket);

/* Connect to address */
int mpn_socket_connect(mpn_socket Socket);

/* Listen on a socket */
int mpn_socket_listen(mpn_socket Socket);

/* Set a socket option */
int mpn_socket_options_set(mpn_socket Socket, enum mpn_socket_options_e option, void *value);

/* Set socket nonblocking */
int mpn_socket_nonblocking(mpn_socket Socket);

/* Set socket to reuse address */
int mpn_socket_reuseaddr(mpn_socket Socket);

/* Get information about address */
void mpn_socket_getaddrinfo(mpn_socket Socket);



/*********************************************
**                Session                    *
**********************************************/

enum mpn_options_e {
	MPN_OPTIONS_TEST
};

/* Flags for the session */
enum mpn_session_flags_e {
	MPN_SESSION_CLIENT = 0,
	MPN_SESSION_SERVER = 1,
};

/* Holds information of a session */
typedef struct mpn_session_struct {
	/* session */
	uint32_t session_flags;
	/* socket */
	mpn_socket socket;
	/* error */
	mpn_error error;
} *mpn_session;

/* Initialize a new server session */
mpn_session mpn_server_new(void);

/* Initialize a new client session */
mpn_session mpn_client_new(void);

/* Closing the session */
void mpn_close(mpn_session session);

/* Free mpn_session object */
void mpn_free(mpn_session session);

/* Set Options of session */
int mpn_options_set(mpn_session session, enum mpn_options_e type, const void *value);



/*********************************************
**                Server specific            *
**********************************************/

/* Server start listening */
int mpn_listen(mpn_session session);



/*********************************************
**                Client specific            *
**********************************************/

/* Client connect */
int mpn_connect(mpn_session session);



#endif