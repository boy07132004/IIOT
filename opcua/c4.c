#include </usr/include/python3.5/Python.h>
#include <signal.h>
#include "open62541.h"

UA_Boolean running = true;
    static void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
        running = false;
    }

UA_Client_call()

int main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    UA_Client *client = UA_Client_new(UA_ClientConfig_default);
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.50.93:4840");
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int)retval;}

    while(running);
    UA_Client_disconnect(client);
    return UA_STATUSCODE_GOOD;
}

