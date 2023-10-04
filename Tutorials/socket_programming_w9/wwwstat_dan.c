/*Header files in programming contain function declarations, constants, macro definitions, and data structure declarations that are used by other files in a program. They provide information about the available functions and data types that can be used in a program.

Header files are typically included at the beginning of a source code file using the #include preprocessor directive. By including the necessary header files, the compiler knows the function signatures, constants, and data types that will be used in the program and can perform proper type checking and compilation.

Header files can be system-provided or user-defined. System-provided header files are part of the programming language or libraries, and they provide fundamental functionality. Examples include <stdio.h> for standard input/output operations, <stdlib.h> for memory allocation and utilities, <string.h> for string manipulation functions, and <sys/socket.h> for socket programming.

User-defined header files are created by the programmer and contain custom function declarations, macros, and structures that are used in their program. These files are often created to group related functions or to separate interface declarations from the implementation details.

Overall, header files make it easier to organize code, share code between multiple files, and ensure that proper definitions and declarations are available for the code to compile and function correctly.


    <netinet/in.h>: This header file provides constants, data structures, and function prototypes for working with Internet addresses and network connectivity. It includes declarations related to IP addresses, socket addresses, and network protocols.

    <arpa/inet.h>: This header file contains functions for converting IP addresses between different formats, such as from binary to string representation (IPv4 and IPv6) and vice versa. It also provides functions for network byte order conversion (byte swapping) and checking IP address validity.

    <sys/socket.h>: This header file includes definitions and functions related to socket programming. It provides the necessary structures, constants, and function prototypes for creating, binding, connecting, and manipulating sockets.

    <netdb.h>: This header file contains data structures and functions for performing DNS (Domain Name System) lookups and working with hostnames and network services. It includes functions like gethostbyname() for resolving hostnames to IP addresses and getaddrinfo() for more advanced DNS lookups.

    <errno.h>: This header file declares the errno variable and provides constants and functions related to error handling. It allows you to access the error codes set by various functions and use them to identify and handle specific errors during runtime.

These header files are typically included when working with network-related operations, such as socket programming, network address conversions, host lookups, and error handling in C.
*/

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

