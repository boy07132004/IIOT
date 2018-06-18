#include </usr/include/python3.5/Python.h>
#include <signal.h>
#include "open62541.h"

UA_Double tmp=0;
UA_Double hum=0;

//  DHT11  CALLBACK
    static UA_StatusCode DHTcallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
        
        PyObject *pModule = NULL, *pFunc2 = NULL, *pFunc = NULL,*pArg = NULL, *resultH = NULL, *resultM = NULL; 
    
        pModule = PyImport_ImportModule ("DHT");
        pFunc= PyObject_GetAttrString (pModule, "H");
        resultH = PyEval_CallObject(pFunc, pArg);
        hum=PyFloat_AsDouble(resultH);
        pFunc2= PyObject_GetAttrString (pModule, "M");
        resultM = PyEval_CallObject(pFunc2, pArg);
        tmp=PyFloat_AsDouble(resultM);
        printf("Humidity : %f\nTemperature : %f\n",hum,tmp);
        //  REFRESH Temp
        UA_NodeId CHANGEId = UA_NODEID_STRING(1, "DHT-Variable");
        UA_Variant myVarTMP;
        UA_Variant_init(&myVarTMP);
        UA_Variant_setScalar(&myVarTMP, &tmp, &UA_TYPES[UA_TYPES_DOUBLE]);
        UA_Server_writeValue(server, CHANGEId, myVarTMP);
        //  REFRESH HUM
        UA_NodeId CHANGEHUMId = UA_NODEID_STRING(0, "DHT-Variable");
        UA_Variant myVarHUM;
        UA_Variant_init(&myVarHUM);
        UA_Variant_setScalar(&myVarHUM, &hum, &UA_TYPES[UA_TYPES_DOUBLE]);
        UA_Server_writeValue(server, CHANGEHUMId, myVarHUM);
        //
        return UA_STATUSCODE_GOOD;}
//  DHT11  Add OBJECT
    static void addObjectDHT(UA_Server *server) {
    UA_ObjectAttributes HAttr = UA_ObjectAttributes_default;
    HAttr.displayName = UA_LOCALIZEDTEXT("en-US","DHT11");
    UA_NodeId DOBJNodeId = UA_NODEID_STRING(1, "DHT");
    UA_Server_addObjectNode(server, DOBJNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, "DHT"),
                            UA_NODEID_NULL,HAttr, NULL, NULL);
    
    //==VARIABLE==//
        UA_VariableAttributes HUMattr = UA_VariableAttributes_default;
        UA_Double HUM = hum;
        UA_Variant_setScalar(&HUMattr.value, &HUM, &UA_TYPES[UA_TYPES_DOUBLE]);
        HUMattr.description = UA_LOCALIZEDTEXT("en-US","the answer");
        HUMattr.displayName = UA_LOCALIZEDTEXT("en-US","Hum");
        HUMattr.dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
        HUMattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        UA_QualifiedName HUMName = UA_QUALIFIEDNAME(1, "the answer");
        UA_NodeId humnodeid =UA_NODEID_STRING(0, "DHT-Variable");
        UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
        UA_Server_addVariableNode(server, humnodeid, DOBJNodeId,
                                parentReferenceNodeId, HUMName,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), HUMattr, NULL, NULL);
    
    //==VARIABLE2==//
        UA_VariableAttributes TEMPattr = UA_VariableAttributes_default;
        UA_Double TEMP = tmp;
        UA_Variant_setScalar(&TEMPattr.value, &TEMP, &UA_TYPES[UA_TYPES_DOUBLE]);
        TEMPattr.description = UA_LOCALIZEDTEXT("en-US","the answer");
        TEMPattr.displayName = UA_LOCALIZEDTEXT("en-US","Temp");
        TEMPattr.dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
        TEMPattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        UA_QualifiedName TEMPName = UA_QUALIFIEDNAME(1, "the answer");
        UA_NodeId tmpnodeid =UA_NODEID_STRING(1, "DHT-Variable");
        UA_Server_addVariableNode(server, tmpnodeid, DOBJNodeId,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), TEMPName,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), TEMPattr, NULL, NULL);
    //==METHOD==//
        UA_MethodAttributes Hattr = UA_MethodAttributes_default;
        Hattr.description = UA_LOCALIZEDTEXT("en-US","DHTdata");
        Hattr.displayName = UA_LOCALIZEDTEXT("en-US","Get Data");
        Hattr.executable = true;
        Hattr.userExecutable = true;
        UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1,62542),
                                DOBJNodeId,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                                UA_QUALIFIEDNAME(1, "DHT-QualName"),
                                Hattr, &DHTcallback,
                                0, NULL, 0, NULL, NULL, NULL);
    }


UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

int main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    Py_Initialize();
    PyRun_SimpleString("import sys");  
    PyRun_SimpleString("import RPi.GPIO as GPIO");
    PyRun_SimpleString("sys.path.append('./')");
    PyRun_SimpleString("GPIO.setwarnings(False)");
    UA_ServerConfig *config = UA_ServerConfig_new_default();
    UA_Server *server = UA_Server_new(config);
    addObjectDHT(server);

    UA_StatusCode retval = UA_Server_run(server, &running);  
    PyRun_SimpleString("GPIO.cleanup()"); 
    Py_Finalize(); 
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    return (int)retval;
}
