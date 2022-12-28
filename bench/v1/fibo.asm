; http://dada.perl.it/shootout/fibo.html

LIT 1 EXT ARG
CAL fib
;EXT DOT
OPR HALT

fib: ; (N -- x)
	INT 1 STO 0 ; capture n from stack
	LOD 0 LIT 2 OPR LT JZ @else ; n < 2
		LIT 1 OPR RET ; return 1
	else:
		LOD 0 LIT 2 OPR SUB CAL fib ; fib(n-2)
		LOD 0 LIT 1 OPR SUB CAL fib ; fib(n-1)
		OPR ADD OPR RET

