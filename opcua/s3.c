#include </usr/include/python3.5/Python.h>
#include <signal.h>
#include "open62541.h"
/*
    建立兩個Object 分別為LED 以及DTH11
• 在LED 下建立
    • status 為LED 狀態（UA_String，初始值為off）
• 在DTH11 下建立
    • getdata method 測量溫濕度
    • temp 放溫度值（UA_Double ，初始值為0）
    • hum 放濕度值（UA_Double，初始值為0）
*/
UA_Double tmp=0;
UA_Double hum=0;

//  LED    CALLBACK TURN ON
    static UA_StatusCode Ledcallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
        UA_String LEDDDD = UA_STRING("on");
        UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "the.answer");
        UA_Variant myVar;
        UA_Variant_init(&myVar);
        UA_Variant_setScalar(&myVar, &LEDDDD, &UA_TYPES[UA_TYPES_STRING]);
        UA_Server_writeValue(server, myIntegerNodeId, myVar);
        PyObject *pModule = NULL, *pDict = NULL, *pFunc = NULL, *pArg = NULL, *result = NULL; 
        pModule = PyImport_ImportModule("LED");
        pFunc = PyObject_GetAttrString(pModule, "main");
        PyObject_CallObject(pFunc, pArg);
        return UA_STATUSCODE_GOOD;}

//  LED    CALLBACK TURN OFF
    static UA_StatusCode Ledcallback2(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
        UA_String LEDDDD = UA_STRING("off");
        UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, "the.answer");
        UA_Variant myVar;
        UA_Variant_init(&myVar);
        UA_Variant_setScalar(&myVar, &LEDDDD, &UA_TYPES[UA_TYPES_STRING]);
        UA_Server_writeValue(server, myIntegerNodeId, myVar);
        PyObject *pModule = NULL, *pDict = NULL, *pFunc = NULL, *pArg = NULL, *result = NULL; 
        pModule = PyImport_ImportModule("LED");
        pFunc = PyObject_GetAttrString(pModule, "OFF");
        PyObject_CallObject(pFunc, pArg);
        return UA_STATUSCODE_GOOD;}



//  LED    Add OBJECT
    static void addObject(UA_Server *server) {
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US","LED");
    UA_NodeId OBJNodeId = UA_NODEID_STRING(1, "OBJECT");
    UA_Server_addObjectNode(server, OBJNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, "OBJECT"),
                            UA_NODEID_NULL,oAttr, NULL, NULL);
 
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
    //==METHOD==//

        UA_MethodAttributes ledattr = UA_MethodAttributes_default;
        ledattr.description = UA_LOCALIZEDTEXT("en-US","Turn on");
        ledattr.displayName = UA_LOCALIZEDTEXT("en-US","turnon");
        ledattr.executable = true;
        ledattr.userExecutable = true;
        UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1,62541),
                                OBJNodeId,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                                UA_QUALIFIEDNAME(1, "LED-QualName"),
                                ledattr, &Ledcallback,
                                0,NULL, 0, NULL, NULL, NULL);
        
    //==METHOD2==//
        UA_Argument inputArgument2;
        UA_Argument_init(&inputArgument2);
        inputArgument2.description = UA_LOCALIZEDTEXT("en-US", "A String");
        inputArgument2.name = UA_STRING("MyInput");
        inputArgument2.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
        inputArgument2.valueRank = -1;

        UA_Argument outputArgument2;
        UA_Argument_init(&outputArgument2);
        outputArgument2.description = UA_LOCALIZEDTEXT("en-US", "A String");
        outputArgument2.name = UA_STRING("MyOutput");
        outputArgument2.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
        outputArgument2.valueRank = -1;

        UA_MethodAttributes ledattr2 = UA_MethodAttributes_default;
        ledattr2.description = UA_LOCALIZEDTEXT("en-US","Turn off");
        ledattr2.displayName = UA_LOCALIZEDTEXT("en-US","turnoff");
        ledattr2.executable = true;
        ledattr2.userExecutable = true;
        UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1,62540),
                                OBJNodeId,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                                UA_QUALIFIEDNAME(2, "LED-QualName2"),
                                ledattr2, &Ledcallback2,
                                1, &inputArgument2, 1, &outputArgument2, NULL, NULL);
    
    }
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
        //
        UA_NodeId CHANGEId = UA_NODEID_STRING(1, "DHT-Variable");
        UA_Variant myVarTMP;
        UA_Variant_init(&myVarTMP);
        UA_Variant_setScalar(&myVarTMP, &tmp, &UA_TYPES[UA_TYPES_DOUBLE]);
        UA_Server_writeValue(server, CHANGEId, myVarTMP);
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
    addObject(server);
    addObjectDHT(server);

    UA_StatusCode retval = UA_Server_run(server, &running);  
    PyRun_SimpleString("GPIO.cleanup()"); 
    Py_Finalize(); 
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    return (int)retval;
}
