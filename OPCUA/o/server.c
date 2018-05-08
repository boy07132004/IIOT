#include <signal.h>
#include "open62541.h"


UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

int main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_ServerConfig *config = UA_ServerConfig_new_default();
    UA_Server *server = UA_Server_new(config);

    UA_StatusCode retval = UA_Server_run(server, &running);
    //const char* IDn = "66";
    //const char *ID = "66";
    //UA_String ID="N66061359";
    UA_String st("TES");
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,st);
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    return (int)retval;
}
