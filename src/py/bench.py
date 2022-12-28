import statistics as stat
import re
import os

from tqdm import tqdm

def test_c(rom_name, args=[], mem_size=1024, rom_dir='../../bench/v1/pcode'):
	for i,a in enumerate(args):
		os.environ[f'P{i+1}']=str(a)
	os.chdir('../c/')
	output = os.popen(f'v1_vm_case.exe {rom_dir}/{rom_name}.rom {mem_size}').read()
	match = re.findall('STATUS: ip (\d+) sp (\d+) rp (\d+) fp (\d+) ic (\d+) dt (\d+) ms tos (\d+)', output)
	status = {k:int(x) for k,x in zip(['ip','sp','rp','fp','ic','dt','tos'],match[0])}
	print(output) # xxx
	#print(status) # xxx
	return status

def bench_c(rom_name, args=[], mem_size=1024, repeat=30):
	dt_list = []
	for _ in tqdm(range(repeat), desc=f"{rom_name} {args}", leave=False):
		dt = test_c(rom_name, args, mem_size=mem_size)['dt']
		dt_list += [dt]
	mean = stat.mean(dt_list)
	stdev = stat.stdev(dt_list)
	print(rom_name, args, f"{mean:0.1f}", f"{stdev:0.1f}", repeat, dt_list, sep='\t')


if __name__=="__main__":
	print('rom','args','mean_ms','stdev','runs','dt_list',sep='\t')
	#bench_c('ackerman_jnz', [3,3], repeat=10) # ERROR
	bench_c('fibo', [25], repeat=10)
	bench_c('loops6', [8], repeat=10)
	bench_c('random', [250_000], repeat=10)
