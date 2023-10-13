cmd_mykernel/built-in.o :=  ld -m elf_i386   -r -o mykernel/built-in.o mykernel/mymain.o mykernel/myinterrupt.o 
