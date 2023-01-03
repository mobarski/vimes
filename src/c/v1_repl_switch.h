
#define BEFORE
#define AFTER

#define DISPATCH \
	/*if ((n>0) && (ic >= n)) goto _STOP;*/ \
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
				case ARR: goto _ARR; \
				case GET: goto _GET; \
				case SET: goto _SET; \
			} \
	}

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
	
	{
		int op;
		int a;
		
		DISPATCH;
		
		_LIT: BEFORE; mem[sp++]=a;                                     ip+=2; AFTER; DISPATCH;
		_LOD: BEFORE; mem[sp++]=mem[fp-a];                             ip+=2; AFTER; DISPATCH;
		_STO: BEFORE; mem[fp-a]=mem[--sp];                             ip+=2; AFTER; DISPATCH;
		_INT: BEFORE; rp-=a;                                           ip+=2; AFTER; DISPATCH;
		_INC: BEFORE; mem[fp-a]+=code[ip+2];                           ip+=3; AFTER; DISPATCH;
		_JMP: BEFORE; ip+=a;                                                  AFTER; DISPATCH;
		_JZ:  BEFORE; ip+=(mem[--sp]==0) ? a:2;                               AFTER; DISPATCH;
		_JNZ: BEFORE; ip+=(mem[--sp]!=0) ? a:2;                               AFTER; DISPATCH;
		_CAL: BEFORE; mem[rp]=fp; mem[rp-1]=ip+2; ip=a; rp-=2; fp=rp;         AFTER; DISPATCH;
		// OPR                                                                                
		_ADD: BEFORE; sp--; mem[sp-1] += mem[sp];                             AFTER; DISPATCH;
		_SUB: BEFORE; sp--; mem[sp-1] -= mem[sp];                             AFTER; DISPATCH;
		_MUL: BEFORE; sp--; mem[sp-1] *= mem[sp];                             AFTER; DISPATCH;
		_DIV: BEFORE; sp--; mem[sp-1] /= mem[sp];                             AFTER; DISPATCH;
		_MOD: BEFORE; sp--; mem[sp-1] %= mem[sp];                             AFTER; DISPATCH;
		_LT:  BEFORE; sp--; mem[sp-1] = mem[sp-1]  < mem[sp] ? 1:0;           AFTER; DISPATCH;
		_GT:  BEFORE; sp--; mem[sp-1] = mem[sp-1]  > mem[sp] ? 1:0;           AFTER; DISPATCH;
		_EQ:  BEFORE; sp--; mem[sp-1] = mem[sp-1] == mem[sp] ? 1:0;           AFTER; DISPATCH;
		_NE:  BEFORE; sp--; mem[sp-1] = mem[sp-1] != mem[sp] ? 1:0;           AFTER; DISPATCH;
		_LE:  BEFORE; sp--; mem[sp-1] = mem[sp-1] <= mem[sp] ? 1:0;           AFTER; DISPATCH;
		_GE:  BEFORE; sp--; mem[sp-1] = mem[sp-1] >= mem[sp] ? 1:0;           AFTER; DISPATCH;
		_RET: BEFORE; ip=mem[fp+1]; rp=fp+2; fp=mem[fp+2];                    AFTER; DISPATCH;
		_HLT: BEFORE; ip-=2; goto _STOP;                                                      
		// EXT                                                                                
		_ARR: BEFORE; rp-=mem[sp-1]; mem[sp-1]=rp+1;                          AFTER; DISPATCH;
		_GET: BEFORE; mem[sp-1] = mem[mem[sp-1]];                             AFTER; DISPATCH;
		_SET: BEFORE; mem[mem[sp-1]] = mem[sp-2];                      sp-=2; AFTER; DISPATCH;
		_DOT: BEFORE; printf("%d\n",mem[sp-1]);                               AFTER; DISPATCH;
		_AST: BEFORE;                                                                         
			sp--;                                                                             
			if (mem[sp-1]==mem[sp]) {                                                         
				sp--;                                                         AFTER; DISPATCH;
			} else {                                                                          
				printf("ERROR: expected %d got %d instead\n",                                 
				       mem[sp],mem[sp-1]);                                                    
				goto _STOP;                                                                   
			}                                                                                 
		_ARG: BEFORE;                                                                         
			t = mem[sp-1];                                                                    
			mem[sp-1] = ((t>=0)&&(t<=7)) ? atoi(getenv(args[t])) : 0;                         
			//printf("DEBUG: ARG %d %s = %d\n", t, args[t], mem[sp-1]);                       
			                                                                  AFTER; DISPATCH;
	}
	_STOP:
	
	end = clock();
	dt_ms = 1000*((double)(end-start)) / CLOCKS_PER_SEC;
	printf("STATUS: ip %d sp %d rp %d fp %d ic %lld dt %0.0f ms tos %d\n", ip,sp,rp,fp,ic,dt_ms, sp>0?mem[sp-1]:0);
	
	free(mem);
	return ic;
}
