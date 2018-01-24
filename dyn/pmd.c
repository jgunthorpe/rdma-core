#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <infiniband/verbs.h>

#ifndef _INFINIBAND_DYN_VERBS_H
static inline int dl_link_init(void)
{
	return 0;
}
#endif

static void demo(void)
{
	struct ibv_device **dev_list;
	struct ibv_context *context;
	struct ibv_cq *cq;
	int rc;

	dev_list = ibv_get_device_list(NULL);
	assert(dev_list != NULL);
	context = ibv_open_device(dev_list[0]);
	assert(context != NULL);

	cq = ibv_create_cq(context, 100, NULL, NULL, 0);
	assert(cq != NULL);

	rc = ibv_destroy_cq(cq);
	assert(rc == 0);

	cq = ibv_create_cq(context, 100, NULL, NULL, 0);
	assert(cq != NULL);

	ibv_destroy_cq(cq);
	assert(rc == 0);

	rc = ibv_close_device(context);
	assert(rc == 0);

	ibv_free_device_list(dev_list);
}

int main(int argc, const char *argv[])
{
	if (dl_link_init() != 0) {
		printf("Failed!\n");
		return EXIT_FAILURE;
	}

	demo();

	return EXIT_SUCCESS;
}
