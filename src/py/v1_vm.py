
# VM v4 - PL/0 inspired, with frames
# - S grows up
# - R grows down

# INSTRUCTIONS
LIT=0
OPR=1
LOD=2
STO=3
CAL=4
JMP=5
JZ=6
JNZ=7
INT=8 # Allocate x cells on R stack (name from p-machine -- RENAME ???)
EXT=9

# OPERATIONS
HALT=0
RET=1
ADD=2
SUB=3
MUL=4
DIV=5
MOD=6
#
LT=10
GT=11
EQ=12
NE=13
LE=14
GE=15
#

# EXTENSION
AST=0
DOT=1
ARG=2

# EMIT (single char)

from time import time as now

class VM:
	def __init__(self, code, argv=[], trace=False, mem_size=None):
		self.code = code
		self.argv = {i+1:argv[i] for i in range(len(argv))}
		self.mem = [0]*(mem_size or 32)
		self.running = True
		self.trace = trace
		# registers
		self.ip = 0 # instruction pointer
		self.sp = 0 # stack pointer
		self.rp = len(self.mem)-1 # return-stack pointer
		self.fp = self.rp # frame pointer
		self.ic = 0 # instruction counter
		#
		self.fun_by_op  = {}
		self.fun_by_opr = {}
		self.fun_by_ext = {}
		self.name_by_op = {}
		self.map_handlers()
	
	def map_handlers(self):
		for x in dir(self):
			if x.startswith('op_'):
				name = x[3:].upper()
				code = globals()[name]
				self.fun_by_op[code] = getattr(self,x)
				self.name_by_op[code] = name
			elif x.startswith('opr_'):
				name = x[4:].upper()
				self.fun_by_opr[globals()[name]] = getattr(self,x)
			elif x.startswith('ext_'):
				name = x[4:].upper()
				self.fun_by_ext[globals()[name]] = getattr(self,x)

	def run(self, n=0, status=False):
		t0 = now()
		while self.running:
			self.ic += 1
			op  = self.code[self.ip]
			arg = self.code[self.ip+1]
			if self.trace:
				print('ic', self.ic, 'ip', self.ip, 'sp', self.sp, 'rp', self.rp, self.name_by_op[op], arg, 'stack', self.mem[0:self.sp])
			self.fun_by_op[op](arg)
			if n and self.ic >= n:
				break
		# output VM status
		dt_ms = 1000*(now() - t0)
		tos = self.mem[self.sp-1]
		if status:
			print('STATUS: ip', self.ip, 'sp', self.sp, 'rp', self.rp, 'fp', self.fp, 'ic', self.ic, 'dt', int(dt_ms), 'ms', 'tos', tos)
		return dict(ip=self.ip, sp=self.sp, rp=self.rp, fp=self.fp, ic=self.ic, dt=int(dt_ms), tos=tos)

	# INSTRUCTIONS
	
	def op_lit(self, arg):
		self.mem[self.sp] = arg
		self.sp += 1
		self.ip += 2
	
	def op_int(self, arg):
		self.rp -= arg # ret-stack grows down
		self.ip += 2
	
	def op_lod(self, arg):
		self.mem[self.sp] = self.mem[self.fp - arg]
		self.sp += 1
		self.ip += 2
	
	def op_sto(self, arg):
		self.sp -= 1
		self.mem[self.fp - arg] = self.mem[self.sp]
		self.ip += 2
	
	def op_cal(self, arg):
		self.mem[self.rp] = self.fp
		self.mem[self.rp-1] = self.ip+2
		self.ip = arg
		self.rp -= 2 # ret-stack grows down
		self.fp = self.rp
	
	def op_jmp(self, arg):
		self.ip += arg

	def op_jz(self, arg):
		self.sp -= 1
		if self.mem[self.sp]==0:
			self.ip += arg
		else:
			self.ip += 2

	def op_jnz(self, arg):
		self.sp -= 1
		if self.mem[self.sp]!=0:
			self.ip += arg
		else:
			self.ip += 2

	def op_opr(self, arg):
		self.ip += 2
		self.fun_by_opr[arg]()
	
	def op_ext(self, arg):
		self.ip += 2
		self.fun_by_ext[arg]()

	# OPERATIONS
	
	def opr_ret(self):
		self.ip = self.mem[self.fp+1]
		self.rp = self.fp+2
		self.fp = self.mem[self.fp+2]
	
	def opr_halt(self):
		self.running = False
		self.ip -= 2
	
	def opr_add(self): self.sp -= 1 ; self.mem[self.sp-1]  += self.mem[self.sp]
	def opr_sub(self): self.sp -= 1 ; self.mem[self.sp-1]  -= self.mem[self.sp]
	def opr_mul(self): self.sp -= 1 ; self.mem[self.sp-1]  *= self.mem[self.sp]
	def opr_div(self): self.sp -= 1 ; self.mem[self.sp-1] //= self.mem[self.sp]
	def opr_mod(self): self.sp -= 1 ; self.mem[self.sp-1]  %= self.mem[self.sp]
	
	def opr_lt(self):  self.sp -= 1 ; self.mem[self.sp-1] = 1 if self.mem[self.sp-1]  < self.mem[self.sp] else 0
	def opr_gt(self):  self.sp -= 1 ; self.mem[self.sp-1] = 1 if self.mem[self.sp-1]  > self.mem[self.sp] else 0
	def opr_eq(self):  self.sp -= 1 ; self.mem[self.sp-1] = 1 if self.mem[self.sp-1] == self.mem[self.sp] else 0
	def opr_ne(self):  self.sp -= 1 ; self.mem[self.sp-1] = 1 if self.mem[self.sp-1] != self.mem[self.sp] else 0
	def opr_le(self):  self.sp -= 1 ; self.mem[self.sp-1] = 1 if self.mem[self.sp-1] <= self.mem[self.sp] else 0
	def opr_ge(self):  self.sp -= 1 ; self.mem[self.sp-1] = 1 if self.mem[self.sp-1] >= self.mem[self.sp] else 0

	# EXTENSION

	def ext_arg(self):
		'get program argument'
		arg = self.mem[self.sp-1]
		val = self.argv.get(arg,0)
		self.mem[self.sp-1] = val

	def ext_dot(self):
		'print top-of-stack'
		print(self.mem[self.sp-1])
	
	def ext_ast(self):
		'assert equal'
		self.sp -= 2
		if self.mem[self.sp] != self.mem[self.sp+1]:
			print(f'ERROR: expected {self.mem[self.sp+1]} (got {self.mem[self.sp]})')

if __name__=="__main__":
	vm = VM([])
