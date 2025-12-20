#ifndef EVENTS_H
#define EVENTS_H

#include <stdio.h>

/* * Particle Event Structure
 * Represents a single detection event in the collider.
 */
typedef struct {
	int id;		/* Unique event identifier */
	char type[16];	/* e.g., "PROTON", "MUON", "HIGGS" */
	double energy;	/* Energy un GeV */
	int timestamp;	/* Detection time (logical tick) */
} ParticleEvent;

/* * Resizable Array Structure (Dynamic Memory)
 * Manages a batch of events efficiently.
 */
typedef struct {
	ParticleEvent *events;	/* Pointer to the data array in Heap */
	int capacity;		/* Total allocated slots */
	int count;		/* Currently used slots */
} EventBatch;

/* --- API FUNCTIONS --- */

/* Memory Management */
void batch_init(EventBatch *batch);
void batch_free(EventBatch *batch);
int batch_add(EventBatch *batch, ParticleEvent ev);	/* Uses realloc */

/* I/O Operations */
void load_data(FILE *file, EventBatch *batch);
void generate_report(EventBatch *batch, double energy_threshold);

#endif
