#include </usr/include/python3.5/Python.h>
#include <signal.h>
#include "open62541.h"

//  Add variable
//  Handler

    static void handler_HUMChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
                           UA_UInt32 monId, void *monContext, UA_DataValue *value) {
    if(UA_Variant_hasScalarType(&value->value, &UA_TYPES[UA_TYPES_DOUBLE])) {
        UA_Double Hdata = *(UA_Double *) value->value.data;
        printf("Humidity: %f \n",Hdata);}}

    static void handler_TMPChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
                           UA_UInt32 monId, void *monContext, UA_DataValue *value) {
    if(UA_Variant_hasScalarType(&value->value, &UA_TYPES[UA_TYPES_DOUBLE])) {
        UA_Double Tdata = *(UA_Double *) value->value.data;
        printf("Temperature: %f \n",Tdata);}
    }
//  Stop Function
    UA_Boolean running = true;
    static void stopHandler(int sig) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
        running = false;
    }

//  default
    static void deleteSubscriptionCallback(UA_Client *client, UA_UInt32 subscriptionId, void *subscriptionContext) {
    printf("Bye Bye\n");
    }
    static void subscriptionInactivityCallback (UA_Client *client, UA_UInt32 subId, void *subContext) {
    printf("Inactivity for subscription\n");}
    

int main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    UA_Client *client = UA_Client_new(UA_ClientConfig_default);
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.50.93:4840");
    if(retval != UA_STATUSCODE_GOOD) {UA_Client_delete(client);return (int)retval;}
    //
    UA_CreateSubscriptionRequest
        request = UA_CreateSubscriptionRequest_default();
    UA_CreateSubscriptionResponse
        response = UA_Client_Subscriptions_create(client, request,NULL, NULL, deleteSubscriptionCallback);
    
        UA_MonitoredItemCreateRequest
        humRequest = UA_MonitoredItemCreateRequest_default(UA_NODEID_STRING(0, "DHT-Variable"));
        UA_MonitoredItemCreateResult
        humResponse = UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,UA_TIMESTAMPSTORETURN_BOTH,
                                             humRequest, NULL,handler_HUMChanged, NULL);
   
        UA_MonitoredItemCreateRequest
            tmpRequest = UA_MonitoredItemCreateRequest_default(UA_NODEID_STRING(1, "DHT-Variable"));
        UA_MonitoredItemCreateResult
            tmpResponse = UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,UA_TIMESTAMPSTORETURN_BOTH,
                                             tmpRequest, NULL, handler_TMPChanged, NULL);
     
    //
    while(running){
        UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.50.93:4840");
        UA_Client_runAsync(client, 1000);
    }

    UA_Client_disconnect(client);
    return UA_STATUSCODE_GOOD;
}

