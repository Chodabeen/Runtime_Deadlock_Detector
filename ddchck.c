#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h> 

// if it was succeed return 0, otherwise return 1
int read_bytes(int fd, void * buf, size_t len) {
    char * p = buf ;
    size_t acc = 0 ;

    while (acc < len) {
        size_t _read ;
        _read = read(fd, p, len - acc) ;
        // if (_read == 0) {
        //     fprintf(stderr, "FIFO closed");
        //     return 1 ;
        // }
        p += _read ;
        acc += _read ;
    }
    return 0 ;
}

int main () {
    // remove .ddtrace fifo file
	if (unlink(".ddtrace") == 0) {
        printf(".ddtrace removed\n");
    } else {
        perror("remove .ddtrace failed\n");
    }

	// open fifo
    if (mkfifo(".ddtrace", 0666)) {
		if (errno != EEXIST) {
			perror("fail to open fifo: ") ;
			exit(EXIT_FAILURE) ;
		}
	}

	int fd = open(".ddtrace", O_RDONLY | O_SYNC) ;

	while (1) {
		char s[128];
		pthread_t p;
		pthread_mutex_t * mutex;
		int len, pc_addr;
		int flag;

		// read flag
		if (read_bytes(fd, &flag, sizeof(int)) == 1) {
			// fprintf(stderr, "read flag failed\n");
			exit(EXIT_FAILURE);
		}
		fprintf(stderr, "flag: %d\n", flag);

		// read pthread
		if (read_bytes(fd, &p, sizeof(pthread_t)) == 1) {
			fprintf(stderr, "read pthread failed\n");
			exit(EXIT_FAILURE);
		}
    	fprintf(stderr, "thread: %lu\n", p);

		// read mutex
		if (read_bytes(fd, &mutex, sizeof(&mutex)) == 1) {
			fprintf(stderr, "read mutex addr failed\n");
			exit(EXIT_FAILURE);
		}
    	fprintf(stderr, "mutex: %p\n", (void *)mutex);
	
		// read pc addr
		if (read_bytes(fd, &pc_addr, sizeof(int)) == 1) {
			fprintf(stderr, "read pc addr failed\n");
			exit(EXIT_FAILURE);
		}
		fprintf(stderr, "+0x%x\n", pc_addr);

        fprintf(stderr, "\n\n\n");

	}


	close(fd) ;
	return 0 ;
}