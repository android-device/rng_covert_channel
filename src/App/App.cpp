#include <stdio.h>
#include <iostream>
#include <string>
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"
#include <pthread.h>

#define DELAY 1000

void print_usage(const char* name);

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

void* spawn_listener_thread(void* arguments)
{
    /* blocking call, does not return... */
    sgx_status_t status = listener_thread(global_eid);
    if (status != SGX_SUCCESS)
    {
        std::cout << "noob" << std::endl;
    }
}

// OCall implementations
void ocall_print(const char* str) {
    printf("%s", str);
}

void nops(uint32_t n)
{
    asm volatile(
    "loop%=: \n\t"
    "loop loop%=\n\t"
    :
    : "c"(n)
    : );
}

int main(int argc, char const *argv[]) {
    // Get Enclave
    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        std::cout << "Fail to initialize enclave." << std::endl;
        return 1;
    }

    pthread_t threads[2];
    int pthread_args[2];
    int pthread_status[2];

    pthread_args[0] = 0;
    pthread_args[1] = 0;

    std::cout << "starting listener_thread" << std::endl;
    pthread_status[0] = pthread_create(&threads[0], NULL, spawn_listener_thread, &pthread_args[0]);
    std::cout << "asynchronous call" << std::endl;

    printf("Press enter after starting the other process, in order to begin tuning.");
    fgets(text_buf, sizeof(text_buf), stdin);
    for(int i=0; i<20; i++)
    {
        send_string(global_eid, "Welcome");
        nops(DELAY);
    }

    while (1) {
        char text_buf[128];
        fgets(text_buf, sizeof(text_buf), stdin);

        send_string(global_eid, text_buf);
    }

    /* both workers are blocking calls, should continue until kill signal is received. */
    while (true) {} ;

    return 0;
}

void print_usage(const char* name)
{
    printf("Usage: %s (rx|tx)\n", name);
}

