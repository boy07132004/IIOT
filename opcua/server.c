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
    
    //-----------STUDENT_ID------------//
    const char* student = "N66061359";
    UA_String ID = UA_String_fromChars(student);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,"%s logging...",ID.data);
    //------------STARTTIME------------//
    UA_DateTimeStruct t = UA_DateTime_toStruct(UA_DateTime_now()+UA_DateTime_localTimeUtcOffset());   
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,"StartTime:%04u-%02u-%02u %02u:%02u:%02u",t.year,t.month,t.day,t.hour,t.min,t.sec);
    
    //---------------END---------------//

    UA_StatusCode retval = UA_Server_run(server, &running); 
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    return (int)retval;
}
