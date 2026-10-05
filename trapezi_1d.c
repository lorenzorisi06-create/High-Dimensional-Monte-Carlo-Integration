#include <stdio.h>
#include <math.h>


#define PI M_PI

// CALCOLO DEL PIGRECO CALCOLANDO L'INTEGRALE DA -1 A 1 DELLA FUNZIONE DELLA CIRCONFERENZA

double f(double x) {
	return 2.0*sqrt(1.0-x*x);
}

double q(double a, double b, int N) {
	double Q = 0.0;
	double h = (b-a)/N;
	for (int i = 1; i < N; i++) {
		Q += f(a+i*h);
	}
	Q *= 2;
	Q += f(a) + f(b);
	Q *= h/2;
	
	return Q;
}


int main() {
	
	double Q;
	double a = -1.0, b = 1.0;
	
	printf("Fino a quale numero vuoi calcolare? ");
	int n;
	scanf("%d", &n);
	
	
	FILE *pf = fopen("trapezi_1d.txt", "w");
	if (!pf) {
		printf("\nErrore di apertura file!\n");
		return 1;
	}
	for (int N = 3; N <= n; N++) {
		Q = q(a, b, N);
		fprintf(pf, "%d %.9lf %.9lf\n", N, Q, fabs(PI - Q));
	}	
	
	printf("Stima dell'integrale: %.9lf\n", Q);
	printf("Errore: %.9lf", fabs(Q - PI));
	
	fclose(pf);
	
	FILE *gp1 = popen("gnuplot -persist", "w");
	if (!gp1) {
		printf("\nErrore di apertura gnuplot!\n");
		return 1;
	}
	fprintf(gp1, "f(x) = 1000/x**2\n");
	fprintf(gp1, "set logscale y\n");
	fprintf(gp1, "plot 'trapezi_1d.txt' u 1:3 w l title '|I - Q|', f(x) title '1000/M^2'\n");
	fprintf(gp1, "quit\n");
	pclose(gp1);
	
	FILE *gp2 = popen("gnuplot -persist", "w");
	if (!gp2) {
		printf("\nErrore di apertura gnuplot\n");
		return 1;
	}
	fprintf(gp2, "g(x) = %lf\n", PI);
	double ultimo_Q = q(a, b, n);
	double margine = fabs(PI - ultimo_Q) * 10; 
	fprintf(gp2, "set grid\n");
	fprintf(gp2, "set yrange[%lf:%lf]\n", PI - margine, PI + margine);
	fprintf(gp2, "plot 'trapezi_1d.txt' u 1:2 w l title 'Q', g(x) w l lw 2 title 'PI'\n");
	fprintf(gp2, "quit\n");
	pclose(gp2);
	
	
	
	printf("\n\n");
	
	
	
	
	return 0;
}
