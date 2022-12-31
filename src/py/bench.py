import sys
import statistics as stat
from time import time as now

from tqdm import tqdm

import utils

def bench(vm, rom_name, args=[], mem_size=1024, repeat=30):
	dt_list = []
	ic_list = []
	run_fun = utils.run_asm_py if '_vm_py' in vm else utils.run_rom_cmd 
	for _ in tqdm(range(repeat), desc=f"{vm} {rom_name} {args}", leave=True):
		status = run_fun(rom_name, args, mem_size=mem_size, vm=vm)
		dt_list += [status['dt']]
		ic_list += [status['ic']]
	mean_dt = stat.mean(dt_list)
	stdev_dt = stat.stdev(dt_list)
	min_dt = min(dt_list)
	mips = (stat.mean(ic_list) / 1000 / mean_dt) if mean_dt else float('nan')
	print('',vm, rom_name, args, f"{min_dt:0.1f}", f"{mean_dt:0.1f}", f"{stdev_dt:0.1f}", f"{mips:0.1f}", repeat, dt_list, '', sep=' | ', flush=True)

# =================================================================================================================================

if __name__=="__main__":
	f=open(f'../../bench/raw/report_{int(now())}.md','w')
	sys.stdout=f
	print('','vm','rom','args','best_ms','mean_ms','stdev','mips','runs','dt_list','',sep=' | ')
	print('','--','---','----','-------','-------','-----','----','----','-------','',sep=' | ')
	
	R = 10
	
	# ACKERMANN
	if 0:
		bench('v1_vm_python',        'ackermann_jnz', [3,5], repeat=R, mem_size=100_000)
		bench('v1_vm_python',        'ackermann_jz',  [3,5], repeat=R, mem_size=100_000)
		#
		bench('v1_vm_case_tcc',      'ackermann_jnz', [3,8], repeat=R, mem_size=100_000)
		bench('v1_vm_case_gcc',      'ackermann_jnz', [3,8], repeat=R, mem_size=100_000)
		bench('v1_vm_repl_case_tcc', 'ackermann_jnz', [3,8], repeat=R, mem_size=100_000)
		bench('v1_vm_repl_case_gcc', 'ackermann_jnz', [3,8], repeat=R, mem_size=100_000)
		#
		bench('v1_vm_case_tcc',      'ackermann_jz',  [3,8], repeat=R, mem_size=100_000)
		bench('v1_vm_case_gcc',      'ackermann_jz',  [3,8], repeat=R, mem_size=100_000)
		bench('v1_vm_repl_case_tcc', 'ackermann_jz',  [3,8], repeat=R, mem_size=100_000)
		bench('v1_vm_repl_case_gcc', 'ackermann_jz',  [3,8], repeat=R, mem_size=100_000)
	
	# FIBO
	if 0:
		bench('v1_vm_case_tcc',      'fibo', [30], repeat=R)
		bench('v1_vm_case_gcc',      'fibo', [30], repeat=R)
		bench('v1_vm_repl_case_tcc', 'fibo', [30], repeat=R)
		bench('v1_vm_repl_case_gcc', 'fibo', [30], repeat=R)
		#
		bench('v1_vm_python',        'fibo', [24], repeat=R)
		bench('v1_vm_case_tcc',      'fibo', [24], repeat=R)
		bench('v1_vm_case_gcc',      'fibo', [24], repeat=R)
		bench('v1_vm_repl_case_tcc', 'fibo', [24], repeat=R)
		bench('v1_vm_repl_case_gcc', 'fibo', [24], repeat=R)
	
	# RANDOM
	if 0:
		bench('v1_vm_python',        'random',     [50_000],    repeat=R)
		bench('v1_vm_case_tcc',      'random',     [5_000_000], repeat=R)
		bench('v1_vm_case_gcc',      'random',     [5_000_000], repeat=R)
		bench('v1_vm_repl_case_tcc', 'random',     [5_000_000], repeat=R)
		bench('v1_vm_repl_case_gcc', 'random',     [5_000_000], repeat=R)
		#
		bench('v1_vm_python',        'random_inc', [50_000],    repeat=R)
		bench('v1_vm_case_tcc',      'random_inc', [5_000_000], repeat=R)
		bench('v1_vm_case_gcc',      'random_inc', [5_000_000], repeat=R)
		bench('v1_vm_repl_case_tcc', 'random_inc', [5_000_000], repeat=R)
		bench('v1_vm_repl_case_gcc', 'random_inc', [5_000_000], repeat=R)
	
	# LOOPS
	if 1:
		#
		bench('v1_vm_direct_gcc',    'loops6_inc_inc', [14],  repeat=R)
		bench('v1_vm_direct_tcc',    'loops6_inc_inc', [14],  repeat=R)
		bench('v1_vm_case_tcc',      'loops6_inc_inc', [14],  repeat=R)
		bench('v1_vm_case_gcc',      'loops6_inc_inc', [14],  repeat=R)
		bench('v1_vm_repl_case_tcc', 'loops6_inc_inc', [14],  repeat=R)
		bench('v1_vm_repl_case_gcc', 'loops6_inc_inc', [14],  repeat=R)
		#
		bench('v1_vm_case_tcc',      'loops6_inc',     [14],  repeat=R)
		bench('v1_vm_case_gcc',      'loops6_inc',     [14],  repeat=R)
		bench('v1_vm_repl_case_tcc', 'loops6_inc',     [14],  repeat=R)
		bench('v1_vm_repl_case_gcc', 'loops6_inc',     [14],  repeat=R)
		#
		bench('v1_vm_case_tcc',      'loops6',         [14],  repeat=R)
		bench('v1_vm_case_gcc',      'loops6',         [14],  repeat=R)
		bench('v1_vm_repl_case_tcc', 'loops6',         [14],  repeat=R)
		bench('v1_vm_repl_case_gcc', 'loops6',         [14],  repeat=R)
		#
		#bench('v1_vm_python',        'loops6',         [6],   repeat=R)
		bench('v1_vm_repl_case_gcc', 'loops6',         [6],   repeat=R)
		#bench('v1_vm_python',        'loops6_inc',     [6],   repeat=R)
		bench('v1_vm_repl_case_gcc', 'loops6_inc',     [6],   repeat=R)
		#bench('v1_vm_python',        'loops6_inc_inc', [6],   repeat=R)
		bench('v1_vm_repl_case_gcc', 'loops6_inc_inc', [6],   repeat=R)
		#
	f.close()
