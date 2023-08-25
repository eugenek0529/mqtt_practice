#include <stdio.h>
#include <stdlib.h>
#include "/opt/homebrew/include/mosquitto.h"
#include <string.h>


void on_connect(struct mosquitto *sub, void *obj, int rc2)
{
    printf("on_connect function is called\n");
    if(rc2)
    {
        printf("Failed with result code: %d\n", rc2);
    }
    mosquitto_subscribe(sub,NULL, "topic",0);
}

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
    rc1 = mosquitto_connect(pub,"localhost",1883,60);
    rc2 = mosquitto_connect(sub,"localhost",1883,60);

    if(rc1 || rc2)
    {
        printf("Could not connect to the borker\n");
        return -1;
    }

    // function to continously receive msg from subed topic
    mosquitto_connect_callback_set(sub,on_connect);
    mosquitto_message_callback_set(sub, msg_sub);


    // New version of total algorithm
    //
    //
    // 
    int session; // 1 - keep, 0 - end
    int quit;
    int role; // 0 - pub, 1 - sub

    // pub variables
    const char *topic = "choice";
    char *payload;
    int payloadLength; // later use strlen(payload)
    int qos = 0; // publisher send msg once and does not wait for ack
    int published; // 0 - published, !0 - not published    
    bool retain = false; // msg does not need to be saved


    do{
        printf("This is start of a new session\n");
        printf("Please choose your role: 0) pub 1) sub: ");
        scanf("%d", &role);
        printf("Key check %d\n", role);
        /*
        do {
            scanf("%d", &role);
        } while ((role != 0) && (role != 1));
        */

        // now the role is choosed
        do{
            //continous loop until user want to stop pub/sub
            if(role == 0)
            {
                // pub
                printf("Please enter your choice: " );
                scanf("%c",payload);
                
                payloadLength = strlen(payload);

                published = mosquitto_publish(pub,NULL,topic,payloadLength,payload,qos,retain);

                if(published != 0)
                {
                    printf("Failed to publish message\n");
                    break;
                }
                else 
                {
                    printf("Message published successfully\n");
                    continue;
                }

            }
            else if(role == 1)
            {
                // sub


            }

            // out here when task is done, and update role
            role++;
            role = role % 2;
            printf("\n\n single pub/sub ended, 0 to continue, 1 to stop: ");
            scanf("%d",&quit);
        } while (quit != 1);
        

        printf("\n\nDo you want to start a new session?: 0)no 1) yes ");
        scanf("%d", &session);

    } while (session == 1);
    //
    //  End of NEW VERSION
    //
    //

    // out here to close the mosquitto objects

    // clear the connection for objects
    printf("Now clearing all mosquitto objects and lib\n");
    mosquitto_disconnect(pub);
    mosquitto_disconnect(sub);
    mosquitto_destroy(pub);
    mosquitto_destroy(sub);
    mosquitto_lib_cleanup();

    return 0;
}