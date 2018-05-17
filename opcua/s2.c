#include </usr/include/python3.5/Python.h>
#include <signal.h>
#include "open62541.h"

int led=1;

static UA_StatusCode
Ledcallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
//==================================================//
    PyObject *pModule,*pFunc;
    PyObject *pArgs, *pValue;


    pModule = PyImport_Import(PyUnicode_FromString("sold"));


    pFunc = PyObject_GetAttrString(pModule, "led");


    pArgs = PyTuple_New(led);

    pValue = PyObject_CallObject(pFunc, pArgs);
//==================================================//
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Turn on LED");
    return UA_STATUSCODE_GOOD;
}

static void
LedMethod(UA_Server *server) {
    UA_Argument inputArgument;
    UA_Argument_init(&inputArgument);
    inputArgument.description = UA_LOCALIZEDTEXT("en-US", "A String");
    inputArgument.name = UA_STRING("MyInput");
    inputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    inputArgument.valueRank = -1;

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT("en-US", "A String");
    outputArgument.name = UA_STRING("MyOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = -1;

    UA_MethodAttributes ledattr = UA_MethodAttributes_default;
    ledattr.description = UA_LOCALIZEDTEXT("en-US","Turn on");
    ledattr.displayName = UA_LOCALIZEDTEXT("en-US","LED-Display");
    ledattr.executable = true;
    ledattr.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1,62541),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            UA_QUALIFIEDNAME(1, "LED-QualName"),
                            ledattr, &Ledcallback,
                            1, &inputArgument, 1, &outputArgument, NULL, NULL);

}

UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

int main(void) {
    Py_Initialize();
    PyRun_SimpleString("import sys"); PyRun_SimpleString("sys.path.append('./')");
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    /*
    PyRun_SimpleString("import RPi.GPIO as GPIO");
    */
    UA_ServerConfig *config = UA_ServerConfig_new_default();
    UA_Server *server = UA_Server_new(config);
 
    LedMethod(server);
    
    UA_StatusCode retval = UA_Server_run(server, &running); 
    
    Py_Finalize();
    printf("hi\n");
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    return (int)retval;
}