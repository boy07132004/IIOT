#include <signal.h>
#include "open62541.h"
#include <stdio.h>

UA_Boolean running = true;

static void stopHandler(int sig)
{
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
	running = false;
}

int main(void)
{
	signal(SIGINT, stopHandler);
	signal(SIGTERM,stopHandler);
	UA_ServerConfig *config = UA_ServerConfig_new_default();
	UA_ServerNetworkLayer nl = UA_ServerNetworkLayerTCP(UA_ConnectionConfig_standard,4840);
	config->networkLayers = &nl;
	config->networkLayersSize = 1;
	UA_Server *server = UA_Server_new(config);
	UA_Server_run(server,&running);
	UA_Server_delete(server);
	nl.deleteMembers(&nl);
	return 0 ;
}
