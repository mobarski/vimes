
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
	
	
	//printf("rewriting code as pointers, code_size %d\n", code_size);
	// rewrite code as instruction pointers (prog)
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
				}; break;
			case EXT:
				switch (a) {
					case DOT: prog[i]=(void*)&&_DOT; prog[i+1]=NULL; i+=2; break;
					case AST: prog[i]=(void*)&&_AST; prog[i+1]=NULL; i+=2; break;
					case ARG: prog[i]=(void*)&&_ARG; prog[i+1]=NULL; i+=2; break;
				}; break; 
		}
	}
	//printf("PROG: ");  for (int ii=0; ii<code_size; ii++) { printf("%lld ", prog[ii]); }; printf("\n");
	
	#define NEXT ic++; goto *prog[ip]
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
