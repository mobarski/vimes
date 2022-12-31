import utils

utils.run_rom_cmd('loops6_inc_inc', [16], vm='v1_vm_case_gcc', debug=True)
utils.run_rom_cmd('loops6_inc_inc', [16], vm='v1_vm_direct_gcc', debug=True)
#utils.run_rom_cmd('loops6_inc_inc', [2], vm='v1_vm_direct_tcc', debug=True)
