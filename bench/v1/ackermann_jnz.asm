
LIT 1 EXT ARG # m
LIT 2 EXT ARG # n
CAL ack
EXT DOT
OPR HALT

# (m n -- x)
ack:
	### m:0 n:1
		INT 2 STO 1 STO 0
	
	### if not M: return N+1
		LOD 0 JNZ @end-if
			LOD 1 LIT 1 OPR ADD OPR RET
		end-if:
	
	### if not N: return ack(M-1,1)
		LOD 1 JNZ @end-if2
			LOD 0 LIT 1 OPR SUB LIT 1 CAL ack OPR RET
		end-if2:
	
	### return ack(m-1, ack(m,n-1))
		LOD 0 LIT 1 OPR SUB # (m-1)
		LOD 0 LOD 1 LIT 1 OPR SUB # (m-1,m,n-1)
		CAL ack CAL ack
		OPR RET
