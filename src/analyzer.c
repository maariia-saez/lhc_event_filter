#include <stdio.h>
#include <stdlib.h>
#include "events.h"

int main(void) {
	EventBatch batch;

	printf("[INFO] Analyzer started.\n");

	/* Just testing the header inclusion */
	batch_init(&batch);
	printf("[INFO] Batch initialized (Logic pending).\n");

	return 0;
}
