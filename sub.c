#include <stdio.h> 
#include <stdlib.h>
#include "/opt/homebrew/include/mosquitto.h"



void on_connect(struct mosquitto *mosq, void *obj, int rc)
{
    if(rc)
    {
        printf("ID: %d\n", * (int *) obj);

        // return value greater than 0 is an error in mosquitto
        if(rc)
        {
            printf("Result code error: %d\n", rc);
            exit(-1);
        }
        mosquitto_subscribe(mosq, NULL,"test/t1",0);


    }
}


void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
}

int main()
{
    int rc, id=12;

    mosquitto_lib_init();

    struct mosquitto *mosq;

    // create a new mosq object (Name, cleanSession, pointer to client id)
    mosq = mosquitto_new("subscriber-test",true,&id);
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    // connect to broker
    rc = mosquitto_connect(mosq, "localhost", 1883, 10);

    if(rc)
    {
        printf("Could not connet to the broker with return code: %d\n", rc);
        return -1;
    }

    mosquitto_loop_start(mosq);
    printf("Press Enter to quit...\n");
    getchar();
    mosquitto_loop_stop(mosq,true);

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    
    return 0;
}