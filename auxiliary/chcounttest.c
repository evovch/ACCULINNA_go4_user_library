#include <stdio.h>

int main(int argc, char** argv)
{
	int detch;
	int electrch;

	int geo=0;
	int startdetch=0;
	int startelectrch=0;
	int stepelecrtch=2;
	int nelectrch=32;

	detch = startdetch;
	for (electrch = startelectrch; electrch < nelectrch; electrch += stepelecrtch) {
		printf("geo=%d\telectrch=%d\tdetch=%d\n", geo, electrch, detch);
		detch++;
	}

	geo=0;
	startdetch=0;
	startelectrch=1;
	stepelecrtch=2;
	nelectrch=32;

	detch = startdetch;
	for (electrch = startelectrch; electrch < nelectrch; electrch += stepelecrtch) {
		printf("geo=%d\telectrch=%d\tdetch=%d\n", geo, electrch, detch);
		detch++;
	}

	geo=1;
	startdetch=16;
	startelectrch=0;
	stepelecrtch=2;
	nelectrch=32;

	detch = startdetch;
	for (electrch = startelectrch; electrch < nelectrch; electrch += stepelecrtch) {
		printf("geo=%d\telectrch=%d\tdetch=%d\n", geo, electrch, detch);
		detch++;
	}

	geo=1;
	startdetch=16;
	startelectrch=1;
	stepelecrtch=2;
	nelectrch=32;

	detch = startdetch;
	for (electrch = startelectrch; electrch < nelectrch; electrch += stepelecrtch) {
		printf("geo=%d\telectrch=%d\tdetch=%d\n", geo, electrch, detch);
		detch++;
	}

	return 0;
}
