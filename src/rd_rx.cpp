
#include"util.hpp"
#define DELAY 5000
#define SAMP_BUF_SZ 100
#define FILTER 0.1
#define ITER_MAX 1<<30

void print_array(int32_t array[], int size);
void shift_in(int32_t val, int32_t array[], int size);

int main(int argc, char **argv)
{
	// Put your covert channel setup code here

        double value = 0;
        double hysteresis = 0;
        double value_prev = 0;
        int32_t deltas[SAMP_BUF_SZ];
        int32_t iteration = 0;
        int32_t delta = 0;

        for (int i=0; i < SAMP_BUF_SZ; i++)
        {
          deltas[i] = 0;
        }

	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");

	bool listening = true;
	while (listening) {

		// Put your covert channel code here

                value_prev = value;
                value = (1-FILTER)*value + (FILTER)*test_n_rdseed(1);

                //delta = iteration - deltas[0];
                //if(delta < 0) delta += ITER_MAX;

                if(value_prev > threshold && value < threshold)
                {
                  shift_in(-iteration, deltas, SAMP_BUF_SZ);
                  //print_array(deltas, SAMP_BUF_SZ);
                  //printf("\n");
                  printf("- %d\n", iteration);
                  iteration = 0;
                  //printf("1\n");
                }
                else if(value_prev < threshold && value > threshold)
                {
                  shift_in(iteration, deltas, SAMP_BUF_SZ);
                  //print_array(deltas, SAMP_BUF_SZ);
                  //printf("\n");
                  printf("+ %d\n", iteration);
                  iteration = 0;
                  //printf("1\n");
                }

                for(int i=0; i < DELAY; i++);

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


