import v1_vm as vm
import re

# get opcodes
opcode = {}
for name,v in vars(vm).items():
	if name.isupper() and type(v) is int:
		opcode[name] = v

def gen_c_header(path):
	with open(path,'w') as f:
		for name,v in opcode.items():
			print(f'#define {name} {v}', file=f)

def asm(text):
	lines = text.split('\n')
	# first pass - collect labels
	label = {}
	tokens = []
	for line in lines:
		line_tokens = re.split('\s+',line)
		for token in line_tokens:
			if not token: continue
			# comments
			if token[0]=='#': break
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
	gen_c_header('../c/v1_opcodes.h')
