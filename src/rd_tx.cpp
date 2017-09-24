

#include"util.hpp"
#define TICK_HI 50000
#define TICK_LO 10000

void send_preamble();
void send_byte(uint8_t byte);
void tick();

int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	printf("Please type a message.\n");

	bool sending = true;
	while (sending) {
		char text_buf[128];
		fgets(text_buf, sizeof(text_buf), stdin);
	
                send_preamble();
//                send_byte(text_buf[0]);
                //for(int i=0; i < DELAY; i++)
                //{
                //  measure_n_rdseed_time(8); 
                //}
	}

	printf("Sender finished.\n");

	return 0;
}


void tick()
{
  int i;
  for(int i=0; i < TICK_HI; i++)
  {
    //printf("tick\n");
    i += 2;
  }

}

void send_preamble()
{
  int preamble_len = 10;

  for(int i=0; i < preamble_len; i++)
  {
    measure_n_rdseed_time(TICK_LO);
    tick();
  }
}

void send_byte(uint8_t byte)
{
  int preamble_len = 10;

  for(int i=0; i < 8; i++)
  {
    if(byte & 0x01)
      measure_n_rdseed_time(TICK_LO);
    tick();
    byte = byte>>1;
  }
}




