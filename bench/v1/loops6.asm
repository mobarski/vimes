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
			init-loop4:
				LOD 0 STO 4 ### d:4 = n
			loop4:
				LOD 4 JZ @end-loop4
				#
				init-loop5:
					LOD 0 STO 5 ### e:5 = n
				loop5:
					LOD 5 JZ @end-loop5
					#
					init-loop6:
						LOD 0 STO 6 ### f:6 = n
					loop6:
						LOD 6 JZ @end-loop6
						#
						LOD 9 LIT 1 OPR ADD STO 9 ### cnt+=1
						#
						LOD 6 LIT 1 OPR SUB STO 6 ### f-=1
						JMP @loop6
					end-loop6:
					#
					LOD 5 LIT 1 OPR SUB STO 5 ### e-=1
					JMP @loop5
				end-loop5:
				#
				LOD 4 LIT 1 OPR SUB STO 4 ### d-=1
				JMP @loop4
			end-loop4:
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
