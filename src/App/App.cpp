#include <stdio.h>
#include <iostream>
#include <string>
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"

void print_usage(const char* name);

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

/* flag, which indicates that a preamble has been seen and a message is being
 * received. The sender should not send anything until this flag is cleared */
volatile bool receiving;

/* flag, which indicates that a message is being sent. The listener should
 * ignore all messages until this flag is cleared. */
volatile bool sending;

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

    /* spawn listener thread */
    sgx_status_t status = listener_thread(global_eid);
    std::cout << "asynchronous call" << std::endl;
    if (status != SGX_SUCCESS)
    {
        std::cout << "noob" << std::endl;
    }

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

