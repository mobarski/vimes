#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "errno.h"

#include "v1_opcodes.h"

// === CORE =======================================================================================================================

int run(int* code, int n, int mem_size, int code_size) {
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
	
	void **prog = calloc(code_size, sizeof(void*));
	for (int i=0; i<code_size;) {
		int op = code[i];
		int a  = code[i+1];
		int b;
		switch (op) {
			case LIT: prog[i]=(void*)&&_LIT; prog[i+1]=(void*)(long long)a; i+=2; break;
			case LOD: prog[i]=(void*)&&_LOD; prog[i+1]=(void*)(long long)a; i+=2; break;
			case STO: prog[i]=(void*)&&_STO; prog[i+1]=(void*)(long long)a; i+=2; break;
			case INT: prog[i]=(void*)&&_INT; prog[i+1]=(void*)(long long)a; i+=2; break;
			case JMP: prog[i]=(void*)&&_JMP; prog[i+1]=(void*)(long long)a; i+=2; break;
			case JZ:  prog[i]=(void*)&&_JZ;  prog[i+1]=(void*)(long long)a; i+=2; break;
			case JNZ: prog[i]=(void*)&&_JNZ; prog[i+1]=(void*)(long long)a; i+=2; break;
			case INC: prog[i]=(void*)&&_INC; prog[i+1]=(void*)(long long)a;
			                    b=code[i+2]; prog[i+2]=(void*)(long long)b; i+=3; break;
			case CAL: prog[i]=(void*)&&_CAL; prog[i+1]=(void*)(long long)a; i+=2; break;
			case OPR:
				switch (a) {
					case ADD: prog[i]=(void*)&&_ADD; prog[i+1]=NULL; i+=2; break;
					case SUB: prog[i]=(void*)&&_SUB; prog[i+1]=NULL; i+=2; break;
					case MUL: prog[i]=(void*)&&_MUL; prog[i+1]=NULL; i+=2; break;
					case DIV: prog[i]=(void*)&&_DIV; prog[i+1]=NULL; i+=2; break;
					case MOD: prog[i]=(void*)&&_MOD; prog[i+1]=NULL; i+=2; break;
					case EQ:  prog[i]=(void*)&&_EQ;  prog[i+1]=NULL; i+=2; break;
					case LT:  prog[i]=(void*)&&_LT;  prog[i+1]=NULL; i+=2; break;
					case GT:  prog[i]=(void*)&&_GT;  prog[i+1]=NULL; i+=2; break;
					case GE:  prog[i]=(void*)&&_GE;  prog[i+1]=NULL; i+=2; break;
					case LE:  prog[i]=(void*)&&_LE;  prog[i+1]=NULL; i+=2; break;
					case NE:  prog[i]=(void*)&&_NE;  prog[i+1]=NULL; i+=2; break;
					case RET: prog[i]=(void*)&&_RET; prog[i+1]=NULL; i+=2; break;
					case HLT: prog[i]=(void*)&&_HLT; prog[i+1]=NULL; i+=2; break;
				}
			case EXT:
				switch (a) {
					case DOT: prog[i]=(void*)&&_DOT; prog[i+1]=NULL; i+=2; break;
					case AST: prog[i]=(void*)&&_AST; prog[i+1]=NULL; i+=2; break;
					case ARG: prog[i]=(void*)&&_ARG; prog[i+1]=NULL; i+=2; break;
				}
		}
	}
	
	for (;;) {
		if ((n>0) && (ic >= n)) break;

		int op = code[ip];
		int a = code[ip+1];

		switch (op) {
			case LIT: goto _LIT;
			case LOD: goto _LOD;
			case STO: goto _STO;
			case INT: goto _INT;
			case JMP: goto _JMP;
			case JZ:  goto _JZ;
			case JNZ: goto _JNZ;
			case INC: goto _INC;
			case CAL: goto _CAL;
			case OPR:
				ip+=2;
				switch (a) {
					case ADD: goto _ADD;
					case SUB: goto _SUB;
					case MUL: goto _MUL;
					case DIV: goto _DIV;
					case MOD: goto _MOD;
					case LT:  goto _LT;
					case GT:  goto _GT;
					case EQ:  goto _EQ;
					case LE:  goto _LE;
					case GE:  goto _GE;
					case NE:  goto _NE;
					case HLT: goto _HLT;
					case RET: goto _RET;
				}
			case EXT:
				ip+=2;
				switch (a) {
					case DOT: goto _DOT;
					case AST: goto _AST;
					case ARG: goto _ARG;
				}
		}

		_LIT: mem[sp++]=a;           ip+=2;                   goto _NEXT;
		_LOD: mem[sp++]=mem[fp-a];   ip+=2;                   goto _NEXT;
		_STO: mem[fp-a]=mem[--sp];   ip+=2;                   goto _NEXT;
		_INT: rp-=a;                 ip+=2;                   goto _NEXT;
		_JMP: ip+=a;                                          goto _NEXT;
		_JZ:  ip+=(mem[--sp]==0) ? a:2;                       goto _NEXT;
		_JNZ: ip+=(mem[--sp]!=0) ? a:2;                       goto _NEXT;
		_INC: mem[fp-a]+=code[ip+2]; ip+=3;                   goto _NEXT;
		_CAL: mem[rp]=fp; mem[rp-1]=ip+2; ip=a; rp-=2; fp=rp; goto _NEXT;
		// OPR
		_ADD: sp--; mem[sp-1] += mem[sp];                     goto _NEXT;
		_SUB: sp--; mem[sp-1] -= mem[sp];                     goto _NEXT;
		_MUL: sp--; mem[sp-1] *= mem[sp];                     goto _NEXT;
		_DIV: sp--; mem[sp-1] /= mem[sp];                     goto _NEXT;
		_MOD: sp--; mem[sp-1] %= mem[sp];                     goto _NEXT;
		_LT:  sp--; mem[sp-1] = mem[sp-1]  < mem[sp] ? 1:0;   goto _NEXT;
		_GT:  sp--; mem[sp-1] = mem[sp-1]  > mem[sp] ? 1:0;   goto _NEXT;
		_EQ:  sp--; mem[sp-1] = mem[sp-1] == mem[sp] ? 1:0;   goto _NEXT;
		_NE:  sp--; mem[sp-1] = mem[sp-1] != mem[sp] ? 1:0;   goto _NEXT;
		_LE:  sp--; mem[sp-1] = mem[sp-1] <= mem[sp] ? 1:0;   goto _NEXT;
		_GE:  sp--; mem[sp-1] = mem[sp-1] >= mem[sp] ? 1:0;   goto _NEXT;
		_HLT: ic++; goto _STOP;
		_RET: ip=mem[fp+1]; rp=fp+2; fp=mem[fp+2];            goto _NEXT;
		// EXT
		_ARG:
			t = mem[sp-1];
			mem[sp-1] = ((t>=0)&&(t<=7)) ? atoi(getenv(args[t])) : 0;
			//printf("DEBUG: ARG %d %s = %d\n", t, args[t], mem[sp-1]); // XXX
			                                                  goto _NEXT;
		_DOT: printf("%d\n",mem[sp-1]);                       goto _NEXT;
		_AST: printf("AST not implemented\n"); goto _STOP; // TODO
		_NEXT:
		ic++;
	}
	_STOP:
	end = clock();
	double dt_ms = 1000*((double)(end-start)) / CLOCKS_PER_SEC;
	printf("STATUS: ip %d sp %d rp %d fp %d ic %lld dt %0.0f ms tos %d\n",ip,sp,rp,fp,ic,dt_ms,mem[sp-1]);
	free(mem);
	return ic;
}

// === END OF CORE ================================================================================================================

typedef struct int_array {
	int* ptr;
	int  cnt;
} int_array;

int_array get_code(char* path) {
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

	// RUN
	run(code.ptr, n, mem_size, code.cnt);
	free(code.ptr);
	return 0;

ERROR:
	printf("USAGE: vm rom_path [mem_size=1024] [n_iters=0]\n");
	return 1;
}
