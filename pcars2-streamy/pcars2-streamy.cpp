
//Including standard header imports
#include "stdafx.h"
#include "APIService.h"

#define STREAMY_VERSION "v0.1"
#define POLL_TIME_IN_MILLIS 17
#define ESC_KEY 27
#define API_URL "/streamy/v1/api"
#define HTTP_RESPONSE_404 "{\"status\": \"404 Not found, please use the correct URL: " API_URL "\"}"

static const char *s_http_port = "8888";
static struct ns_serve_http_opts s_http_server_opts;


static APIService apiService = APIService();


static void eventHandler(struct ns_connection *nc, int ev, void *ev_data)
{
	struct http_message *hm = (struct http_message *) ev_data;

	switch (ev)
	{
	case NS_HTTP_REQUEST:
		if (ns_vcmp(&hm->uri, API_URL) == 0)
		{
			apiService.handle(nc, hm);
		}
		else {
			ns_printf(nc, "HTTP/1.1 404 Not found\r\n"
				"Content-Type: application/json\r\n"
				"Cache-Control: no-cache\r\n"
				"Access-Control-Allow-Origin: *\r\n"
				"Content-Length: %d\r\n\r\n%s",
				(int)strlen(HTTP_RESPONSE_404), HTTP_RESPONSE_404);
		}
		break;
	default:
		break;
	}
}


int main()
{
	struct ns_mgr mgr;
	struct ns_connection *nc;
	ns_mgr_init(&mgr, NULL);
	nc = ns_bind(&mgr, s_http_port, eventHandler);
	ns_set_protocol_http_websocket(nc);
	s_http_server_opts.document_root = ".";



	printf("#- PCARS2 STREAMY - PCARS2 STREAM OVERLAY TOOL %s\n", STREAMY_VERSION);
	printf("#- (c) 2018 Nico Weiser\n\n");
	printf("#- Server started... port %s\n", s_http_port);
	printf("#- Available at http://localhost:%s%s \n", s_http_port, API_URL);
	printf("#- Hit ESC to exit\n");


	while (true)
	{
		ns_mgr_poll(&mgr, POLL_TIME_IN_MILLIS);

		if (_kbhit() && _getch() == ESC_KEY)
		{
			break;
		}
	}

}

