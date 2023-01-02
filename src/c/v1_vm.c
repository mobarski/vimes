#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "errno.h"

#include "v1_opcodes.h"

char* op_name[] = {"LIT","OPR","LOD","STO","CAL","JMP","JZ","JNZ","INT","EXT","INC"};

// === CORE =======================================================================================================================

#ifndef CORE
	#error CORE parameter must be set to 1|2|3|4
#endif

#if CORE==1
	#include "v1_switch.h"
#elif CORE==2
	#include "v1_repl_switch.h"
#elif CORE==3
	#include "v1_direct.h"
#elif CORE==4
	#include "v1_indirect.h"
#else
	#error Unknown CORE id
#endif

// === END OF CORE ================================================================================================================

typedef struct int_array {
	int* ptr;
	int  cnt;
} int_array;

int_array get_code(char* path) {
	FILE *fp = fopen(path,"rb");
		if (fp==NULL) {
			printf("ERROR: cannot open %s\n", path);
			exit(1);
		}
	fseek(fp,0,SEEK_END);
	int code_size = ftell(fp) / sizeof(int);
	int* code = calloc(code_size, sizeof(int));
		if (code==NULL) {
			printf("ERROR: cannot allocate memory (%d * %d) for code\n",code_size,sizeof(int));
			exit(1);
		}
	fseek(fp,0,SEEK_SET);
	size_t nread = fread(code, sizeof(int), code_size, fp);
		if (nread<code_size) {
			printf("ERROR: read failed - expected %d items, got %d\n", code_size, nread);
			exit(1);
		}
	fclose(fp);
	//
	int_array out;
	out.ptr = code;
	out.cnt = code_size;
	return out;
}

extern char** environ;
int main(int argc, char** argv) {
	if (argc<2) { goto ERROR; }
	
	// ARG: code_path
	int_array code = get_code(argv[1]);
	if (code.ptr==NULL) {
		printf("ERROR: invalid rom path '%s'\n", argv[1]);
		goto ERROR;
	}
	
	// ARG: mem_size
	int mem_size = 1024;
	if (argc>=3) {
		mem_size = strtol(argv[2], NULL, 10);
		if ((errno!=0)||(mem_size<=0)) {
			printf("ERROR: invalid value for mem_size '%s'\n", argv[2]);
			goto ERROR;
		}
	}
	
	// ARG: n
	int n = 0;
	if (argc>=4) {
		n = strtol(argv[3], NULL, 10);
		if ((errno!=0)||(n<0)) {
			printf("ERROR: invalid value for n '%s'\n", argv[3]);
			goto ERROR;
		}
	}

	// XXX
	// printf("CODE: ");
	// for (int i=0; i<code.cnt; i++) {
		// printf("%d ", code.ptr[i]);
	// }
	// printf("\n\n");

	// RUN
	run(code.ptr, n, mem_size, code.cnt);
	free(code.ptr);
	return 0;

ERROR:
	printf("USAGE: vm rom_path [mem_size=1024] [n_iters=0]\n");
	return 1;
}
