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




    //
    //
    // OLD VERSION
    //
    //
    char sessionEnd;
    do{ 
        printf("Start of new session\n");

        // Prompt the user to choose what to play
        char user;
        
        printf("Player(1) or Player(2)?: ");
        do{
            // input validation
            user = getchar();
        } while ((user != '1') || (user != '2'));



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

        printf("\n\nDo you want to satrt a new session?: 1) yes 2) no");
        sessionEnd = getchar();

    } while (sessionEnd != '2'); //end of big-dowhile
    //
    // END of OLD VERSOIN
    //
    //



    // New version of total algorithm
    //
    //
    // 
    int session; // 1 - keep, 0 - end
    int quit;
    int role = 0; // 0 - pub, 1 - sub
    do{
        printf("This is start of a new session\n");
        printf("Please choose your role: 0) pub 1) sub: ");
        do {
            scanf("%d", &role);
        } while ((role != 0) || (role != 1));

        // now the role is choosed
        do{
            //continous loop until user want to stop pub/sub
            if(role == 0)
            {
                // pub
            }
            else if(role == 1)
            {
                // sub
            }

            // out here when task is done, and update role
            role++;
            role = role % 2;
            printf("\n\n single pub/sub ended, 0 to continue, 1 to stop: ");
            scanf("%quit",&quit);
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