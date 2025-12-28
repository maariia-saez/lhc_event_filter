/**
 * @file generator.c
 * @brief Synthetic Data Generator for the LHC Event Filter project.
 *
 * This proggram simulates a High-Throughput Data Acquisition (DAQ) sensor.
 * It generates random particle detectcion events and streams them to Standard Output (stdout).
 *
 * Usage:
 * ./bin/generator [num_events] > output_file.dat
 *
 * Output Format:
 * ID TYPE ENERGY TIMESTAMP
 *
 * Example:
 * 1045 PROTON 450.5000 1600000000
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Default number of events if no argument is provided */
#define DEFAULT_EVENTS 10000

/* Available particle types for simulation */
const char *PARTICLES[] = {
	"PROTON",
	"NEUTRON",
	"ELECTRON",
	"MUON",
	"PION",
	"HIGGS"
};

/* Total number of particle types defined above */
#define NUM_PARTICLE_TYPES 6

/**
 * Generates a random double precision number within a specific range.
 *
 * @param min Minimum value (inclusive).
 * @param max Maximum value (inclusive).
 * @return A random double between min and max.
 */
double rand_double(double min, double max) {
	/* Calculate a scaling factor between 0.0 and 1.0 */
	double scale = rand() / (double) RAND_MAX;
	/* Map to the desired range */
	return min + scale * (max - min);
}

/**
 * Main entry point for the generator.
 *
 * @param argc Argument count.
 * @param argv Argument vector. argv[1] (optional) is the number of events to generate.
 * @return 0 on success.
 */
int main(int argc, char *argv[]) {
	int i;
	int num_events = DEFAULT_EVENTS;
	int id, type_idx, timestamp;
	double energy;

	/* Initialize the random number generator using the current time as seed.
	 * This ensures different data sequences on every run.
	 */
	srand(time(NULL));

	/* Parse command line arguments */
	if (argc > 1) {
		/* Convert string argument to integer. */
		num_events = atoi(argv[1]);
	}

	/* Inform the user via Standard Error (stderr) so it doesn't pollute the data stream */
	fprintf(stderr, "[GENERATOR] Starting simulation of %d events...\n", num_events);

	/* Main Generation Loop */
	for (i = 0; i < num_events; i++){
		/* 1. ID: Sequential identifier */
		id = i + 1;

		/* 2. TYPE: Random selection from the PARTICLES array*/
		type_idx = rand() % NUM_PARTICLE_TYPES;

		/* 3. ENERGY: Random value between 0.5 GeV and 1000.0 GeV */
		energy = rand_double(0.5, 1000.0);

		/* 4. TIMESTAMP: Simulated logical clock (incrementing slightly) */
		/* Base time + sequence offset + small jitter */
		timestamp = 1600000000 + (i * 2) + (rand() % 5);

		/* OUTPUT: Print formatted data to Standard Outout (stdout).
		 * This allows the user to redirect data to a file using '>'
		 * or pipe it to another proggram using '|'.
		 */
		printf("%d %s %.4f %d\n", id, PARTICLES[type_idx], energy, timestamp);

	}

	fprintf(stderr, "[GENERATOR] Successfully created %d events.\n", num_events);

	return 0;
}
