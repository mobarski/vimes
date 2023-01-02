#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "errno.h"

#include "v1_opcodes.h"

char* op_name[] = {"LIT","OPR","LOD","STO","CAL","JMP","JZ","JNZ","INT","EXT","INC"};

// === CORE =======================================================================================================================

long long run(int* code, int n, int mem_size, int code_size) {
	setbuf(stdout, NULL);
	//printf("run n %d mem_size %d code_size %d\n",n,mem_size,code_size);
	
	int ip = 0;  // instruction pointer
	int sp = 0;  // stack pointer
	int rp = mem_size-1; // return stack pointer (ret-stack grows down)
	int fp = rp; // frame pointer
	long long ic = 0;  // instruction counter (64-bit)
	int *mem = calloc(mem_size, sizeof(int));
	//
	clock_t start = clock();
	clock_t end;
	double dt_ms;
	
	int t; // temporary value (ie for top-of-stack)
	char* args[] = {"P0","P1","P2","P3","P4","P5","P6","P7"};
	
	void* P_INS[] = {&&_LIT,&&_OPR,&&_LOD,&&_STO,&&_CAL,&&_JMP,&&_JZ,&&_JNZ,&&_INT,&&_EXT,&&_INC};
	void* P_OPR[] = {&&_HLT,&&_RET,&&_ADD,&&_SUB,&&_MUL,&&_DIV,&&_MOD,0,0,0,&&_LT,&&_GT,&&_EQ,&&_NE,&&_LE,&&_GE};
	void* P_EXT[] = {&&_AST,&&_DOT,&&_ARG};
	
	#define NEXT ic++; goto *P_INS[code[ip]]
	#define NEXTxxx goto *prog[ip]
	#define STACK printf("STACK: "); for (int ii=0;  ii<sp;    ii++) { printf("%d ", mem[ii]); }
	#define CODE  printf("CODE: ");  for (int ii=ip; ii<ip+10; ii++) { printf("%d ", code[ii]); }
	#define EOL   printf("\n")
	#define TAB   printf("\t\t")
	#define INFOxxx if (((n>0) && (ic >= n)) || (sp>rp)) goto _STOP; printf("ip %d\t\top %d\t\t%s\t\ta %d\t\tsp %d\t\ttos %d\t\trp %d\t\tfp %d\t\tic %d\t\t",ip,code[ip],op_name[code[ip]],code[ip+1],sp,sp>0?mem[sp-1]:-1,rp,fp,ic); STACK; EOL
	#define INFO 
	#define a code[ip+1]
	//{
		NEXT;

		_LIT: INFO; mem[sp++]=a;           ip+=2;                        NEXT;
		_LOD: INFO; mem[sp++]=mem[fp-a];   ip+=2;                        NEXT;
		_STO: INFO; mem[fp-a]=mem[--sp];   ip+=2;                        NEXT;
		_INT: INFO; rp-=a;                 ip+=2;                        NEXT;
		_JMP: INFO; ip+=a;                                               NEXT;
		_JZ:  INFO; ip+=(mem[--sp]==0) ? a:2;                            NEXT;
		_JNZ: INFO; ip+=(mem[--sp]!=0) ? a:2;                            NEXT;
		_INC: INFO; mem[fp-a]+=code[ip+2]; ip+=3;                        NEXT;
		_CAL:
			INFO;
			mem[rp]=fp;
			mem[rp-1]=ip+2;
			ip=code[ip+1];
			rp-=2;
			fp=rp;
			NEXT;
		
		_OPR: goto *P_OPR[code[ip+1]];
		_EXT: goto *P_EXT[code[ip+1]];
		
		// OPR
		_ADD: INFO; mem[sp-2] += mem[sp-1]; sp-=1; ip+=2;                    NEXT;
		_SUB: INFO; mem[sp-2] -= mem[sp-1]; sp-=1; ip+=2;                    NEXT;
		_MUL: INFO; mem[sp-2] *= mem[sp-1]; sp-=1; ip+=2;                    NEXT;
		_DIV: INFO; mem[sp-2] /= mem[sp-1]; sp-=1; ip+=2;                    NEXT;
		_MOD: INFO; mem[sp-2] %= mem[sp-1]; sp-=1; ip+=2;                    NEXT;
		_LT:  INFO; mem[sp-2]  = mem[sp-2]  < mem[sp-1] ? 1:0; sp-=1; ip+=2; NEXT;
		_GT:  INFO; mem[sp-2]  = mem[sp-2]  > mem[sp-1] ? 1:0; sp-=1; ip+=2; NEXT;
		_EQ:  INFO; mem[sp-2]  = mem[sp-2] == mem[sp-1] ? 1:0; sp-=1; ip+=2; NEXT;
		_NE:  INFO; mem[sp-2]  = mem[sp-2] != mem[sp-1] ? 1:0; sp-=1; ip+=2; NEXT;
		_LE:  INFO; mem[sp-2]  = mem[sp-2] <= mem[sp-1] ? 1:0; sp-=1; ip+=2; NEXT;
		_GE:  INFO; mem[sp-2]  = mem[sp-2] >= mem[sp-1] ? 1:0; sp-=1; ip+=2; NEXT;
		_HLT: goto _STOP;
		_RET: INFO; ip=mem[fp+1]; rp=fp+2; fp=mem[fp+2]; INFO;        NEXT;
		
		// EXT
		_ARG:
			INFO;
			t = mem[sp-1];
			mem[sp-1] = ((t>=0)&&(t<=7)) ? atoi(getenv(args[t])) : 0;
			//printf("DEBUG: ARG %d %s = %d\n", t, args[t], mem[sp-1]); // XXX
			ip+=2;
			                                                  NEXT;
		_DOT: INFO; printf("%d\n",mem[sp-1]); ip+=2;                NEXT;
		_AST:
			INFO;
			if (mem[sp-1]==mem[sp-2]) {
				sp-=2; ip+=2; NEXT;
			} else {
				printf("ERROR: expected %d got %d instead\n",mem[sp-1],mem[sp-2]);
				goto _STOP;
			}
	//}
	_STOP:
	end = clock();
	dt_ms = 1000*((double)(end-start)) / CLOCKS_PER_SEC;
	printf("STATUS: ip %d sp %d rp %d fp %d ic %lld dt %d ms tos %d\n",ip,sp,rp,fp,ic,(int)dt_ms, sp>0?mem[sp-1]:0);
	//free(mem);
	//free(prog);
	return ic;
}

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
