#include "events.h"
#include <stdlib.h>


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

void load_data(FILE *file, EventBatch *batch) {
	(void)file;
	(void)batch;
}

void generate_report(EventBatch *batch, double energy_threshold) {
	(void)batch;
	(void)energy_threshold;
}
