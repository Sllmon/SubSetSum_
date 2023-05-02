#include "../util/subsum.h"

int main(){
	word_t x;

	word_init_str(x, "13"); //vect [1,0,1,1,0,0,0,0] en dim 8
	word_dispBinVector(x, 8);

	word_clear(x);
	
	return 0;
}