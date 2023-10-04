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
    
    // Construct HTTP 1.0 GET request
    char    pcHttpGet[200];
    sprintf(pcHttpGet, "GET %s HTTP/1.0\n\n", pcPage);
    
    // Create socket and onnect to Web Server
    int iSocket;
    
    if ((iSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "ERROR: Cannot create socket\n");
        exit(1);
    }

    if (connect(iSocket, (struct sockaddr *) &sAddress, sizeof(sAddress)) < 0)
    {
        fprintf(stderr, "ERROR: Unable to bind to port\n");
        exit(1);
    }

    // Send GET Request
    if (send(iSocket, (void *) pcHttpGet, strlen(pcHttpGet), 0) != strlen(pcHttpGet))
    {
        fprintf(stderr, "ERROR: Did not send whole GET request: %s\n", pcHttpGet);
        exit(1);
    }
    
    // Variables to store read bytes and the file pointer to the save file
    char    pcBuffer[1024];
    int     iBytesRead;
    FILE    *fSaveFile;
    
    if ((fSaveFile = fopen("saved.html", "w")) == NULL)
    {
        fprintf(stderr, "ERROR: Unable to open save file\n");
        exit(1);
    }

    // Loop reading all data and saving to file
    while (iBytesRead = recv(iSocket, (void *)pcBuffer, 1024, 0))
    {
        pcBuffer[iBytesRead] = 0;
        fwrite((void *) pcBuffer, iBytesRead, 1, fSaveFile);
    }

    fclose(fSaveFile);
    
    return 0;
}
