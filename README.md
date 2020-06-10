# test-unveil

The goal of this code was to understand how `unveil` behaves when forking. Does
the child process have the same restrictions?

The answer is yes, but before `execve`. Once you've `execve`, the restrictions
are not applied anymore.

```
$ ./test-fork 
[*] Unveil called
[*] Was able to open /etc/installurl
[*] Wasn't able to open /etc/hosts
[*] Forking
[*] Was able to open /etc/installurl
[*] Wasn't able to open /etc/hosts
[*] Try permissions with exec
127.0.0.1       localhost
::1             localhost

127.0.0.1    openbsd6 openbsd6.localdomain
[*] Child terminated normally
Exit status: 0
```
