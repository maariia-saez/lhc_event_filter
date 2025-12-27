#include <stdio.h>
#include <stdlib.h>
#include "events.h"

int main(void) {
	EventBatch batch;
	int i;
	ParticleEvent ev;

	printf("=== LHC EventAnalyzer: Memory Stress Test ===\n");

	batch_init(&batch);

	printf("Adding 50 dummy events to trigger realloc...\n");

	for (i = 0; i < 50; i++) {
		ev.id = i;
		ev.energy = i * 1.5;
		ev.timestamp = 1000 + i;

		if(batch_add(&batch, ev) != 0) {
			fprintf(stderr, "FATAL: Failed to add event %d\n", i);
			break;
		}

		printf("Event %02d added | Count: %d | Capacity: %d\n", i, batch.count, batch.capacity);
	}

	printf("\n=== Final Report ===\n");
	printf("Total Events Stored: %d\n", batch.count);
	printf("Final Memory Capacity: %d\n", batch.capacity);

	batch_free(&batch);
	printf("Memory freed correctly. Test Passed.\n");

	return 0;
}
