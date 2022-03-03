#include <stdio.h>

#define length 3

int main() {
	printf("Applicazione partita!\n");
	int v1[length] = { 3, 4, 5};
       	int v2[length] = { 5, 7, 9};
	int v3[length];
	printf("Ora calcoliamo la somma dei vettori: \n");
	for(int i = 0; i < length; i++){
		v3[i] = v1[i]+v2[i];
		printf("V[%d] = %d\n", i, v3[i]);
	}	
	return 0;
}
