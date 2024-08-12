#include "protocol.h"

struct dynamic_server *dynamic_server_new(uint16_t http_port, uint16_t rtsp_port)
{
    struct dynamic_server *server;

    evhttp_bind_socket(server->http, "localhost", http_port);
}

void dynamic_server_free(struct dynamic_server *server);

void dynamic_server_serve(struct dynamic_server *server);

void dynamic_server_shutdown(struct dynamic_server *server);