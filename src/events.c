#include "events.h"
#include <stdlib.h>

void batch_init(EventBatch * batch) {
	/* To supress unused parameter warning during skeleton phase */
	(void)batch;
}

void batch_free(EventBatch *batch) {
	(void)batch;
}

int batch_add(EventBatch *batch, ParticleEvent ev) {
	(void)batch;
	(void)ev;
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
