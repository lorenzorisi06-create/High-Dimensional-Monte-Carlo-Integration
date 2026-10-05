#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// STIMA ERRORE INTEGRALE DI f(x) = x^2 IN [0, 1] USANDO MONTECARLO
// INPUT DELL'UTENTE: ERRORE MASSIMO, CONFIDENZA

float my_logf (float);

/* compute inverse error functions with maximum error of 2.35793 ulp */
float my_erfinvf (float a)
{
    float p, r, t;
    t = fmaf (a, 0.0f - a, 1.0f);
    t = my_logf (t);
    if (fabsf(t) > 6.125f) { // maximum ulp error = 2.35793
        p =              3.03697567e-10f; //  0x1.4deb44p-32 
        p = fmaf (p, t,  2.93243101e-8f); //  0x1.f7c9aep-26 
        p = fmaf (p, t,  1.22150334e-6f); //  0x1.47e512p-20 
        p = fmaf (p, t,  2.84108955e-5f); //  0x1.dca7dep-16 
        p = fmaf (p, t,  3.93552968e-4f); //  0x1.9cab92p-12 
        p = fmaf (p, t,  3.02698812e-3f); //  0x1.8cc0dep-9 
        p = fmaf (p, t,  4.83185798e-3f); //  0x1.3ca920p-8 
        p = fmaf (p, t, -2.64646143e-1f); // -0x1.0eff66p-2 
        p = fmaf (p, t,  8.40016484e-1f); //  0x1.ae16a4p-1 
    } else { // maximum ulp error = 2.35002
        p =              5.43877832e-9f;  //  0x1.75c000p-28 
        p = fmaf (p, t,  1.43285448e-7f); //  0x1.33b402p-23 
        p = fmaf (p, t,  1.22774793e-6f); //  0x1.499232p-20 
        p = fmaf (p, t,  1.12963626e-7f); //  0x1.e52cd2p-24 
        p = fmaf (p, t, -5.61530760e-5f); // -0x1.d70bd0p-15 
        p = fmaf (p, t, -1.47697632e-4f); // -0x1.35be90p-13 
        p = fmaf (p, t,  2.31468678e-3f); //  0x1.2f6400p-9 
        p = fmaf (p, t,  1.15392581e-2f); //  0x1.7a1e50p-7 
        p = fmaf (p, t, -2.32015476e-1f); // -0x1.db2aeep-3 
        p = fmaf (p, t,  8.86226892e-1f); //  0x1.c5bf88p-1 
    }
    r = a * p;
    return r;
}

/* compute natural logarithm with a maximum error of 0.85089 ulp */
float my_logf (float a)
{
    float i, m, r, s, t;
    int e;

    m = frexpf (a, &e);
    if (m < 0.666666667f) { // 0x1.555556p-1
        m = m + m;
        e = e - 1;
    }
    i = (float)e;
    /* m in [2/3, 4/3] */
    m = m - 1.0f;
    s = m * m;
    /* Compute log1p(m) for m in [-1/3, 1/3] */
    r =             -0.130310059f;  // -0x1.0ae000p-3
    t =              0.140869141f;  //  0x1.208000p-3
    r = fmaf (r, s, -0.121484190f); // -0x1.f19968p-4
    t = fmaf (t, s,  0.139814854f); //  0x1.1e5740p-3
    r = fmaf (r, s, -0.166846052f); // -0x1.55b362p-3
    t = fmaf (t, s,  0.200120345f); //  0x1.99d8b2p-3
    r = fmaf (r, s, -0.249996200f); // -0x1.fffe02p-3
    r = fmaf (t, m, r);
    r = fmaf (r, m,  0.333331972f); //  0x1.5554fap-2
    r = fmaf (r, m, -0.500000000f); // -0x1.000000p-1
    r = fmaf (r, s, m);
    r = fmaf (i,  0.693147182f, r); //  0x1.62e430p-1 // log(2)
    if (!((a > 0.0f) && (a <= 3.40282346e+38f))) { // 0x1.fffffep+127
        r = a + a;  // silence NaNs if necessary
        if (a  < 0.0f) r = ( 0.0f / 0.0f); //  NaN
        if (a == 0.0f) r = (-1.0f / 0.0f); // -Inf
    }
    return r;
}


double f(double x) {
	return x * x;
}

double generate(void) {
	return (double)rand() / RAND_MAX;
}

double I_N(int N) {
	double I = 0.0;
	double x;
	for (int i = 1; i <= N; i++) {
		x = generate();  // GENERA NUMERO CASUALE TRA 0 E 1
		I += f(x);
	}
	return I / N;
}

double I_media(int M, double *I_campionamenti) {
	double sum = 0.0;
	for (int i = 0; i < M; i++) {
		sum += I_campionamenti[i];
	}
	return sum / M;
}

double var(int M, int N, double *I_campionamenti) {
	double e = 0.0;
	double I_medio = I_media(M, I_campionamenti);
	for (int j = 0; j < M; j++) {  // VOGLIO EFFETTUARE M STIME DI I
		e += (I_campionamenti[j] - I_medio) * (I_campionamenti[j] - I_medio);
	}
	e /= M;
	double v = sqrt(e);
	return v * sqrt(N);
}

double err(double I) {
	return fabs((1.0/3.0) - I);
}
		

