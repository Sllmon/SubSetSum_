#include "../util/subsum.h"

int main(){
	pair_t * P1;
	word_t val, x;

	//word initialisation
	word_init_str(val, "123456789");
	word_init_str(x, "15");

	//Alloc P1
	P1 = pair_alloc();

	//Set P1 to values:
	pair_setValue(P1, x, val);

	//Free P1
	pair_free(P1);
	word_clear(x);
	word_clear(val);
	return 0;
}