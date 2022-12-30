#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "errno.h"

#include "v1_opcodes.h"

// === CORE =======================================================================================================================

#define DISPATCH \
	if ((n>0) && (ic >= n)) goto _STOP; \
	ic++; \
	op = code[ip]; \
	a = code[ip+1]; \
	switch (op) { \
		case LIT: goto _LIT; \
		case LOD: goto _LOD; \
		case STO: goto _STO; \
		case INT: goto _INT; \
		case JMP: goto _JMP; \
		case JZ:  goto _JZ; \
		case JNZ: goto _JNZ; \
		case INC: goto _INC; \
		case CAL: goto _CAL; \
		case OPR: \
			ip+=2; \
			switch (a) { \
				case ADD: goto _ADD; \
				case SUB: goto _SUB; \
				case MUL: goto _MUL; \
				case DIV: goto _DIV; \
				case MOD: goto _MOD; \
				case LT:  goto _LT; \
				case GT:  goto _GT; \
				case EQ:  goto _EQ; \
				case LE:  goto _LE; \
				case GE:  goto _GE; \
				case NE:  goto _NE; \
				case HLT: goto _HLT; \
				case RET: goto _RET; \
			} \
		case EXT: \
			ip+=2; \
			switch (a) { \
				case DOT: goto _DOT; \
				case AST: goto _AST; \
				case ARG: goto _ARG; \
			} \
	}


int run(int* code, int n, int mem_size) {
	int ip = 0;  // instruction pointer
	int sp = 0;  // stack pointer
	int rp = mem_size-1; // return stack pointer (ret-stack grows down)
	int fp = rp; // frame pointer
	long long ic = 0;  // instruction counter (64-bit)
	int *mem = calloc(mem_size, sizeof(int));
	clock_t start = clock();
	clock_t end;
	
	int t; // temporary value (ie for top-of-stack)
	char* args[] = {"P0","P1","P2","P3","P4","P5","P6","P7"};
	
	{
		int op;
		int a;
		
		DISPATCH;

		_LIT: mem[sp++]=a;           ip+=2; DISPATCH;
		_LOD: mem[sp++]=mem[fp-a];   ip+=2; DISPATCH;
		_STO: mem[fp-a]=mem[--sp];   ip+=2; DISPATCH;
		_INT: rp-=a;                 ip+=2; DISPATCH;
		_JMP: ip+=a;                        DISPATCH;
		_JZ:  ip+=(mem[--sp]==0) ? a:2;     DISPATCH;
		_JNZ: ip+=(mem[--sp]!=0) ? a:2;     DISPATCH;
		_INC: mem[fp-a]+=code[ip+2]; ip+=3; DISPATCH;
		_CAL: mem[rp]=fp; mem[rp-1]=ip+2; ip=a; rp-=2; fp=rp; DISPATCH;
		// OPR
		_ADD: sp--; mem[sp-1] += mem[sp];                   DISPATCH;
		_SUB: sp--; mem[sp-1] -= mem[sp];                   DISPATCH;
		_MUL: sp--; mem[sp-1] *= mem[sp];                   DISPATCH;
		_DIV: sp--; mem[sp-1] /= mem[sp];                   DISPATCH;
		_MOD: sp--; mem[sp-1] %= mem[sp];                   DISPATCH;
		_LT:  sp--; mem[sp-1] = mem[sp-1]  < mem[sp] ? 1:0; DISPATCH;
		_GT:  sp--; mem[sp-1] = mem[sp-1]  > mem[sp] ? 1:0; DISPATCH;
		_EQ:  sp--; mem[sp-1] = mem[sp-1] == mem[sp] ? 1:0; DISPATCH;
		_NE:  sp--; mem[sp-1] = mem[sp-1] != mem[sp] ? 1:0; DISPATCH;
		_LE:  sp--; mem[sp-1] = mem[sp-1] <= mem[sp] ? 1:0; DISPATCH;
		_GE:  sp--; mem[sp-1] = mem[sp-1] >= mem[sp] ? 1:0; DISPATCH;
		_HLT: goto _STOP;
		_RET: ip=mem[fp+1]; rp=fp+2; fp=mem[fp+2]; DISPATCH;
		// EXT
		_DOT: printf("%d\n",mem[sp-1]); DISPATCH;
		_AST: printf("AST not implemented\n"); goto _STOP; // TODO
		_ARG:
			t = mem[sp-1];
			mem[sp-1] = ((t>=0)&&(t<=7)) ? atoi(getenv(args[t])) : 0;
			//printf("DEBUG: ARG %d %s = %d\n", t, args[t], mem[sp-1]); // XXX
			DISPATCH;
	}
	_STOP:
	end = clock();
	double dt_ms = 1000*((double)(end-start)) / CLOCKS_PER_SEC;
	printf("STATUS: ip %d sp %d rp %d fp %d ic %lld dt %0.0f ms tos %d\n",ip,sp,rp,fp,ic,dt_ms,mem[sp-1]);
	free(mem);
	return ic;
}

// === END OF CORE ================================================================================================================


int* get_code(char* path) {
	FILE *fp = fopen(path,"r");
	// TODO: handle errors (fp==NULL)
	fseek(fp,0,SEEK_END);
	int code_size = ftell(fp) / sizeof(int);
	fseek(fp,0,SEEK_SET);
	int* code = calloc(code_size, sizeof(int));
	// TODO: handle errors (code==NULL)
	size_t nread = fread(code, sizeof(int), code_size, fp);
	// TODO: handle errors (nread<code_size)
	fclose(fp);
	return code;
}

extern char** environ;
int main(int argc, char** argv) {
	if (argc<2) { goto ERROR; }
	
	// ARG: code_path
	int* code = get_code(argv[1]);
	if (code==NULL) {
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

	// RUN
	run(code, n, mem_size);
	free(code);
	return 0;

ERROR:
	printf("USAGE: vm rom_path [mem_size=1024] [n_iters=0]\n");
	return 1;
}
