from v1_vm import VM
from v1_asm import asm
from time import time as now
import traceback as tb

def test(name, asm_text, argv=[], n=1000, mem_size=None, trace=False):
	print(f'\n=== {name} ===')
	code = asm(asm_text)
	vm = VM(code, argv=argv, trace=trace, mem_size=mem_size)
	try:
		vm.run(n)
	except Exception as e:
		print('ERROR:', ''.join(tb.format_exception(e)))
		print('code',vm.code)
	print(flush=True)
	return vm

def load(path):
	return open(path).read()

##################################################################################


test('ackermann JNZ', load('../../bench/v1/ackermann_jnz.asm'), argv=[3,3], trace=False, mem_size=100_000, n=0)
test('ackermann JZ',  load('../../bench/v1/ackermann_jz.asm'),  argv=[3,3], trace=False, mem_size=100_000, n=0)

test('fibo', load('../../bench/v1/fibo.asm'), argv=[20], n=0, mem_size=100_000)

vm=test('randomN', load('../../bench/v1/random.asm'), argv=[2000], n=0)

vm=test('call',"""

CAL bbb
CAL aaa
CAL bbb
CAL aaa
OPR HALT

aaa: LIT 111 EXT DOT OPR RET
bbb: LIT 222 EXT DOT OPR RET
""")

test('assert', """
	LIT 10
	LIT 20 OPR AST
	LIT 42
	OPR HALT
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
OPR HALT
""")

test('lit mul add dot', """
	LIT 10 ;ok
xxx:
	LIT 4
	OPR MUL
	LIT 2
	OPR ADD
	
	EXT DOT
	OPR HALT
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
	
	OPR HALT
""")


test('random',"""
LIT 42 ; SEED
CAL gen-random EXT DOT
CAL gen-random EXT DOT
CAL gen-random EXT DOT
OPR HALT

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
