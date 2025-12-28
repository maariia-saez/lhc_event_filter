#include <stdio.h>
#include <stdlib.h>
#include "events.h"

/**
 * Main entry point for the LHC Event Analyzer.
 * * Reads data from Standard Input, stores it in dynamic memory,
 * and prints a summary of the loaded events.
 */
int main(void) {
	EventBatch batch;

	/* 1. Initialize the batch structure */
	batch_init(&batch);

	/*2. Load dataa directly from Standard Input (Pipe )*/
	fprintf(stderr, "[INFO] Analyzer started. Waiting for data...\n");
	load_data(stdin, &batch);

	/* 3. Generate report */
	generate_report(&batch, 500.0);

	/* 4. Cleanup */
	batch_free(&batch);
	fprintf(stderr, "[INFO] Memory released. Exiting.\n");

	return 0;
}
