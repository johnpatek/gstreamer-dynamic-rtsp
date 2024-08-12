#include "rtsp.h"


int main(int argc, const char **argv)
{
    struct rtsp_server *server;
    server = rtsp_server_new(NULL,0);
    rtsp_server_serve(server);
    return 0;
}