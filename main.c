#include <stdio.h>
#include "liblwm2m.h"
#include "coap/er-coap-13/er-coap-13.h"

uint8_t message_send_mock_server(coap_packet_t * message); // implicit declaration warning

int main() {

    coap_packet_t * message = lwm2m_malloc(sizeof(coap_packet_t));
    message_send_mock_server(message);

    get_test_object();

    printf("%d\n", message_send_mock_server(message));
}


/// MAYBE USEFULL

/*
* Let liblwm2m respond to the query depending on the context    ||||| dispatch received data to liblwm2m |||| from packet.c
*/
//lwm2m_handle_packet(lwm2mH, buffer, (size_t)numBytes, connP);

// EASILY TO CREATE AND FILL UP
//client_data_t data;
//lwm2m_context_t * lwm2mH = NULL;
//lwm2m_object_t * objArray[OBJ_COUNT];



