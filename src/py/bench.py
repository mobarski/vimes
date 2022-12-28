import statistics as stat
import re
import os

from tqdm import tqdm

def test_c(rom_name, args=[], mem_size=1024, rom_dir='../../bench/v1/pcode'):
	add_args_to_env(args)
	os.chdir('../c/')
	output = os.popen(f'v1_vm_case.exe {rom_dir}/{rom_name}.rom {mem_size}').read()
	return postproc(output)

def test_py(asm_name, args=[], mem_size=1024, asm_dir='../../bench/v1'):
	from v1_vm import VM
	from v1_asm import asm
	asm_path = f"{asm_dir}/{asm_name}.asm"
	code = asm(open(asm_path).read())
	vm = VM(code, argv=args, mem_size=mem_size)
	return vm.run()

def add_args_to_env(args):
	for i,a in enumerate(args):
		os.environ[f'P{i+1}']=str(a)

def postproc(output):
	# postprocessing
	match = re.findall('STATUS: ip (\d+) sp (\d+) rp (\d+) fp (\d+) ic (\d+) dt (\d+) ms tos (\d+)', output)
	status = {k:int(x) for k,x in zip(['ip','sp','rp','fp','ic','dt','tos'],match[0])}
	#print(output) # xxx
	#print(status) # xxx
	return status

def bench(label, test_fun, rom_name, args=[], mem_size=1024, repeat=30):
	dt_list = []
	for _ in tqdm(range(repeat), desc=f"{rom_name} {args}", leave=False):
		dt = test_fun(rom_name, args, mem_size=mem_size)['dt']
		dt_list += [dt]
	mean = stat.mean(dt_list)
	stdev = stat.stdev(dt_list)
	print(label, rom_name, args, f"{mean:0.1f}", f"{stdev:0.1f}", repeat, dt_list, sep='\t')

# =================================================================================================================================

if __name__=="__main__":
	print('rom','args','mean_ms','stdev','runs','dt_list',sep='\t')
	bench('py', test_py, 'ackermann_jnz', [3,5], mem_size=100_000, repeat=3)
	bench('tcc',test_c,  'ackermann_jnz', [3,5], mem_size=100_000, repeat=3)
	bench('py', test_py, 'ackermann_jz', [3,5], mem_size=100_000, repeat=3)
	bench('tcc',test_c,  'ackermann_jz', [3,5], mem_size=100_000, repeat=3)
	bench('py', test_py, 'fibo', [21], repeat=10)
	bench('tcc',test_c,  'fibo', [21], repeat=10)
	bench('py', test_py, 'random', [30_000], repeat=10)
	bench('tcc',test_c,  'random', [30_000], repeat=10)
	bench('py', test_py, 'loops6', [6], repeat=10)
	bench('tcc',test_c,  'loops6', [6], repeat=10)
