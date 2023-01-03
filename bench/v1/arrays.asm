; PYTHON version:
;    n = int(sys.argv[1])
;    x = n * [0]       
;    y = n * [0]
;    for i in xrange(0,n):
;        x[i] = i + 1
;    for k in xrange(0,1000):
;        for i in xrange(n-1,-1,-1):
;            y[i] += x[i]
;    print y[0], y[-1]

INT 5
LIT 1 EXT ARG STO 0 ; n:0 = argv[1]
LOD 0 EXT ARR STO 1 ; x:1
LOD 0 EXT ARR STO 2 ; y:2

LIT 0 STO 3 ; i:3 = 0
loop1:
	LOD 3 LIT 1 OPR ADD ; i+1
	LOD 1 LOD 3 OPR ADD EXT SET ; x[i] = _
	INC 3 1 LOD 3 LOD 0 OPR LT JNZ @loop1 ; next i

LIT 0 STO 4 ; k:4 = 0
loop2:
	
	LOD 0 LIT 1 OPR SUB STO 3 ; i:3 = n-1
	loop3:
		LOD 1 LOD 3 OPR ADD EXT GET ; x[i]
		LOD 2 LOD 3 OPR ADD EXT GET ; y[i]
		OPR ADD
		LOD 2 LOD 3 OPR ADD EXT SET ; y[i] += _
		INC 3 -1 LOD 3 LIT -1 OPR GT JNZ @loop3 ; next i
	
	INC 4 1 LOD 4 LIT 1000 OPR LT JNZ @loop2 ; next k

OPR HLT
