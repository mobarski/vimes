import utils

if 1:
	rom = 'arr_get_set'
	utils.run_rom_cmd(rom, [], vm='v1_repl_switch_tcc',    debug=True, rom_dir='../test/v1/pcode')
	utils.run_rom_cmd(rom, [], vm='v1_repl_switch_gcc',    debug=True, rom_dir='../test/v1/pcode')

if 0:
	rom = 'cal_ret'
	utils.run_rom_cmd(rom, [1], vm='v1_indirect_tcc',    debug=True, rom_dir='../test/v1/pcode')
	utils.run_rom_cmd(rom, [1], vm='v1_indirect_gcc',    debug=True, rom_dir='../test/v1/pcode')
	exit()
	utils.run_rom_cmd(rom, [1], vm='v1_direct_tcc',      debug=True, rom_dir='../test/v1/pcode')
	utils.run_rom_cmd(rom, [1], vm='v1_direct_gcc',      debug=True, rom_dir='../test/v1/pcode')
	utils.run_rom_cmd(rom, [1], vm='v1_switch_gcc',      debug=True, rom_dir='../test/v1/pcode')
	utils.run_rom_cmd(rom, [1], vm='v1_repl_switch_gcc', debug=True, rom_dir='../test/v1/pcode')

if 0:
	utils.run_rom_cmd('loops6', [1], vm='v1_direct_gcc', debug=True, mem_size=64)
	utils.run_rom_cmd('loops6', [1], vm='v1_direct_tcc', debug=True, mem_size=64)
	utils.run_rom_cmd('loops6', [1], vm='v1_switch_gcc', debug=True, mem_size=64)
