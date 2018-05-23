#include </usr/include/python3.5/Python.h>
#include <signal.h>
#include "open62541.h"
/*
    建立兩個Object 分別為LED 以及DTH11
• 在LED 下建立
    • turnon method 打開LED
    • turnoff method 關閉LED
    • status 為LED 狀態（UA_String，初始值為off）
• 在DTH11 下建立
    • getdata method 測量溫濕度
    • temp 放溫度值（UA_Double ，初始值為0）
    • hum 放濕度值（UA_Double，初始值為0）
*/

static void
addObject(UA_Server *server) {
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US","OBJECT");
    UA_NodeId OBJNodeId = UA_NODEID_STRING(1, "OBJECT");
    UA_Server_addObjectNode(server, OBJNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, "OBJECT"),
                            UA_NODEID_NULL,oAttr, NULL, NULL);
    PyRun_SimpleString("import sys");  
    PyRun_SimpleString("sys.path.append('./')");
    Py_Initialize();
    PyObject *pModule = NULL, *pDict = NULL, *pFunc = NULL, *pArg = NULL, *result = NULL; 
    PyObject *pFunc2 = NULL,*pFunc3 = NULL,*pFunc4 = NULL;
    pModule = PyImport_ImportModule("LED");
    pFunc = PyObject_GetAttrString(pModule, "initEnv");
    pFunc2 = PyObject_GetAttrString(pModule, "initPin");
    pFunc2 = PyObject_GetAttrString(pModule, "LEDDD");

    //==VARIABLE==//
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    UA_String LEDDDD = UA_STRING("off");
    UA_Variant_setScalar(&attr.value, &LEDDDD, &UA_TYPES[UA_TYPES_STRING]);
    attr.description = UA_LOCALIZEDTEXT("en-US","the answer");
    attr.displayName = UA_LOCALIZEDTEXT("en-US","LED_Status");
    attr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId LEDDDDNodeId = UA_NODEID_STRING(1, "the.answer");
    UA_QualifiedName LEDDDDName = UA_QUALIFIEDNAME(1, "the answer");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_Server_addVariableNode(server, LEDDDDNodeId, OBJNodeId,
                              parentReferenceNodeId, LEDDDDName,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, NULL, NULL);
}

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
    addObject(server);
    

    UA_StatusCode retval = UA_Server_run(server, &running); 
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    return (int)retval;
}
