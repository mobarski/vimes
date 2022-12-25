#include "stdio.h"
#include "v1_opcodes.h"

int run(int* code, int n, int mem_size) {
	int ip = 0;
	int sp = 0;
	int rp = mem_size-1;
	int fp = rp;
	int ic = 0;
	int mem[mem_size];
	
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
					case AST:  printf("AST not implemented"); goto STOP; // TODO
					case ARG:  printf("ARG not implemented"); goto STOP; // TODO
				}
		}
	}
	STOP:
	printf("STATUS: ip %d sp %d rp %d fp %d ic %d dt XXX ms tos %d\n",ip,sp,rp,fp,ic,mem[sp-1]); // TODO dt
	return ic;
}

int main() {
	int code[] = {LIT,40, LIT,2, OPR,ADD, EXT,DOT, OPR,HALT};
	run(code, 0, 100);
}
