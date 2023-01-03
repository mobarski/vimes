
#define BEFORE
#define AFTER

long long run(int* code, int n, int mem_size, int code_size) {
	setbuf(stdout, NULL);
	
	int ip = 0;          // instruction pointer
	int sp = 0;          // stack pointer
	int rp = mem_size-1; // return stack pointer (ret-stack grows down)
	int fp = rp;         // frame pointer
	long long ic = 0;    // instruction counter (64-bit)
	int *mem = calloc(mem_size, sizeof(int));
	
	clock_t start = clock();
	clock_t end;
	double dt_ms;
	
	int t; // temporary value (ie for top-of-stack)
	
	for (;;ic++) {
		int op = code[ip];
		int a = code[ip+1];
		BEFORE;
		switch (op) {
			case LIT: mem[sp++]=a;                                     ip+=2; break;
			case LOD: mem[sp++]=mem[fp-a];                             ip+=2; break;
			case STO: mem[fp-a]=mem[--sp];                             ip+=2; break;
			case INT: rp-=a;                                           ip+=2; break;
			case INC: mem[fp-a]+=code[ip+2];                           ip+=3; break;
			case JMP: ip+=a;                                                  break;
			case JZ:  ip+=(mem[--sp]==0) ? a:2;                               break;
			case JNZ: ip+=(mem[--sp]!=0) ? a:2;                               break;
			case CAL: mem[rp]=fp; mem[rp-1]=ip+2; ip=a; rp-=2; fp=rp;         break;
			case OPR:
				ip+=2;
				switch (a) {
					case ADD: sp--; mem[sp-1] += mem[sp];                     break;
					case SUB: sp--; mem[sp-1] -= mem[sp];                     break;
					case MUL: sp--; mem[sp-1] *= mem[sp];                     break;
					case DIV: sp--; mem[sp-1] /= mem[sp];                     break;
					case MOD: sp--; mem[sp-1] %= mem[sp];                     break;
					case LT:  sp--; mem[sp-1] = mem[sp-1]  < mem[sp] ? 1:0;   break;
					case GT:  sp--; mem[sp-1] = mem[sp-1]  > mem[sp] ? 1:0;   break;
					case EQ:  sp--; mem[sp-1] = mem[sp-1] == mem[sp] ? 1:0;   break;
					case NE:  sp--; mem[sp-1] = mem[sp-1] != mem[sp] ? 1:0;   break;
					case LE:  sp--; mem[sp-1] = mem[sp-1] <= mem[sp] ? 1:0;   break;
					case GE:  sp--; mem[sp-1] = mem[sp-1] >= mem[sp] ? 1:0;   break;
					case RET: ip=mem[fp+1]; rp=fp+2; fp=mem[fp+2];            break;
					case HLT: ic++; ip-=2; goto _STOP;
				}
				break;
			case EXT:
				ip+=2;
				switch (a) {
					case ARR: rp-=mem[sp-1]; mem[sp-1]=rp+1;                  break;
					case GET: mem[sp-1] = mem[mem[sp-1]];                     break;
					case SET: mem[mem[sp-1]] = mem[sp-2];              sp-=2; break;
					case DOT: printf("%d\n",mem[sp-1]); break;
					case AST:
						sp--;
						if (mem[sp-1]==mem[sp]) {
							sp--; break;
						} else {
							printf("ERROR: expected %d got %d instead\n", mem[sp], mem[sp-1]);
							goto _STOP;
						}
					case ARG:
						t = mem[sp-1];
						mem[sp-1] = ((t>=0)&&(t<=7)) ? atoi(getenv(args[t])) : 0;
						//printf("DEBUG: ARG %d %s = %d\n", t, args[t], mem[sp-1]); // XXX
						break;
				}
				break;
		}
		AFTER;
	}
	_STOP:
	
	end = clock();
	dt_ms = 1000*((double)(end-start)) / CLOCKS_PER_SEC;
	printf("STATUS: ip %d sp %d rp %d fp %d ic %lld dt %0.0f ms tos %d\n",ip,sp,rp,fp,ic,dt_ms, sp>0?mem[sp-1]:0);
	
	free(mem);
	return ic;
}
