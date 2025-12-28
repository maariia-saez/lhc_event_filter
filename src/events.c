#include "events.h"
#include <stdlib.h>
#include <string.h>

/**
 * Initializes a ner EventBatch structure.
 * Sets the internal pointer to NULL and resets counters to zero.
 * This function must be called before using any EventBatch to ensure
 * a clean state and void undefined behaviour with uninitialized pointers.
 *
 * @param batch Pointer to the EventBatch structure to be initialized.
 * If NULL, the function does nothing.
 */
void batch_init(EventBatch * batch) {
	/* Safety check: ensure the pointer is valid */
	if (batch == NULL) {
		return;
	}

	batch->events = NULL; /* No memory allocated yet*/
	batch->capacity = 0; /* Zero capacity */
	batch->count = 0; /* Zero elements */
}

/**
 * Releases the memory allocated for the event batch.
 * * Frees the dynamic array if it exists.
 * * Resets the batch to a clean state (capacity=0, count=0) using batch_init.
 * This prevents "dangling pointers" by ensuring the pointer is NUll after freeing.
 *
 * @param batch Pointer to the EventBatch to be freed.
 */
void batch_free(EventBatch *batch) {
	/* Safety check: ensure batch itself is not NULL */
	if (batch->events != NULL) {
		free(batch->events);
	}

	/* Reuse init to reset counters and pointer */
	batch_init(batch);
}

/**
 * Adds a new particle event to the batch.
 *
 * This function manages the dynamic memory of the batch using an exponential
 * growth strategy (doubling capacity).
 *
 * Algorithm:
 * 1. Checks if the current capacity is exhausted.
 * 2. If full, calculates a new capacity (starts at 10, then doubles).
 * 3. Reallocates memory using a temporary pointer to ensure exception safety.
 * (In realloc fails, the original data is preserved).
 * 4. Updates the batch structure only if reallocation succeeds.
 * 5. Appends the new event to the end of the array.
 *
 * @param batch Pointer to the EventBatch structure.
 * @param ev The ParticleEvent data to be added.
 * @return 0 on success, -1 on memory allocation failure.
 */
int batch_add(EventBatch *batch, ParticleEvent ev) {
	/* Check if the array is full */
	if (batch->count >= batch->capacity) {
		int new_capacity;
		ParticleEvent *aux;

		/* Calculate new capacity: Start with 10, otherwise double it */
		if (batch->capacity == 0) {
			new_capacity = 10;
		} else {
			new_capacity = 2 * batch->capacity;
		}

		/* Attempt to resize memory.
		 * CRITICAL: I use a temporary pointer (aux) to protect data if realloc fails.
		 */
		aux = realloc(batch->events, new_capacity * sizeof(ParticleEvent));

		/* HAndle allocation failure */
		if (aux == NULL) {
			/* Return error without modifying the original batch */
			return -1;
		}

		/* Success: Update the batch structure with new memory and capacity */
		batch->events = aux;
		batch->capacity = new_capacity;
	}

	/* Append the event to the next available slot and increment counter */
	batch->events[batch->count] = ev;
	batch->count++;

	return 0;
}

/**
 * Parses raw data from a file stream and loads it into the batch.
 *
 * This function read text line-by-line using fgets (safe reading).
 * It parses each line looking for the format: "ID TYPE ENERGY TIMESTAMP".
 * If a line is valid (4 fields parsed), it's added to the batch.
 * Invalid lines are reported to stderr but do not stop execution.
 *
 * @param file Input file stream (can be stdin or a real file).
 * @param batch Pointer to the destination EventBatch.
 */
void load_data(FILE *file, EventBatch *batch) {
	char buffer[256];
	ParticleEvent ev;
	int parsed_fields;
	int line_num = 0; /* Counter to track position in file for error report */

	/* Read the file line by line until EOF is reached. */
	while (fgets(buffer, sizeof(buffer), file) != NULL ){
		line_num++;

		/* Parse the line. 4 exact fields are expected:
		 * %d -> Integer ID
		 * %15s -> String (max 15 chars to prevent overflow)
		 * %lf -> Double precision float
		 * %d -> Integer timestamp
		 */
		parsed_fields = sscanf(buffer, "%d %15s %lf %d", &ev.id, ev.type, &ev.energy, &ev.timestamp);

		/* Validate that we got exactly the 4 expected pieces of data */
		if (parsed_fields != 4) {
			/* Report error to stderr so it doesn't pollute standard output
			 * Line number is included to help debugging
			 */
			fprintf(stderr, "[WARNING] Parsing error at line %d: Invalid format\n", line_num);
		} else {
			/* Data is valid: Attempt to store it in the dynamic array */
			if (batch_add(batch, ev) != 0) {
				fprintf(stderr, "[ERROR] Memory allocation failed at line %d\n", line_num);
				break; /* Critical failure, stop processing */
			}
		}
	}
}

/**
 * Analyzes the batch and prints a statistical report to stdout.
 *
 * It calculates the following metrics:
 * - Total number of events processed.
 * - Count of specific particle types (e.g HIGGS).
 * - Number of events exceeding the energy threshold (High Energy Physics).
 * - Mean energy of the collision batch.
 * - The single event with the highest record energy (Peak Signal).
 *
 * @param batch Pointer to the EventBatch structure containing the data.
 * @param energy_threshold Miminum energy (in GeV) to classify an event as "high energy".
 */
void generate_report(EventBatch *batch, double energy_threshold) {
	double total_energy = 0;
	double max_energy = -1.0;
	int max_energy_id = -1;
	int higgs_count = 0;
	int high_energy_count = 0;
	int i;
	ParticleEvent ev;

	/* Safety Check: Ensure is data to analyze */
        if (batch->count == 0) {
                fprintf(stderr, "[ERROR] No data to analyze\n");
                return;
        }


	/* Main Analysis Loop: Iterate through all stored events */
	for (i = 0; i < batch->count; i++) {
		ev = batch->events[i];

		/* 1. Acumulate Total Energy (for Mean calculation) */
		total_energy += ev.energy;

		/* 2. Peak Detection: Check if this is the most energetuc event so far */
		if (ev.energy > max_energy) {
			max_energy = ev.energy;
			max_energy_id = ev.id;
		}

		/* 3. Particle Classification: Looks for rare 'HIGGS' particles. */
		if (strcmp(ev.type, "HIGGS") == 0) {
			higgs_count++;
		}

		/* 4. Energy Filter: Count events above the user-defined threshold*/
		if (ev.energy > energy_threshold) {
			high_energy_count++;
		}
	}

	/* Final Output: Print statistics to Standard Output */
	printf("=== Analysis Report ===\n");
	printf("Events stored = %d\n", batch->count);
	printf("Higgs detected = %d\n", higgs_count);
	printf("Events above %.2f GeV: %d\n", energy_threshold, high_energy_count);
	printf("Mean Energy = %.4f GeV\n", total_energy/batch->count);
	printf("Highes Energy Event: ID %d (Energy: %.4f GeV)\n)", max_energy_id, max_energy);
}
