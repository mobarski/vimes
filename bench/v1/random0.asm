# http://dada.perl.it/shootout/random.html

# TODO: loop

main:
	LIT 42 # SEED
	CAL gen-random EXT DOT
	CAL gen-random EXT DOT
	CAL gen-random EXT DOT
	OPR HLT

# (last -- new)
gen-random:
	LIT 3877
	OPR MUL
	LIT 29573
	OPR ADD
	LIT 139968
	OPR MOD
	OPR RET
