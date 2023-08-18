#include <stdio.h>
#include "/opt/homebrew/include/mosquitto.h"


int main()
{
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




    
    // Clean up and disconnect
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);

    mosquitto_lib_cleanup();

    return 0;


}