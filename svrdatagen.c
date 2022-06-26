#include "open62541.h"
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

UA_UInt32 uiCurrentValue = 0; 

static void
beforeRead(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeid, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data) {

    UA_UInt32 intValue = *(UA_UInt32 *)nodeContext;
    UA_Variant value;
    UA_Variant_setScalar(&value, &intValue, &UA_TYPES[UA_TYPES_UINT32]);
    UA_Server_writeValue(server, **&nodeid, value);
}

static void
addDataSourceVariable(UA_Server *server) {
    char nodeName[18];
    char snum[11];

    for(int i = 1; i <= 100000; i++) {
        strcpy(nodeName, "uint32.");
        sprintf(snum, "%d", i);
        strcat(nodeName, snum);

        UA_VariableAttributes attr = UA_VariableAttributes_default;
        UA_Variant_setScalar(&attr.value, &uiCurrentValue, &UA_TYPES[UA_TYPES_UINT32]);
        attr.description = UA_LOCALIZEDTEXT("en-US",nodeName);
        attr.displayName = UA_LOCALIZEDTEXT("en-US",nodeName);
        attr.dataType = UA_TYPES[UA_TYPES_UINT32].typeId;
        attr.accessLevel = UA_ACCESSLEVELMASK_READ;
        attr.value.storageType = UA_VARIANT_DATA_NODELETE;

        /* Add the variable node to the information model */
        UA_NodeId myIntegerNodeId = UA_NODEID_STRING(1, nodeName);
        UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME(1, nodeName);
        UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
        UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
        UA_Server_addVariableNode(server, myIntegerNodeId, parentNodeId,
                                parentReferenceNodeId, myIntegerName,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, NULL, NULL);

        UA_ValueCallback callback;
        callback.onRead = beforeRead;
        callback.onWrite = NULL;

        UA_Server_setNodeContext(server, myIntegerNodeId, &uiCurrentValue);
        UA_Server_setVariableNode_valueCallback(server, myIntegerNodeId, callback);
    }
}

static volatile UA_Boolean running = true;
static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

void *updateCurrentValue(void *arg) 
{
    while(1) {
        uiCurrentValue++;
        if (false == running) {
            break;
        }

        UA_sleep_ms(1000);
        // printf("Updating: %d\n", uiCurrentValue);
    }
    printf("Exiting updateCurrentValue\n");
}

int main(int argc, char** argv) {
    pthread_t threadUpdate;
    int iret = pthread_create( &threadUpdate, NULL, updateCurrentValue, NULL );

    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
	if( argc > 1) {
        int port = 4840;
		port = atoi(argv[1]);
        UA_ServerConfig_setMinimal(UA_Server_getConfig(server), port, NULL);
	} else {
        UA_ServerConfig_setDefault(UA_Server_getConfig(server));
    }
    
    addDataSourceVariable(server);

    UA_StatusCode retval = UA_Server_run(server, &running);

    UA_Server_delete(server);
    pthread_join( threadUpdate, NULL );
    printf("Thread return : %d\n", iret );
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
