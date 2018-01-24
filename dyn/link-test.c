#define DYN_VERBS_NO_DLOPEN 1
#include "dyn-verbs.c"

int main(int argc, const char *argv[])
{
	hard_link_init();

	return 0;
}
