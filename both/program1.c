#include <stdio.h>
#include "/opt/homebrew/include/mosquitto.h"
#include <string.h>

void msg_sub(struct mosquitto *sub, void *obj, const struct mosquitto_message *msg)
{
    printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
}


int main()
{
    // mosquitto objs
    struct mosquitto *pub;
    struct mosquitto *sub;

    // return value catcher
    int rc1, rc2;

    mosquitto_lib_init();

    pub = mosquitto_new("publish",true,NULL);
    sub = mosquitto_new("subscribe",true,NULL);

    //connect both objs and check for both obj connection
    rc1 = msoquitto_connect(pub,"localhost",1883,60);
    rc2 = mosquitto_connect(sub,"localhost",1883,60);

    if(rc1 || rc2)
    {
        printf("Could not connect to the borker\n");
        return -1;
    }

    // function to continously receive msg from subed topic
    mosquitto_message_callback_set(sub, msg_sub);

    // sub loop start
    mosquitto_loop_start(sub);
    do{
        // do what ever
    } while (getchar() != 'q');
    
    // out the loop, stop sub-loop
    mosquitto_loop_stop(sub,true);



    // clear the connection for objects
    mosquitto_disconnect(pub);
    mosquitto_disconnect(sub);
    mosquitto_destroy(pub);
    mosquitto_destroy(sub);
    mosquitto_lib_cleanup();

}