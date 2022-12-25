loops:
	INT 10
	LIT 1 EXT ARG STO 0 ### n:0 = ARG1
	LIT 0 STO 9 ### cnt:9 = 0
init-loop1:
	LOD 0 STO 1 ### a:1 = n
loop1:
	LOD 1 JZ @end-loop1
	#
	init-loop2:
		LOD 0 STO 2 ### b:2 = n
	loop2:
		LOD 2 JZ @end-loop2
		#
			init-loop3:
				LOD 0 STO 3 ### c:3 = n
			loop3:
				LOD 3 JZ @end-loop3
				#
				LOD 9 LIT 1 OPR ADD STO 9 ### cnt+=1
				#
				LOD 3 LIT 1 OPR SUB STO 3 ### c-=1
				JMP @loop3
			end-loop3:
		#
		LOD 2 LIT 1 OPR SUB STO 2 ### b-=1
		JMP @loop2
	end-loop2:
	#
	LOD 1 LIT 1 OPR SUB STO 1 ### a-=1
	JMP @loop1
end-loop1:

LOD 9 EXT DOT # print(cnt)
OPR HALT
