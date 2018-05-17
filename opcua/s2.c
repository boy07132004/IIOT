#include </usr/include/python3.5/Python.h>
#include <signal.h>
#include "open62541.h"

int led=1;

static void addVariable(UA_Server *server) {
    /* Define the attribute of the LEDDDD variable node */
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    const char* temp="123";
    UA_String LEDDDD = UA_String_fromChars(temp);
    UA_Variant_setScalar(&attr.value, &LEDDDD, &UA_TYPES[UA_TYPES_INT32]);
    attr.description = UA_LOCALIZEDTEXT("en-US","the answer");
    attr.displayName = UA_LOCALIZEDTEXT("en-US","LED_Status");
    attr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    /* Add the variable node to the information model */
    UA_NodeId LEDDDDNodeId = UA_NODEID_STRING(1, "the.answer");
    UA_QualifiedName LEDDDDName = UA_QUALIFIEDNAME(1, "the answer");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_Server_addVariableNode(server, LEDDDDNodeId, parentNodeId,
                              parentReferenceNodeId, LEDDDDName,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, NULL, NULL);
}


static UA_StatusCode
Ledcallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
//==================================================//
    switch(led%4){
        case 1:
            PyRun_SimpleString("pwmr.ChangeDutyCycle(90)");
            PyRun_SimpleString("pwmg.ChangeDutyCycle(0)");
            PyRun_SimpleString("pwmb.ChangeDutyCycle(0)");
            break;
        case 2:
            PyRun_SimpleString("pwmr.ChangeDutyCycle(0)");
            PyRun_SimpleString("pwmg.ChangeDutyCycle(90)");
            PyRun_SimpleString("pwmb.ChangeDutyCycle(0)");
            break;
        case 3:
            PyRun_SimpleString("pwmr.ChangeDutyCycle(0)");
            PyRun_SimpleString("pwmg.ChangeDutyCycle(0)");
            PyRun_SimpleString("pwmb.ChangeDutyCycle(90)");
            break;
        case 0:
            PyRun_SimpleString("pwmr.ChangeDutyCycle(0)");
            PyRun_SimpleString("pwmg.ChangeDutyCycle(0)");
            PyRun_SimpleString("pwmb.ChangeDutyCycle(0)");
            break;
    }
    
    led++;
    printf("%d",led);
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
    addVariable(server);
    LedMethod(server);
    ////
    PyRun_SimpleString("import RPi.GPIO as GPIO");
    PyRun_SimpleString("GPIO.setmode(GPIO.BOARD)");
    PyRun_SimpleString("pins={'r':3,'g':5,'b':7}");
    PyRun_SimpleString("for i in pins:GPIO.setup(pins[i],GPIO.OUT)");
    PyRun_SimpleString("pwmr = GPIO.PWM(pins['r'],2000)");
    PyRun_SimpleString("pwmg = GPIO.PWM(pins['g'],2000)");
    PyRun_SimpleString("pwmb = GPIO.PWM(pins['b'],2000)");
    PyRun_SimpleString("pwmr.start(0)");
    PyRun_SimpleString("pwmg.start(0)");
    PyRun_SimpleString("pwmb.start(0)");
    ///
    UA_StatusCode retval = UA_Server_run(server, &running); 
    PyRun_SimpleString("pwmr.stop()");
    PyRun_SimpleString("pwmg.stop()");
    PyRun_SimpleString("pwmb.stop()");
    PyRun_SimpleString("GPIO.cleanup()");
    Py_Finalize();
    led = led * 0;
    printf("\nbye\n");
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    return (int)retval;
}