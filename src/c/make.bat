
tcc v1_vm_case.c          -o ../../bin/v1_vm_case_tcc.exe
tcc v1_vm_repl_case.c     -o ../../bin/v1_vm_repl_case_tcc.exe
tcc v1_vm_direct.c        -o ../../bin/v1_vm_direct_tcc.exe

gcc v1_vm_case.c      -O3 -o ../../bin/v1_vm_case_gcc.exe
gcc v1_vm_repl_case.c -O3 -o ../../bin/v1_vm_repl_case_gcc.exe
gcc v1_vm_direct.c    -O3 -o ../../bin/v1_vm_direct_gcc.exe
