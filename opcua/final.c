#include </usr/include/python3.5/Python.h>
#include <signal.h>
#include "open62541.h"
#include "unistd.h"
#include <stdlib.h>
#include <sys/types.h>

UA_Double tmp=0;
UA_Double hum=0;
pid_t child=2;
bool died = true;
int execres = 100;

//  Mode_1  CALLBACK
    static UA_StatusCode Mode_1_callback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
            if (!died) {
                kill(child,SIGINT);
                signal(SIGCHLD, SIG_IGN);
            }
            child = fork();
            died =false;
            if (child ==0){
                printf("Start Mode_1");
                char *argvlist[]={"/usr/bin/python3.5", "/home/pi/IIOT/opcua/mode_1.py", NULL};
                execres = execve(argvlist[0],argvlist,NULL);
            }
        return UA_STATUSCODE_GOOD;
    }

//  Mode_2  CALLBACK
    static UA_StatusCode Mode_2_callback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
        PyRun_SimpleString("GPIO.cleanup()");
        kill(child,SIGINT);
        child = fork();
        if (child == 0){
            printf("Start Mode_2");
            char cmd[100];
            sprintf(cmd,"python3 ./mode_2.py");
            system(cmd);
        }
        else if (child >0)return UA_STATUSCODE_GOOD;
        else printf("error!!\n");
    }

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
        UA_VariableAttributes TEMPattr = UA_VariableAttributes_default;
        UA_Double TEMP = tmp;
        UA_Variant_setScalar(&TEMPattr.value, &TEMP, &UA_TYPES[UA_TYPES_DOUBLE]);
        TEMPattr.description = UA_LOCALIZEDTEXT("en-US","the answer");
        TEMPattr.displayName = UA_LOCALIZEDTEXT("en-US","Temp");
        TEMPattr.dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
        TEMPattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        UA_QualifiedName TEMPName = UA_QUALIFIEDNAME(1, "the answer");
        UA_NodeId tmpnodeid =UA_NODEID_STRING(0, "DHT-Variable");
        UA_Server_addVariableNode(server, tmpnodeid, DOBJNodeId,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), TEMPName,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), TEMPattr, NULL, NULL);
    //==MODE_1==//
        UA_MethodAttributes MODE1_ATTR = UA_MethodAttributes_default;
        MODE1_ATTR.description = UA_LOCALIZEDTEXT("en-US","Auto");
        MODE1_ATTR.displayName = UA_LOCALIZEDTEXT("en-US","Mode_1");
        MODE1_ATTR.executable = true;
        MODE1_ATTR.userExecutable = true;
        UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(0,62542),
                                DOBJNodeId,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                                UA_QUALIFIEDNAME(1, "DHT-QualName"),
                                MODE1_ATTR, &Mode_1_callback,
                                0, NULL, 0, NULL, NULL, NULL);

        UA_MethodAttributes MODE2_ATTR = UA_MethodAttributes_default;
        MODE2_ATTR.description = UA_LOCALIZEDTEXT("en-US","Auto");
        MODE2_ATTR.displayName = UA_LOCALIZEDTEXT("en-US","Mode_2");
        MODE2_ATTR.executable = true;
        MODE2_ATTR.userExecutable = true;
        UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1,62542),
                                DOBJNodeId,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                                UA_QUALIFIEDNAME(1, "DHT-QualName"),
                                MODE2_ATTR, &Mode_2_callback,
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
