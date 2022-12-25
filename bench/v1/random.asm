# http://dada.perl.it/shootout/random.html

LIT 1 EXT ARG # N
INT 1 STO 0

main:
	LIT 42 # SEED
		loop: LOD 0 JZ @end-loop
		CAL gen-random
		LOD 0 LIT 1 OPR SUB STO 0 JMP @loop end-loop:
	EXT DOT
	OPR HALT

# (last -- new)
gen-random:
	LIT 3877
	OPR MUL
	LIT 29573
	OPR ADD
	LIT 139968
	OPR MOD
	OPR RET