int main() {
	
	srand(time(NULL));
	
	// CALCOLO UNA STIMA ACCURATA DELLA VARIANZA MEDIANTE LA VARIANZA CAMPIONARIA
	int N_0 = 1000;
	int M = 10; 
	double I_campionario[M];
	for (int i = 0; i < M; i++) {
		I_campionario[i] = I_N(N_0);
	}
	double sigma = var(M, N_0, I_campionario);
	
	// CHIEDO ALL'UTENTE LA PRECISIONE E L'ACCURATEZZA
	// DA QUI RICAVO IL NUMERO N DI SIMULAZIONI NECESSARIE
	// DATO DAL TEOREMA DEL LIMITE CENTRALE
	double e;
	double c;
	
	printf("\nErrore massimo: ");
	scanf("%lf", &e);
	while (e <= 0) {
		printf("\nImpossibile! Inserisci un altro valore (magari strettamente positivo): ");
		scanf("%lf", &e);
	}
	printf("\nAccuratezza desiderata [in %%]: ");  // Numero in percentuale
	scanf("%lf", &c);
	while ((c <= 0) || (c >= 100)) {
		printf("\nImpossibile! Inserisci un altro valore (magari compreso tra 0 e 100): ");
		scanf("%lf", &c);
	} 
	double s = my_erfinvf(c / 100) * sqrt(2.0);
	
	long long N = (long long)((sigma*sigma*s*s) / (e*e));
	N += 1;
	
	usleep(500000);
	
	printf("\nPer avere un errore inferiore a %g con accuratezza del %.2lf%% sono necessarie %lld iterazioni\n", e, c, N);
	
	usleep(1000000);
	
	printf("\n---CALCOLO INTEGRALE CON MONTECARLO---");
	usleep(300000);
	printf(".");
	usleep(300000);
	printf(".");
	usleep(300000);
	printf(".");
	usleep(500000);
	printf("\n\n");
	
	// CALCOLO INTEGRALE CON MONTECARLO
	double I = 0.0;
	double x;
	
	FILE *pf = fopen("montecarlo_PAC.txt", "w");
	if (!pf) {
		printf("\nErrore di apertura file 'montecarlo_PAC.txt'!\n");
		return 1;
	}
	for (int i = 1; i <= N; i++) {
		x = generate();
		I += f(x);
		fprintf(pf, "%d %lf %lf %lf\n", i, I/i, err(I/i), x);
	}
	
	usleep(500000);
	printf("Integrale stimato: %lf\n", I/N);
	printf("Errore richiesto: %g; errore effettivo: %g", e, err(I/N));
	
	fclose(pf);
		 
	printf("\n\n");
	
	usleep(1000000);
	
	FILE *gp = fopen("montecarlo_PAC.gnp", "w");
	if (!gp) {
		printf("\nErrore di apertura file 'montecarlo_PAC.gnp'!\n");
		return 1;
	}
	
	fprintf(gp, "set xrange [-0.5:1.5]\n"); // Range più centrato su [0,1]
	fprintf(gp, "set yrange [-0.2:1.2]\n");
	fprintf(gp, "set grid\n");
	fprintf(gp, "n = %lld\n", N); // Uso %lld perché N è long long
	fprintf(gp, "f(x) = x*x\n");
	fprintf(gp, "set xzeroaxis lt -1 lc rgb 'black' lw 1.5\n"); // Asse X
	fprintf(gp, "set yzeroaxis lt -1 lc rgb 'black' lw 1.5\n"); // Asse Y
	fprintf(gp, "set tics nomirror\n"); // Rimuove i trattini sui lati opposti
	fprintf(gp, "set arrow from 1,0 to 1,1 nohead lc rgb 'black' dt 2 lw 1\n");

	fprintf(gp, "do for [i=1:n-1:10] {\n"); // Passo 10 per non morire di vecchiaia
	// Estraiamo il valore della stima alla riga i (colonna 2 del file)
	fprintf(gp, "\tstats 'montecarlo_PAC.txt' every ::i::i using 2 nooutput\n");
	fprintf(gp, "\tstima = STATS_min\n"); // STATS_min su una riga singola è il valore stesso
	fprintf(gp, "\tstats 'montecarlo_PAC.txt' every ::i::i using 3 nooutput\n");
	fprintf(gp, "\terr = STATS_min\n");
	fprintf(gp, "\tset title sprintf('Iterazione: %%d | Stima Integrale: %%.6f | Errore: %%.6f', i, stima, err)\n");
	fprintf(gp, "\tplot f(x) lw 2 lc rgb 'blue' title 'f(x)', \\\n");
	fprintf(gp, "\t'montecarlo_PAC.txt' every ::0::i using 4:(f($4)):(0):(-f($4)) with vectors nohead lc rgb 'red' lw 0.1 title 'Punto Corrente'\n");
	fprintf(gp, "\tpause 0.01\n");
	fprintf(gp, "}\n");
	fclose(gp);
	
	char d;
	printf("\nVuoi visualizzare l'andamento della convergenza con gnuplot? [Y/n] ");
	scanf(" %c", &d);
	if ((d == 'Y') || (d == 'y')) {
		system("gnuplot -p 'montecarlo_PAC.gnp'");
	}
	
	
	
	return 0;
}
