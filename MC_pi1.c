/**
 * @file montecarlo_pi.c
 * @brief PI estimation via Monte Carlo integration in a 2D domain.
 * 
 * This program estimates the value of PI by sampling random points in 
 * the square [-1,1]x[-1,1] and calculating the ratio of points falling 
 * within the unit circle.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Checks if a 2D point is inside the unit circle.
 * @return 1 if inside (or on boundary), 0 otherwise.
 */
int is_inside_circle(double x, double y) {
    return (x * x + y * y) <= 1.0;
}

/**
 * @brief Generates a pseudo-random double in the range [-1.0, 1.0].
 */
double get_random_coordinate(void) {
    return -1.0 + 2.0 * ((double)rand()) / ((double)RAND_MAX);
}

int main() {
    long n_samples;
    long hits = 0; // Points inside the circle (S)
    
    printf("\n========================================\n");
    printf("   Monte Carlo PI Estimator             \n");
    printf("========================================\n");
    printf("Enter number of samples to generate: ");
    
    // Input validation
    if (scanf("%ld", &n_samples) != 1) return 1;
    srand((unsigned int)time(NULL));

    // Files for data persistence and visualization
    FILE *f_convergence = fopen("convergence_data.txt", "w");
    FILE *f_animation = fopen("animation_points.txt", "w");
    
    if (!f_convergence || !f_animation) {
        fprintf(stderr, "Error: Could not open output files for writing.\n");
        return 1;
    }

    double x, y, pi_estimate;

    printf("\nRunning simulation...\n");

    for (long i = 1; i <= n_samples; i++) {
        x = get_random_coordinate();
        y = get_random_coordinate();
        
        if (is_inside_circle(x, y)) {
            hits++;
        }

        pi_estimate = 4.0 * (double)hits / i;
        double error = fabs(pi_estimate - M_PI);

        // Store data: [Iteration, Hits, Absolute Error, PI Estimate]
        fprintf(f_convergence, "%ld %ld %.9lf %.9lf\n", i, hits, error, pi_estimate);
        
        // Store animation data: [Iteration, Hits, x, y, Current Estimate]
        fprintf(f_animation, "%ld %ld %lf %lf %lf\n", i, hits, x, y, pi_estimate);
    }

    double final_estimate = 4.0 * (double)hits / n_samples;
    double final_error = fabs(M_PI - final_estimate);

    printf("Simulation Complete.\n");
    printf("Final Estimate: %.10f\n", final_estimate);
    printf("Absolute Error: %.10e\n", final_error);
    printf("========================================\n");

    fclose(f_convergence);
    fclose(f_animation);

    // --- Visualization Logic (Gnuplot) ---
    char choice;
    printf("Display convergence plots? [y/N]: ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        // Plot 1: Error Convergence vs 1/sqrt(N)
        FILE *gp1 = popen("gnuplot -persist", "w");
        if (gp1) {
            fprintf(gp1, "set title 'Error Convergence: Monte Carlo vs Theoretical O(1/sqrt(N))'\n");
            fprintf(gp1, "f(x) = 1.0/sqrt(x)\n");
            fprintf(gp1, "set logscale xy\n");
            fprintf(gp1, "set xlabel 'Samples (N)'\n");
            fprintf(gp1, "set ylabel 'Absolute Error'\n");
            fprintf(gp1, "plot 'convergence_data.txt' u 1:3 w l title 'MC Error', f(x) w l dt 2 title '1/sqrt(N) trend'\n");
            pclose(gp1);
        }

        // Plot 2: Estimate Stability
        FILE *gp2 = popen("gnuplot -persist", "w");
        if (gp2) {
            double margin = final_error * 10.0;
            fprintf(gp2, "set title 'PI Estimate Stability'\n");
            fprintf(gp2, "set grid\n");
            fprintf(gp2, "set xlabel 'Samples (N)'\n");
            fprintf(gp2, "set yrange [%lf:%lf]\n", M_PI - margin, M_PI + margin);
            fprintf(gp2, "plot 'convergence_data.txt' u 1:4 w l title 'MC Estimate', %lf w l lw 2 title 'True PI'\n", M_PI);
            pclose(gp2);
        }
    }

    // --- Gnuplot Script Generation for Animation ---
    FILE *f_anim_script = fopen("render_animation.gnp", "w");
    if (f_anim_script) {
        fprintf(f_anim_script, "unset key\nset size square\nset size ratio 1\n");
        fprintf(f_anim_script, "n = %ld\n", n_samples);
        fprintf(f_anim_script, "get_color(x, y) = (x**2 + y**2 <= 1.0 ? 0x0000FF : 0xFF0000)\n"); // Blue if inside, Red if outside
        fprintf(f_anim_script, "set object 1 rectangle from -1, -1 to 1,1 front fs empty border lc rgb 'black'\n");
        fprintf(f_anim_script, "set object 2 circle at 0,0 size 1.0 fc rgb 'blue' fs empty border lc rgb 'blue'\n");
        fprintf(f_anim_script, "do for [i=1:n:100] {\n");
        fprintf(f_anim_script, "\tstats 'animation_points.txt' every ::i::i using 2 nooutput\n");
        fprintf(f_anim_script, "\tS = STATS_min\n");
        fprintf(f_anim_script, "\tstats 'animation_points.txt' every ::i::i using 5 nooutput\n");
        fprintf(f_anim_script, "\tpi_est = STATS_min\n");
        fprintf(f_anim_script, "\tset title sprintf('Samples: %%d | Hits: %%d | PI approx: %%.4f', i, S, pi_est)\n");
        fprintf(f_anim_script, "\tplot 'animation_points.txt' every ::0::i using 3:4:(get_color($3,$4)) with points pt 7 ps 0.2 lc rgb variable\n");
        fprintf(f_anim_script, "\tpause 0.01\n}\n");
        fprintf(f_anim_script, "set xrange[-1.2:1.2]\nset yrange[-1.2:1.2]\n");
        fprintf(f_anim_script, "pause -1 'Animation complete. Press Enter to exit.'\n");
        fclose(f_anim_script);
    }

    printf("Display real-time sampling animation? [y/N]: ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        system("gnuplot -p render_animation.gnp");
    }

    printf("\nProgram exited successfully.\n\n");
    return 0;
}
