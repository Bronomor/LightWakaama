#include <liblwm2m.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

/*
* Create MOCK in SendMessage/core/packet.c ----------> message_send_mock_server
* Create MOCK in SendMessage/core/liblwm2m.h ----------> message_send_mock_server , bottom of file
*
* /////////////////////////// OWN MOCK_SERVER /////////////////////////////
* uint8_t message_send_mock_server(coap_packet_t * message);
*
* message_send_mock_server
*/

/*
* From wakamma/exmaples/shared ------> platfrom.c
*
*/

void * lwm2m_malloc(size_t s)
{
    return malloc(s);
}

void lwm2m_free(void * p)
{
    free(p);
}

char * lwm2m_strdup(const char * str)
{
    if (!str) {
      return NULL;
    }

    const int len = strlen(str) + 1;
    char * const buf = lwm2m_malloc(len);

    if (buf) {
      memset(buf, 0, len);
      memcpy(buf, str, len - 1);
    }

    return buf;
}

int lwm2m_strncmp(const char * s1,
                     const char * s2,
                     size_t n)
{
    return strncmp(s1, s2, n);
}

time_t lwm2m_gettime(void)
{
    return time(NULL);
}

/*
* From wakamma/exmaples/shared ------> connection.h
*
*/

typedef struct _connection_t
{
    struct _connection_t *  next;
    int                     sock; 
    int                     addr; // struct sockaddr_in6
    size_t                  addrLen;
} connection_t;

/*
* From wakamma/exmaples/client ------> lightclient.c
*
*/

typedef struct
{
    lwm2m_object_t * securityObjP;
    int sock;
    connection_t * connList;
    int addressFamily;
} client_data_t;

void lwm2m_close_connection(void * sessionH,
                            void * userData)
{
    client_data_t * app_data;
    connection_t * targetP;

    app_data = (client_data_t *)userData;
    targetP = (connection_t *)sessionH;

    if (targetP == app_data->connList)
    {
        app_data->connList = targetP->next;
        lwm2m_free(targetP);
    }
    else
    {
        connection_t * parentP;

        parentP = app_data->connList;
        while (parentP != NULL && parentP->next != targetP)
        {
            parentP = parentP->next;
        }
        if (parentP != NULL)
        {
            parentP->next = targetP->next;
            lwm2m_free(targetP);
        }
    }
}

/*
* From wakamma/exmaples/shared ------> connection.c
*
*/

int connection_send(connection_t *connP,
                    uint8_t * buffer,
                    size_t length)
{
    //int nbSent;
    //size_t offset;

    (void) connP; /* unused */
    (void) buffer; /* unused */
    (void) length; /* unused */

#ifdef LWM2M_WITH_LOGS
    char s[INET6_ADDRSTRLEN];
    in_port_t port;

    s[0] = 0;

    if (AF_INET == connP->addr.sin6_family)
    {
        struct sockaddr_in *saddr = (struct sockaddr_in *)&connP->addr;
        inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
        port = saddr->sin_port;
    }
    else if (AF_INET6 == connP->addr.sin6_family)
    {
        struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&connP->addr;
        inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
        port = saddr->sin6_port;
    } else {
        return -1;
    }

    fprintf(stderr, "Sending %lu bytes to [%s]:%hu\r\n", length, s, ntohs(port));

    output_buffer(stderr, buffer, length, 0);
#endif

    //offset = 0;
    //while (offset != length)
    //{
    //   nbSent = sendto(connP->sock, buffer + offset, length - offset, 0, (struct sockaddr *)&(connP->addr), connP->addrLen);
    //    if (nbSent == -1) return -1;
    //   offset += nbSent;
    //}
    return 0;
}


uint8_t lwm2m_buffer_send(void * sessionH,
                          uint8_t * buffer,
                          size_t length,
                          void * userdata)
{
    connection_t * connP = (connection_t*) sessionH;

    (void)userdata; /* unused */

    if (connP == NULL)
    {
        fprintf(stderr, "#> failed sending %lu bytes, missing connection\r\n", length);
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    if (-1 == connection_send(connP, buffer, length))
    {
        fprintf(stderr, "#> failed sending %lu bytes\r\n", length);
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    return COAP_NO_ERROR;
}

bool lwm2m_session_is_equal(void * session1,
                            void * session2,
                            void * userData)
{
    (void)userData; /* unused */

    return (session1 == session2);
}


/*
* From wakamma/tests ------> unittests.c
*
*/

// stub function
void * lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
    (void)userData;
    return (void *)(uintptr_t)secObjInstID;
}
