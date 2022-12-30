import v1_vm as vm
from array import array
import re
import os

# get opcodes
opcode = {}
for name,v in vars(vm).items():
	if name.isupper() and type(v) is int:
		opcode[name] = v

def gen_c_header(path):
	"""generate c header file with opcode values"""
	with open(path,'w') as f:
		for name,v in opcode.items():
			print(f'#define {name} {v}', file=f)

def gen_pcode_file(text, path):
	"""generate pcode file"""
	pcode = asm(text)
	a = array('i', pcode)
	a.tofile(open(path,'wb'))

def compile_all():
	gen_c_header('../c/v1_opcodes.h')
	names = []
	for name in os.listdir('../../bench/v1/'):
		if not name.endswith('.asm'): continue
		names.append(name.replace('.asm',''))
	#for name in ['fibo','loops6','random','ackermann_jz','ackermann_jnz']: # TODO
	for name in names:
		print(f'compiling {name} - ',end='')
		try:
			gen_pcode_file(open(f'../../bench/v1/{name}.asm').read(), f'../../bench/v1/pcode/{name}.rom')
			print('OK')
		except Exception as e:
			print(f'ERROR: {e}')

def asm(text):
	"""compile text into p-code -> list[int]"""
	lines = text.split('\n')
	# first pass - collect labels
	label = {}
	tokens = []
	for line in lines:
		line_tokens = re.split('\s+',line)
		for token in line_tokens:
			if not token: continue
			# comments
			if token[0]==';': break
			# labels
			elif token[-1]==':':
				name = token[:-1]
				label[name] = len(tokens)
			else:
				tokens += [token]
	# second pass - all labels are known
	out = []
	for token in tokens:
		# label use
		if token[0]=='@':
			# relative
			name = token[1:]
			pos = label[name]
			offset = pos - len(out) + 1
			out += [offset]
		elif token in label:
			# absolute
			pos = label[token]
			out += [pos]
		# opcodes
		elif token in opcode:
			out += [opcode[token]]
		# numbers
		else:
			try:
				out += [int(token)]
				continue
			except:
				raise Exception(f'Unknown token "{token}"')
	return out

if __name__=="__main__":
	compile_all()
