//#include <stdio.h>

int main() {
	// simple statementsi
	int  a = 2;
	int  b = 3;
	int *c = &a;
	c = &b;
	float d = 0.5f;
	float* e = &d;
	float* f;
	// simple branch
	if (a==3) {
		e = &d;
	} else {
		f = &d;
	}
	float **g;
	//simple loop
	for (int i = 0 ; i < 10 ; i++) {
		if (i==5)
			g = &e;
		if (i==9)
			g = &f;
	}
	return 0;
}
