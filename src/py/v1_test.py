from v1_vm import VM
from v1_asm import asm
from time import time as now
import traceback as tb

def test(name, asm_text, argv=[], n=1000, mem_size=None, trace=False):
	print(f'\n=== {name} ===')
	code = asm(asm_text)
	vm = VM(code, argv=argv, trace=trace, mem_size=mem_size)
	try:
		vm.run(n, status=True)
	except Exception as e:
		print('ERROR:', ''.join(tb.format_exception(e)))
		print('code',vm.code)
	print(flush=True)
	return vm

def load(path):
	return open(path).read()

##################################################################################

test('arrays',"""
INT 5
LIT 3 STO 0 ; n:0 = 3
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
	
	INC 4 1 LOD 4 LIT 5 OPR LT JNZ @loop2 ; next k

OPR HLT
""",mem_size=20,trace=True,n=1000)
exit()

test('',"""
	INT 2
	LIT 4 EXT ARR STO 0
	LOD 0 STO 1
	LIT 11 LOD 1 EXT SET INC 1 1
	LIT 22 LOD 1 EXT SET INC 1 1
	LIT 33 LOD 1 EXT SET INC 1 1
	LIT 44 LOD 1 EXT SET INC 1 1
	OPR HLT
""",mem_size=10,trace=True)

test('',"""
	INT 1
	LIT 4 EXT ARR STO 0
	LIT 11 LOD 0 LIT 0 OPR ADD EXT SET
	LIT 22 LOD 0 LIT 1 OPR ADD EXT SET
	LIT 33 LOD 0 LIT 2 OPR ADD EXT SET
	LIT 44 LOD 0 LIT 3 OPR ADD EXT SET
	OPR HLT
""",mem_size=10,trace=True)

test('',"""
	INT 1
	LIT 3 ARR 0
	LIT 11 LIT 0 SET 0 ; a[0] = 11
	LIT 22 LIT 1 SET 0 ; a[1] = 22
	LIT 33 LIT 2 SET 0 ; a[2] = 33
	OPR HLT
	
	
	
	
	
	
	
	
	
	
""",mem_size=10,trace=True)

exit()

test('inc',"""
	INT 10
	LIT 10 STO 1
	LIT 20 STO 2
	INC 1 2
	INC 2 10
	INC 1 -10
	INC 2 10
	LOD 1
	LOD 2
	OPR ADD
	OPR HLT
""")

test('ackermann JNZ', load('../../bench/v1/ackermann_jnz.asm'), argv=[3,3], trace=False, mem_size=100_000, n=0)
test('ackermann JZ',  load('../../bench/v1/ackermann_jz.asm'),  argv=[3,3], trace=False, mem_size=100_000, n=0)

test('fibo', load('../../bench/v1/fibo.asm'), argv=[25], n=0, mem_size=100_000)

vm=test('randomN', load('../../bench/v1/random.asm'), argv=[2000], n=0)

vm=test('call',"""

CAL bbb
CAL aaa
CAL bbb
CAL aaa
OPR HLT

aaa: LIT 111 EXT DOT OPR RET
bbb: LIT 222 EXT DOT OPR RET
""")

test('assert', """
	LIT 10
	LIT 20 OPR AST
	LIT 42
	OPR HLT
""")

test('variables', """
INT 10
LIT 11 STO 1
LIT 22 STO 3
LIT 33 STO 5
LIT 44 STO 7
LIT 55 STO 9
LIT 66 STO 0
LIT 77 STO 2
LIT 88 STO 4
LIT 99 STO 6
OPR HLT
""")

test('lit mul add dot', """
	LIT 10 ;ok
xxx:
	LIT 4
	OPR MUL
	LIT 2
	OPR ADD
	
	EXT DOT
	OPR HLT
""")

test('lt', """
	LIT 10
	LIT 20
	OPR LT
	LIT 20
	LIT 10
	OPR LT
	LIT 10
	LIT 10
	OPR LT
	
	OPR HLT
""")


test('random',"""
LIT 42 ; SEED
CAL gen-random EXT DOT
CAL gen-random EXT DOT
CAL gen-random EXT DOT
OPR HLT

; (last -- new)
gen-random:
	LIT 3877
	OPR MUL
	LIT 29573
	OPR ADD
	LIT 139968
	OPR MOD
	OPR RET
""")

test('loops1', load('../../bench/v1/loops1.asm'), argv=[6], n=0)
test('loops2', load('../../bench/v1/loops2.asm'), argv=[6], n=0)
test('loops3', load('../../bench/v1/loops3.asm'), argv=[6], n=0)
test('loops4', load('../../bench/v1/loops4.asm'), argv=[6], n=0)
test('loops5', load('../../bench/v1/loops5.asm'), argv=[6], n=0)
test('loops6', load('../../bench/v1/loops6.asm'), argv=[6], n=0)

test('loops6_inc', load('../../bench/v1/loops6_inc.asm'), argv=[6], n=0)
test('loops6_inc_inc', load('../../bench/v1/loops6_inc_inc.asm'), argv=[6], n=0)
