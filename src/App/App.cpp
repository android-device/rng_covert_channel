#include <stdio.h>
#include <iostream>
#include <string>
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"

void print_usage(const char* name);

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

// OCall implementations
void ocall_print(const char* str) {
    printf("%s", str);
}

int main(int argc, char const *argv[]) {
    if( argc < 2)
    {
        print_usage(argv[0]);
        return 0;
    }

    // Get Enclave
    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        std::cout << "Fail to initialize enclave." << std::endl;
        return 1;
    }

    // enter RX mode if "rx" is first arg
    if( std::string(argv[1]) == "rx")
    {
      std::cout << "Entering rx mode: " << std::endl;
      sgx_status_t status = listener_thread(global_eid);
      if (status != SGX_SUCCESS) 
      {
        std::cout << "noob" << std::endl;
      }
      return 0;
    }

    // enter TX mode if "tx" is first arg
    if( std::string(argv[1]) == "tx")
    {
      std::cout << "Entering tx mode. Type your messages and press ENTER to send." << std::endl;
      while (1) {
        char text_buf[128];
        fgets(text_buf, sizeof(text_buf), stdin);
        send_string(global_eid, text_buf);
      }
      return 0;
    }

    // otherwise, print usage
    print_usage(argv[0]);
    return 0;
}

void print_usage(const char* name)
{
    printf("Usage: %s (rx|tx)\n", name);
}

