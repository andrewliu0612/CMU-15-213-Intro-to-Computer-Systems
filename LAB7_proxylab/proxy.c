#include <stdio.h>

#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400


/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";


/* Forward declarations */
void process_request(int fd);
int parse_uri(char *uri, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);
void read_requesthdrs(rio_t *rp);



/* Start server main routine */
int main(int argc, char **argv) {
    
    int listenfd, connfd;                   /* File descriptors */
    struct sockaddr_storage clientaddr;     /* Client info */
    socklen_t clientlen;                    /* Client info length */
    char hostname[MAXLINE], port[MAXLINE];

    /* Check command line args */
    if(argc != 2) {
        fprintf(stderr, "usage: %s <port> \n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);

    /* Main loop */
    while(1) {

        /* Receive for browser request */
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);

        /* Process this single HTTP request */
        process_request(connfd);
    }
}
/* End server main routine */



/* process - Process one HTTP proxy request.
 */
void process_request(int clientfd) {

    rio_t rio;
    int serverfd;           /* fd for host server */ 
    char buf_old[MAXLINE], buf_new[MAXLINE];            /* Request buffer */
    char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE], port[MAXLINE];              /* Buffer for establishing new connection */
    
    /* Read request line and headers */
    Rio_readinitb(&rio, clientfd);
    if (!Rio_readlineb(&rio, buf_old, MAXLINE))
        return;
    printf("%s", buf_old);
    sscanf(buf_old, "%s %s %s", method, uri, version);
    if (strcasecmp(method, "GET")) {            /* Method should only be GET */
        clienterror(clientfd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }

    /* Read HTTP request body. Doesn't actually do anything, just clears buffer. */
    read_requesthdrs(&rio);

    /* Rebuild HTTP request. Write new request to buffer */
    rebuild_request(buf_old, buf_new);

    /* Create new connfd for host server */
    parse_uri(uri, hostname, port);
    serverfd = Open_clientfd(hostname, port);
}


void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) {
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}


/* read_requesthdrs - Read HTTP request body.
 */
void read_requesthdrs(rio_t *rp) {
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    while(strcmp(buf, "\r\n")) {
        Rio_readlineb(rp, buf, MAXLINE);
        // printf("%s", buf);
    }
    return;
}


/* rebuild_request - Rebuild HTTP request given the old HTTP request header:
 *      Method: Unchanged
 *      URI: Remove hostname from URI
 *      Version: Change to HTTP/1.0
 *      
 *      Host header: Copy from URI
 *      User-Agent: Mozilla 5.0
 *      Connection: close
 *      Proxy-Connection: close
 */
void rebuild_request(const char *buf, char *result) {
    
    char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char *cursor;

    /* Split into method, uri, version */
    sscanf(buf, "%s %s %s", method, uri, version);

    /* Split uri into hostname/path */
    assert(strncmp(uri, "http://", strlen("http://")) == 0);    /* Should begin with http:// */
    cursor = uri + strlen("http://");
    while(*cursor != '/')
        cursor++;
    

    /* Fill into result array */
    sprintf(result, "%s", method);
    sprintf(result, "%s%s", result, cursor);
    sprintf(result, "%s%s\r\n", result, "HTTP/1.0");
    *cursor = 0; sprintf(result, "%sHost: %s\r\n", result, uri + strlen("http://"));
    sprintf(result, "%s%s", result, user_agent_hdr);
    sprintf(result, "%sConnection: close\r\n", result);
    sprintf(result, "%sProxy-Connection: close\r\n", result);

}


/* parse_uri - Get the hostname and port of the given URI. The URI may
 *      begin with "https://", and may contain a port number. 
 *      If no port number is given, return the default HTTP port of 80.
 */
void parse_uri(const char *uri, char *hostname, char *port) {

    char *hostname_cursor, *port_cursor;
    
    /* Move hostname cursor to hostname */
    if(strncmp(uri, "http://", strlen("http://")) == 0)
        hostname_cursor = uri + strlen("http_uri");
    
    /* Move port cursor to port */
    port_cursor = hostname_cursor;
    while(*port_cursor && *port_cursor != ':')
        port_cursor++;
    port_cursor++;

    /* Fill hostname */
    memset(hostname, 0, MAXLINE * sizeof(char));
    strncpy(hostname, hostname_cursor, port_cursor - hostname_cursor - 1);

    /* Fill port */
    memset(port, 0, MAXLINE * sizeof(char));
    if(*port_cursor)                /* URI contains port */
        strcpy(port, port_cursor);
    else
        strcpy(port, "80");         /* Default HTTP port */
}