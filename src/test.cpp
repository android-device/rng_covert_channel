
#include"util.hpp"

int main(int argc, char **argv)
{
    // Put your covert channel setup code here

    CYCLES test[100];
    printf("Please press enter.\n");

    char text_buf[2];
    fgets(text_buf, sizeof(text_buf), stdin);

    printf("Receiver now listening.\n");

    bool listening = true;
    while (listening) {

        // Put your covert channel code here
        for(int i = 0; i < 40; i++)
            test[i] = measure_n_rdseed_time(4);
        for(int i = 0; i < 40; i++)
            printf("iter %d: %d\n", i, test[i]);

        printf("Please press enter.\n");
        fgets(text_buf, sizeof(text_buf), stdin);
    }

    printf("Receiver finished.\n");

    return 0;
}


