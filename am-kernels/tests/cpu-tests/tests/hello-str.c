#include "trap.h"
#include <stdio.h>

char buf[128];

int main() {
	printf("%s", "MnZn\n");
	sprintf(buf, "%s", "Hello world!\n");
	check(strcmp(buf, "Hello world!\n") == 0);
	putstr(buf);

	sprintf(buf, "%d-%d-%d\n", 0, 1, 2);
	check(strcmp(buf, "0-1-2\n") == 0);
	putstr(buf);
	
	sprintf(buf, "%d + %d = %d\n", 2, 10, 12);
	check(strcmp(buf, "2 + 10 = 12\n") == 0);
	putstr(buf);

	return 0;
}
