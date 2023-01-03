
tcc -DCORE=1  v1_vm.c     -o ../../bin/v1_switch_tcc.exe
tcc -DCORE=2  v1_vm.c     -o ../../bin/v1_repl_switch_tcc.exe
tcc -DCORE=3  v1_vm.c     -o ../../bin/v1_direct_tcc.exe
tcc -DCORE=4  v1_vm.c     -o ../../bin/v1_indirect_tcc.exe
tcc -DCORE=32 v1_vm.c     -o ../../bin/v1_direct2_tcc.exe

gcc -DCORE=1  v1_vm.c -O3 -o ../../bin/v1_switch_gcc.exe
gcc -DCORE=2  v1_vm.c -O3 -o ../../bin/v1_repl_switch_gcc.exe
gcc -DCORE=3  v1_vm.c -O3 -o ../../bin/v1_direct_gcc.exe
gcc -DCORE=4  v1_vm.c -O3 -o ../../bin/v1_indirect_gcc.exe
gcc -DCORE=32 v1_vm.c -O3 -o ../../bin/v1_direct2_gcc.exe
