// Calcolo volume sfera approssimando con trapezi l'integrale doppio in [0,1] di sqrt(1-x^2-y^2)
// In ogni spicchio sto calcolando il volume di 1/8 di sfera

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI M_PI

double f(double x, double y) {
	if (x*x + y*y < 1.0) { 
		return sqrt(1-x*x-y*y);
	}
	else {
		return 0;
	}
}

double q(double a, double b, int N) {
	double Q = 0.0;
	double h = fabs(b-a)/N;
	double x = 0;
	double y = 0;
	for (int i = 0; i < N; i++) {
		x = h*i;
		for (int j = 0; j < N; j++) {
			y = h*j;
			Q += f(x, y) + f(x, y+h) + f(x+h, y) + f(x+h, y+h); // altezza trapezio = (vertice1 + vertice2 + vertice3 + vertice4)
		}
	}
	
	return 2*Q*h*h;
}
		

int main() {
	
	double Q;
	double a = 0.0, b = 1.0;
	double V = 4.0*PI/3.0;
	
	int N;
	printf("Inserisci N: ");
	scanf("%d", &N);
	printf("\n");
	
	FILE *pf = fopen("trapezi_2d_sfera.txt", "w");
	if (!pf) {
		printf("\nErrore di apertura file!\n");
		return 1;
	}
	for (int i = 3; i <= N; i++) {
		Q = q(a, b, i);
		fprintf(pf, "%d %.9lf %.9lf\n", i, Q, fabs(Q-V));
	}
	fclose(pf);
	
	printf("Stima del pigreco: %.7lf", Q);
	printf("Errore empirico: %.7lf", fabs(Q - PI));
	
	FILE *gp1 = popen("gnuplot -persist", "w");
	if (!gp1) {
		printf("\nErrore di apertura gnuplot\n");
		return 1;
	}
	fprintf(gp1, "f(x) = 1/x\n");
	fprintf(gp1, "set logscale y\n");
	fprintf(gp1, "h(x) = f(x)/750\n");
	fprintf(gp1, "plot 'trapezi_2d_sfera.txt' u 1:3 w l title 'Trapezi', f(x) w l title '1/N', h(x) w l title '1/750N'\n");
	fprintf(gp1, "quit\n");
	pclose(gp1);
	
	FILE *gp2 = popen("gnuplot -persist", "w");
	if (!gp2) {
		printf("\nErrore di apertura gnuplot\n");
		return 1;
	}
	double margine = 10*fabs(Q-V);
	fprintf(gp2, "set yrange[%f:%f]\n", V - margine, V + margine);
	fprintf(gp2, "set grid\n");
	fprintf(gp2, "g(x) = %f\n", V);
	fprintf(gp2, "plot 'trapezi_2d_sfera.txt' u 1:2 w l title 'Trapezi', g(x) w l lw 2 title 'Volume'\n");
	fprintf(gp2, "quit\n");
	pclose(gp2);
	
	
	return 0;
}
