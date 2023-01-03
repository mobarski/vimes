
#define EOL   printf("\n")
#define TAB   printf("\t\t")

#define STACK printf("STACK: "); for (int ii=0;  ii<sp;    ii++) { printf("%d ", mem[ii]); }
#define CODE  printf("CODE: ");  for (int ii=ip; ii<ip+10; ii++) { printf("%d ", code[ii]); }
#define INFO  printf("ip %d\t\top %d\t\t%s\t\ta %d\t\tsp %d\t\ttos %d\t\trp %d\t\tfp %d\t\tic %d\t\t",ip,code[ip],op_name[code[ip]],code[ip+1],sp,sp>0?mem[sp-1]:-1,rp,fp,ic)

#define CHECK if (((n>0) && (ic >= n)) || (sp>rp)) goto _STOP

// define BEFORE CHECK; INFO; TAB; STACK; EOL
#define BEFORE
#define AFTER

long long run(int* code, int n, int mem_size, int code_size) {
	// VERBATIM from v1_direct.h
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
	
	// END-OF-VERBATIM
	
	void* P_INS[] = {&&_LIT,&&_OPR,&&_LOD,&&_STO,&&_CAL,&&_JMP,&&_JZ,&&_JNZ,&&_INT,&&_EXT,&&_INC};
	void* P_OPR[] = {&&_HLT,&&_RET,&&_ADD,&&_SUB,&&_MUL,&&_DIV,&&_MOD,0,0,0,&&_LT,&&_GT,&&_EQ,&&_NE,&&_LE,&&_GE};
	void* P_EXT[] = {&&_AST,&&_DOT,&&_ARG,&&_ARR,&&_GET,&&_SET};
	
	#define NEXT  ic++; goto *P_INS[code[ip]]
	#define a     code[ip+1]

	NEXT;

	_OPR: goto *P_OPR[code[ip+1]];
	_EXT: goto *P_EXT[code[ip+1]];
	
	// VERBATIM from v1_direct.h
	
	_LIT: BEFORE; mem[sp++]=a;                                                     ip+=2; AFTER; NEXT;
	_LOD: BEFORE; mem[sp++]=mem[fp-a];                                             ip+=2; AFTER; NEXT;
	_STO: BEFORE; mem[fp-a]=mem[--sp];                                             ip+=2; AFTER; NEXT;
	_INT: BEFORE; rp-=a;                                                           ip+=2; AFTER; NEXT;
	_INC: BEFORE; mem[fp-a]+=code[ip+2];                                           ip+=3; AFTER; NEXT;
	_JMP: BEFORE; ip+=a;                                                                  AFTER; NEXT;
	_JZ:  BEFORE; ip+=(mem[--sp]==0) ? a:2;                                               AFTER; NEXT;
	_JNZ: BEFORE; ip+=(mem[--sp]!=0) ? a:2;                                               AFTER; NEXT;
	_CAL: BEFORE; mem[rp]=fp; mem[rp-1]=ip+2;                                                         
	              ip=code[ip+1]; rp-=2; fp=rp;                                            AFTER; NEXT;
	// OPR
	_ADD: BEFORE; mem[sp-2] += mem[sp-1];                                   sp-=1; ip+=2; AFTER; NEXT;
	_SUB: BEFORE; mem[sp-2] -= mem[sp-1];                                   sp-=1; ip+=2; AFTER; NEXT;
	_MUL: BEFORE; mem[sp-2] *= mem[sp-1];                                   sp-=1; ip+=2; AFTER; NEXT;
	_DIV: BEFORE; mem[sp-2] /= mem[sp-1];                                   sp-=1; ip+=2; AFTER; NEXT;
	_MOD: BEFORE; mem[sp-2] %= mem[sp-1];                                   sp-=1; ip+=2; AFTER; NEXT;
	_LT:  BEFORE; mem[sp-2]  = mem[sp-2]  < mem[sp-1] ? 1:0;                sp-=1; ip+=2; AFTER; NEXT;
	_GT:  BEFORE; mem[sp-2]  = mem[sp-2]  > mem[sp-1] ? 1:0;                sp-=1; ip+=2; AFTER; NEXT;
	_EQ:  BEFORE; mem[sp-2]  = mem[sp-2] == mem[sp-1] ? 1:0;                sp-=1; ip+=2; AFTER; NEXT;
	_NE:  BEFORE; mem[sp-2]  = mem[sp-2] != mem[sp-1] ? 1:0;                sp-=1; ip+=2; AFTER; NEXT;
	_LE:  BEFORE; mem[sp-2]  = mem[sp-2] <= mem[sp-1] ? 1:0;                sp-=1; ip+=2; AFTER; NEXT;
	_GE:  BEFORE; mem[sp-2]  = mem[sp-2] >= mem[sp-1] ? 1:0;                sp-=1; ip+=2; AFTER; NEXT;
	_RET: BEFORE; ip=mem[fp+1]; rp=fp+2; fp=mem[fp+2];                                    AFTER; NEXT;
	_HLT: BEFORE; goto _STOP;
	
	// EXT
	_ARR: BEFORE; rp-=mem[sp-1]; mem[sp-1]=rp+1;                                   ip+=2; AFTER; NEXT;
	_GET: BEFORE; mem[sp-1] = mem[mem[sp-1]];                                      ip+=2; AFTER; NEXT;
	_SET: BEFORE; mem[mem[sp-1]] = mem[sp-2];                               sp-=2; ip+=2; AFTER; NEXT;
	_ARG: BEFORE; t = mem[sp-1];
	              mem[sp-1] = ((t>=0)&&(t<=7)) ? atoi(getenv(args[t])):0;          ip+=2; AFTER; NEXT;
	_DOT: BEFORE; printf("%d\n",mem[sp-1]);                                        ip+=2; AFTER; NEXT;
	_AST: BEFORE; if (mem[sp-1]==mem[sp-2]) {                               sp-=2; ip+=2; AFTER; NEXT;
	              } else {
	                  printf("ERROR: expected %d got %d instead\n",
	                                 mem[sp-1],  mem[sp-2]);
	                  goto _STOP;
	              }

	_STOP:
	// END-OF-VERBATIM

	end = clock();
	dt_ms = 1000*((double)(end-start)) / CLOCKS_PER_SEC;
	printf("STATUS: ip %d sp %d rp %d fp %d ic %lld dt %d ms tos %d\n",ip,sp,rp,fp,ic,(int)dt_ms, sp>0?mem[sp-1]:0);
	
	free(mem);
	return ic;
}
