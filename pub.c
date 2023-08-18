#include <stdio.h>
#include "/opt/homebrew/include/mosquitto.h"
#include <string.h>

int main()
{
    /**
     * First step is to initialize the mostquitto lib
     * then create a object to certain channel to publish (broker)
     * 
    */
    int rc;
    struct mosquitto *mosq;

    mosquitto_lib_init();

    mosq = mosquitto_new("publisher-test",true,NULL);

    rc = mosquitto_connect(mosq, "localhost", 1883, 60);
    if(rc !=0)
    {
        printf("NOT CONNECTED\n");
        return -1;
    }

    printf("We are now connected to the broker\n");

    /*
     * Next is to create a topic and pulish my own payload 
    */

   const char *topic = "choice";
   const char *payload = "payload_example";
   int payloadLength = strlen(payload);
   int qos = 0; // pubisher send meg once and doesnt wait for acknolegement
   bool retain = false; // msg does not need to be saved
   

   int pub;
   pub = mosquitto_publish(mosq, NULL, topic, payloadLength, payload,qos,retain);

   if (rc != 0)
    {
        printf("Failed to publish message\n");
    }
    else
    {
        printf("Message published successfully\n");
    }

    // Clean up and disconnect
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);

    mosquitto_lib_cleanup();

    return 0;


}