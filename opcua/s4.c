#include </usr/include/python3.5/Python.h>
#include <signal.h>
#include "open62541.h"
//  Add variable
//  Handler
    static void handler(UA_UInt32 monID,UA_DataValue *value,void *context){
        UA_Variant value1;
        UA_Variant_init(&value1);
        const UA_NodeId nodeId= UA_NODEID_STRING(0, "DHT-Variable");
        UA_StatusCode retval = UA_Client_readValueAttribute(context,nodeId,&value1);
        if (retval == UA_STATUSCODE_GOOD){
            UA_Double Hvalue = *(UA_Double*)value1.data;
            printf("Humidity : %f \n",Hvalue);
        }
    }
//  Stop Function
    UA_Boolean running = true;
    static void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
        running = false;
    }


int main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    UA_Client *client = UA_Client_new(UA_ClientConfig_default);
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.50.93:4840");
    if(retval != UA_STATUSCODE_GOOD) {UA_Client_delete(client);return (int)retval;}
    //
    const UA_NodeId HumID = UA_NODEID_STRING(0, "DHT-Variable");
    const UA_NodeId TmpID = UA_NODEID_STRING(1, "DHT-Variable");
    UA_Variant Humvalue,Tmpvalue;
    UA_Variant_init(&Humvalue);
    UA_Variant_init(&Tmpvalue);
    UA_UInt32 subID = 0;
    UA_UInt32 monID = 0;
 

    while(running){
    
    }

    UA_Client_disconnect(client);
    return UA_STATUSCODE_GOOD;
}

