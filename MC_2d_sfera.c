#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define PI M_PI


int insphere(double x, double y, double z) {
	return (x*x+y*y+z*z <= 1);
}

double generate(void) {
	return -1.0 + 2.0 * (double)rand()/(double)RAND_MAX;
}


int main() {
	
	srand(time(NULL));
	int N;
	printf("Inserisci N: ");
	scanf("%d", &N);
	int S = 0;
	double x, y, z;
	double V = 4.0*PI/3.0;
	double MC;
	
	FILE *pf = fopen("montecarlo_2d_sfera.txt", "w");
	if (!pf) {
		printf("\nErrore di apertura file!\n");
		return 1;
	}
	for (int i = 1; i <= N; i++) {
		x = generate();
		y = generate();
		z = generate();
		S += insphere(x, y, z);
		MC = 8.0*((double)S)/i;  // A(sfera) / A(quadrato) = S/N --> A(sfera) = S/N * 8
		fprintf(pf, "%d %d %f %f %f %f %f\n", i, S, MC, fabs(MC - V), x, y, z);
	}
	fclose(pf);
	
	FILE *gp1 = popen("gnuplot -persist", "w");
	if (!gp1) {
		printf("\nErrore di apertura gnuplot\n");
		return 1;
	}
	fprintf(gp1, "f(x) = 1/sqrt(x)\n");
	fprintf(gp1, "set logscale y\n");
	fprintf(gp1, "plot 'montecarlo_2d_sfera.txt' u 1:4 w l title 'Monte Carlo', f(x) w l title '1/N'\n");
	fprintf(gp1, "quit\n");
	pclose(gp1);
	
	FILE *gp2 = popen("gnuplot -persist", "w");
	if (!gp2) {
		printf("\nErrore di apertura gnuplot!\n");
		return 1;
	}
	fprintf(gp2, "g(x) = %f\n", V);
	double margine = 5*fabs(V-8*(double)S/N);
	fprintf(gp2, "set yrange[%f:%f]\n", V - margine, V + margine);
	fprintf(gp2, "plot 'montecarlo_2d_sfera.txt' u 1:3 w l title 'Monte Carlo', g(x) w l lw 2 title 'Volume'\n");
	fprintf(gp2, "quit\n");
	pclose(gp2);
	
	
	return 0;
}
