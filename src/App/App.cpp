#include <stdio.h>
#include <iostream>
#include <string>
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"
#include <pthread.h>

void print_usage(const char* name);

void* spawn_listener_thread()
{
    /* blocking call, does not return... */
    sgx_status_t status = listener_thread(global_eid);
    if (status != SGX_SUCCESS)
    {
        std::cout << "noob" << std::endl;
    }
}

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

// OCall implementations
void ocall_print(const char* str) {
    printf("%s", str);
}

int main(int argc, char const *argv[]) {
    // Get Enclave
    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        std::cout << "Fail to initialize enclave." << std::endl;
        return 1;
    }

    pthread_t threads[2];

    std::cout << "starting listener_thread" << std::endl;
    pthread_create(&threads[0], NULL, spawn_listener_thread, NULL);
    std::cout << "asynchronous call" << std::endl;

    /* process sending of inputs */
    while (1) {
        char text_buf[128];
        fgets(text_buf, sizeof(text_buf), stdin);

        send_string(global_eid, text_buf);
    }

    // otherwise, print usage
    print_usage(argv[0]);
    return 0;
}

void print_usage(const char* name)
{
    printf("Usage: %s (rx|tx)\n", name);
}

