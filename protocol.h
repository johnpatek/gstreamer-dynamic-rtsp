#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <event2/http.h>

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

struct dynamic_server
{
    struct event_base *base;
    struct evhttp *http;
    GstRTSPServer *rtsp;
    GHashTable *sessions;
};

struct dynamic_session
{
};

struct dynamic_client
{
};

struct dynamic_server *dynamic_server_new(uint16_t http_port, uint16_t rtsp_port);

void dynamic_server_free(struct dynamic_server *server);

void dynamic_server_serve(struct dynamic_server *server);

void dynamic_server_shutdown(struct dynamic_server *server);

#endif