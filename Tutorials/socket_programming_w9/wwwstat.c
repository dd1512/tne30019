#include <stdio.h>
#include <stdlib.h>

#define __USE_GNU
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <errno.h>

#define WWW_PORT	80

//--------------------------------------------------------------------------
// Print an error message to stderr and terminate the program immediately
void
printerror(char *pcErrStr)
{
    fprintf(stderr, "ERROR: %s\n\n", pcErrStr);
    exit(1);
}

//--------------------------------------------------------------------------
// Input parameters are the name of the server to query and a pointer to a
// sockaddr_in structure to fill.  The function performs a lookup of the
// specified Internet name and stores the address along with the web port
// number (WWW_PORT) in the provided sockaddr_in structure
void
FillAddress(char *pcURL, struct sockaddr_in *psAddress)
{
    struct hostent      *psHostDetails;

    if ((psHostDetails = gethostbyname(pcURL)) == NULL) printerror("Can't determine address of website");

    memcpy(&(psAddress->sin_addr), psHostDetails->h_addr_list[0], 4);
    psAddress->sin_family = AF_INET;
    psAddress->sin_port = htons(WWW_PORT);

    printf("Server details:\n");
    printf("  Server:\t%s\n", pcURL);
    printf("  IP Address:\t%s\n", inet_ntoa(psAddress->sin_addr));
}

//--------------------------------------------------------------------------
int
main(int iArgC, char **ppcArgV)
{
    struct sockaddr_in  sAddress;
    char                *pcWWWServer, *pcPage, *pcPos;

    if (iArgC != 2) printerror("Command Line requires one parameter = website");

    // If the provided URL begins with "http://" then set pcWWWServer to point
    // to the next character otherwise set it to point to the entire provided URL
    if (pcWWWServer = strstr(ppcArgV[1], "http://")) pcWWWServer+= 7; else pcWWWServer = ppcArgV[1];

    // If there is a "/" in the URL (after the http:// has been stripped) then 
    // replace it with a 0 so the string terminates as just the name.  Create 
    // a new string pointer (pcPage) to be a copy of the remaining string OR
    // just "/" if no page details have been provided
    if (pcPos = strstr(pcWWWServer, "/"))
    {
        pcPage = strdup(pcPos);
        pcPos[0] = 0;
    } else
        pcPage = strdup("/");

    // Call FillAddress() to fill sAddress with the IP address details (and port
    // number 80) of the requested URL
    FillAddress(pcWWWServer, &sAddress);

    // Put rest of code here

    return 0;
}
