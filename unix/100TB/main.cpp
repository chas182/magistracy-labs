#include <sys/mman.h>
#include <stdio.h>

int main(int argc, char** argv) {

	size_t size = (unsigned long)100*1024*1024*1024*1024;

	void* p = mmap(NULL, size, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
	if (p == MAP_FAILED) {
       		perror("mmap");
		return 1;
	}

	getchar();

	if (munmap(p, size) == -1) {
		perror("munmap");
		return 1;
	}

	return 0;
}