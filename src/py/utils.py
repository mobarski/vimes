import re
import os
import sys

def run_rom_cmd(rom_name, args=[], mem_size=1024, rom_dir='../../bench/v1/pcode', vm=None, debug=False):
	add_args_to_env(args)
	os.chdir('../c/')
	cmd = f'{vm} {rom_dir}/{rom_name}.rom {mem_size}'
	if debug:
		print('\nCMD:\n', cmd, file=sys.stderr, sep='') # XXX
	output = os.popen(cmd).read()
	if debug:
		print("\nOUTPUT:\n", output, sep='', flush=True)
	return postproc(output)

def run_asm_py(asm_name, args=[], mem_size=1024, asm_dir='../../bench/v1', vm=None):
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
	try:
		status = {k:int(x) for k,x in zip(['ip','sp','rp','fp','ic','dt','tos'],match[0])}
		return status
	except:
		print(f'\nERROR: cannot parse output - "{output}"', file=sys.stderr)
		exit(1)
