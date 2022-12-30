import statistics as stat
import re
import os
import sys
from time import time as now

from tqdm import tqdm

def test_c(rom_name, args=[], mem_size=1024, rom_dir='../../bench/v1/pcode', vm=None):
	add_args_to_env(args)
	os.chdir('../c/')
	output = os.popen(f'{vm} {rom_dir}/{rom_name}.rom {mem_size}').read()
	return postproc(output)

def test_py(asm_name, args=[], mem_size=1024, asm_dir='../../bench/v1', vm=None):
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

def bench(vm, rom_name, args=[], mem_size=1024, repeat=30):
	dt_list = []
	test_fun = test_py if '_vm_py' in vm else test_c 
	for _ in tqdm(range(repeat), desc=f"{vm} {rom_name} {args}", leave=True):
		dt = test_fun(rom_name, args, mem_size=mem_size, vm=vm)['dt']
		dt_list += [dt]
	mean = stat.mean(dt_list)
	stdev = stat.stdev(dt_list)
	_min = min(dt_list)
	print(vm, rom_name, args, f"{_min:0.1f}", f"{mean:0.1f}", f"{stdev:0.1f}", repeat, dt_list, sep='\t', flush=True)

# =================================================================================================================================

if __name__=="__main__":
	f=open(f'../../bench/report_{int(now())}.tsv.xls','w')
	sys.stdout=f
	print('vm','rom','args','best_ms','mean_ms','stdev','runs','dt_list',sep='\t')
	# bench('py',  test_py, 'ackermann_jnz',  [3,5],    repeat=3, mem_size=100_000)
	# bench('tc',  test_c,  'ackermann_jnz',  [3,5],    repeat=3, mem_size=100_000)
	# bench('py',  test_py, 'ackermann_jz',   [3,5],    repeat=3, mem_size=100_000)
	# bench('tc',  test_c,  'ackermann_jz',   [3,5],    repeat=3, mem_size=100_000)
	# bench('py',  test_py, 'fibo',           [21],     repeat=10)
	# bench('tc',  test_c,  'fibo',           [21],     repeat=10)
	# bench('py',  test_py, 'random',         [30_000], repeat=10)
	# bench('py',  test_py, 'random_inc',     [30_000], repeat=10)
	# bench('tc',  test_c,  'random',         [30_000], repeat=10)
	# bench('tc',  test_c,  'random_inc',     [30_000], repeat=10)
	bench('v1_vm_case_tcc', 'loops6',         [14],      repeat=10)
	bench('v1_vm_case_gcc', 'loops6',         [14],      repeat=10)
	bench('v1_vm_case_tcc', 'loops6_inc',     [14],      repeat=10)
	bench('v1_vm_case_gcc', 'loops6_inc',     [14],      repeat=10)
	bench('v1_vm_case_tcc', 'loops6_inc_inc', [14],      repeat=10)
	bench('v1_vm_case_gcc', 'loops6_inc_inc', [14],      repeat=10)
	bench('v1_vm_repl_case_tcc', 'loops6',         [14],      repeat=10)
	bench('v1_vm_repl_case_gcc', 'loops6',         [14],      repeat=10)
	bench('v1_vm_repl_case_tcc', 'loops6_inc',     [14],      repeat=10)
	bench('v1_vm_repl_case_gcc', 'loops6_inc',     [14],      repeat=10)
	bench('v1_vm_repl_case_tcc', 'loops6_inc_inc', [14],      repeat=10)
	bench('v1_vm_repl_case_gcc', 'loops6_inc_inc', [14],      repeat=10)

	#bench('tc',  test_c,  'loops6_inc',     [14],      repeat=10)
	#bench('tc',  test_c,  'loops6_inc_inc', [14],      repeat=10)
	#bench('tc2', test_c2, 'loops6',         [14],      repeat=10)
	#bench('tc2', test_c2, 'loops6_inc',     [14],      repeat=10)
	#bench('tc2', test_c2, 'loops6_inc_inc', [14],      repeat=10)
	# bench('tc',  test_c,  'loops6',         [6],      repeat=10)
	# bench('tc',  test_c,  'loops6_inc',     [6],      repeat=10)
	# bench('tc',  test_c,  'loops6_inc_inc', [6],      repeat=10)
	bench('v1_vm_python',   'loops6',         [6],      repeat=10)
	bench('v1_vm_python',   'loops6_inc',     [6],      repeat=10)
	bench('v1_vm_python',   'loops6_inc_inc', [6],      repeat=10)
	f.close()
