#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "v1_opcodes.h"

int run(int* code, int n, int mem_size) {
	int ip = 0;  // instruction pointer
	int sp = 0;  // stack pointer
	int rp = mem_size-1; // return stack pointer (ret-stack grows down)
	int fp = rp; // frame pointer
	int ic = 0;  // instruction counter 
	int *mem = calloc(mem_size, sizeof(int));
	clock_t start = clock();
	clock_t end;
	
	for (;;ic++) {
		if ((n>0) && (ic >= n)) break;
		
		int op = code[ip];
		int a = code[ip+1];
		
		switch (op) {
			case LIT: mem[sp++]=a;           ip+=2; break;
			case LOD: mem[sp++]=mem[fp-a];   ip+=2; break;
			case STO: mem[fp-a]=mem[--sp];   ip+=2; break;
			case INT: rp-=a;                 ip+=2; break;
			case JMP: ip+=a;                        break;
			case JZ:  ip+=(mem[--sp]==0) ? a:2;     break;
			case JNZ: ip+=(mem[--sp]!=0) ? a:2;     break;
			case CAL: mem[rp]=fp; mem[rp-1]=ip+2; ip=a; rp-=2; fp=rp; break;
			case OPR:
				ip+=2;
				switch (a) {
					// arithmetic
					case ADD: sp--; mem[sp-1] += mem[sp]; break;
					case SUB: sp--; mem[sp-1] -= mem[sp]; break;
					case MUL: sp--; mem[sp-1] *= mem[sp]; break;
					case DIV: sp--; mem[sp-1] /= mem[sp]; break;
					case MOD: sp--; mem[sp-1] %= mem[sp]; break;
					// comparison
					case LT:  sp--; mem[sp-1] = mem[sp-1]  < mem[sp] ? 1:0; break;
					case GT:  sp--; mem[sp-1] = mem[sp-1]  > mem[sp] ? 1:0; break;
					case EQ:  sp--; mem[sp-1] = mem[sp-1] == mem[sp] ? 1:0; break;
					case NE:  sp--; mem[sp-1] = mem[sp-1] != mem[sp] ? 1:0; break;
					case LE:  sp--; mem[sp-1] = mem[sp-1] <= mem[sp] ? 1:0; break;
					case GE:  sp--; mem[sp-1] = mem[sp-1] >= mem[sp] ? 1:0; break;
					// other
					case HALT: ic++; goto STOP;
					case RET:  ip=mem[fp+1]; rp=fp+2; fp=mem[fp+2]; break;
				}
				break;
			case EXT:
				ip+=2;
				switch (a) {
					case DOT:  printf("%d\n",mem[sp-1]); break;
					case AST:  printf("AST not implemented\n"); goto STOP; // TODO
					case ARG:  printf("ARG not implemented\n"); goto STOP; // TODO
				}
				break;
		}
	}
	STOP:
	end = clock();
	double dt_ms = 1000*((double)(end-start)) / CLOCKS_PER_SEC;
	printf("STATUS: ip %d sp %d rp %d fp %d ic %d dt %0.0f ms tos %d\n",ip,sp,rp,fp,ic,dt_ms,mem[sp-1]);
	free(mem);
	return ic;
}

int* get_code(char* path) {
	FILE *fp = fopen(path,"r");
	// TODO: handle errors (fp==NULL)
	fseek(fp,0,SEEK_END);
	int code_size = ftell(fp) / sizeof(int);
	printf("code size: %d\n", code_size); // XXX
	fseek(fp,0,SEEK_SET);
	int* code = calloc(code_size, sizeof(int));
	// TODO: handle errors (code==NULL)
	size_t nread = fread(code, sizeof(int), code_size, fp);
	// TODO: handle errors (nread<code_size)
	fclose(fp);
	return code;
}

int main(int argc, char** argv, char** environ) {
	int* code = get_code("../../bench/v1/pcode/fibo.rom");
	//int code[] = {LIT,40, LIT,2, OPR,ADD, EXT,DOT, JMP,0, OPR,HALT};
	run(code, 100, 1000);
	free(code);
	return 0;
}
