loops:
	INT 10
	LIT 1 EXT ARG STO 0 ### n:0 = ARG1
	LIT 0 STO 9 ### cnt:9 = 0
init-loop1:
	LOD 0 STO 1 ### a:1 = n
loop1:
	LOD 1 JZ @end-loop1
	#
	LOD 9 LIT 1 OPR ADD STO 9 ### cnt+=1
	#
	LOD 1 LIT 1 OPR SUB STO 1 ### a-=1
	JMP @loop1
end-loop1:

LOD 9 EXT DOT # print(cnt)
OPR HALT
