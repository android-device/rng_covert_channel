
#include"util.hpp" 
#define DELAY 1000
#define SAMP_BUF_SZ 30
#define FILTER 0.2
#define ITER_MAX 1<<30


#define PREAMBLE_LEN 10
#define SUFFIX_LEN 1
#define DATA_LEN 8
#define PACKET_LEN (PREAMBLE_LEN+SUFFIX_LEN+DATA_LEN)
#define PREAMBLE 0x2AA
typedef uint8_t packet_t;

void print_array(int32_t array[], int size);
void shift_in(int32_t val, int32_t array[], int size);
bool search_packet(int32_t array[], int size, packet_t * packet);

int main(int argc, char **argv)
{
	// Put your covert channel setup code here

        double value = 0;
        double threshold_hi = 0.1;
        double threshold_lo = 0.01;
        double bit_threshold = 80;
        int32_t state = 0;
        double value_prev = 0;
        int32_t deltas[SAMP_BUF_SZ];
        int32_t bits[SAMP_BUF_SZ];
        int32_t iteration = 0;
        int32_t delta = 0;
        packet_t packet;

        for (int i=0; i < SAMP_BUF_SZ; i++)
        {
          deltas[i] = 0;
        }

	//printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	//printf("Receiver now listening.\n");

	bool listening = true;
	while (listening) {

		// Put your covert channel code here

                value = (1-FILTER)*value + (FILTER)*(test_n_rdseed(3));

                //delta = iteration - deltas[0];
                //if(delta < 0) delta += ITER_MAX;

                if(state == 0 && value > threshold_hi)
                {
                  //shift_in(+iteration, deltas, SAMP_BUF_SZ);
                  //print_array(deltas, SAMP_BUF_SZ);
                  //printf("\n");
                  //printf("+ %d\n", iteration);
                  iteration = 0;
                  state = 1;
                  //printf("1\n");
                }
                else if(state == 1 && value < threshold_lo)
                {
                  shift_in(-iteration, deltas, SAMP_BUF_SZ);
                  //print_array(deltas, SAMP_BUF_SZ);
                  //printf("\n");
                  //printf("%d\n", iteration);
                  //printf("%d\n", (int)(iteration<bit_threshold));

                  if(iteration < 1000)
                  {
                    bit_threshold = bit_threshold*(0.9999)+iteration*(0.0001);
                    //printf("%f\n", bit_threshold);
                  }
                  shift_in( 
                      (iteration < bit_threshold) ? 1 : 0,
                      bits,
                      SAMP_BUF_SZ);
                  if(search_packet(bits, PACKET_LEN, &packet))
                  {
                    //printf("FOUND_PACKET: %4.4x (%c)\n", packet, packet);
                    printf("%c",packet);
                  }

                  iteration = 0;
                  state = 0;
                  //printf("1\n");
                }

                //if(iteration < 2000)
                //  fprintf(stderr,"%f\n",value);

                nops(DELAY);

                iteration++;
                //if(iteration++ > ITER_MAX) iteration = 0;
	}

	printf("Receiver finished.\n");

	return 0;
}

void shift_in(int32_t val, int32_t array[], int size)
{
  for(int i=size-1; i > 0; i--)
  {
    array[i] = array[i-1];
  }
  array[0] = val;
}

void print_array(int32_t array[], int size)
{
  for(int i=0; i < size; i++)
  {
    printf("%.0d ", array[i]);
  }
}

bool search_packet(int32_t array[], int size, packet_t * packet)
{
  // check preamble
  uint32_t preamble = PREAMBLE;
  packet_t data = 0;
  uint8_t pairity = 0, pairity_expected;
  int32_t i = 0;

  if(size < PACKET_LEN) 
    return false;

  //check preamble
  for(i=PACKET_LEN-1; i >= PACKET_LEN-PREAMBLE_LEN; i--, preamble>>=1)
  {
    if(array[i] != (preamble & 0x1))
      return false;
  }

  //debug aray
  //for(int b=PACKET_LEN-1; b >= 0; b--)
  //  printf("%1d",array[b]);
  //printf("\n");

  // grab data
  for(;i >= SUFFIX_LEN; i--)
  {
    data <<= 1;
    data |= (0x01 & array[i]);
    pairity ^= (array[i] == 0x1);
  }

  // grab pairity
  pairity_expected = array[i];
  // zero out packet so we don't see it again
  for(i=PACKET_LEN-1; i>=0; i--)
  {
    array[i] = 0;
  }

  *packet = data;

  if(pairity_expected != pairity)
    return false;
  else
    return true;
}





