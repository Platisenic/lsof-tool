# lsof-tool
lsof is a tool to list open files. It can be used to list all the files opened by processes running in the system.
## How to use
1. Build the code
```bash
make
```
2. List open files.
```
./hw1
```
arguments
- -c [REGEX]: a regular expression (REGEX) filter for filtering command line
- -t [TYPE]: a TYPE filter. Valid TYPE includes `REG`, `CHR`, `DIR`, `FIFO`, `SOCK`, and `unknown`
- -f [REGEX]: a regular expression (REGEX) filter for filtering filenames
## Sample Usage
```
$ ./hw1 | head -n 20
COMMAND                       PID     USER      FD   TYPE      NODE      NAME
systemd                       1       root      cwd  unknown             /proc/1/cwd (Permission denied)
systemd                       1       root      rtd  unknown             /proc/1/root (Permission denied)
systemd                       1       root      txt  unknown             /proc/1/exe (Permission denied)
systemd                       1       root      NOFD                     /proc/1/fd (Permission denied)
kthreadd                      2       root      cwd  unknown             /proc/2/cwd (Permission denied)
kthreadd                      2       root      rtd  unknown             /proc/2/root (Permission denied)
kthreadd                      2       root      txt  unknown             /proc/2/exe (Permission denied)
kthreadd                      2       root      NOFD                     /proc/2/fd (Permission denied)
rcu_gp                        3       root      cwd  unknown             /proc/3/cwd (Permission denied)
rcu_gp                        3       root      rtd  unknown             /proc/3/root (Permission denied)
rcu_gp                        3       root      txt  unknown             /proc/3/exe (Permission denied)
rcu_gp                        3       root      NOFD                     /proc/3/fd (Permission denied)
rcu_par_gp                    4       root      cwd  unknown             /proc/4/cwd (Permission denied)
rcu_par_gp                    4       root      rtd  unknown             /proc/4/root (Permission denied)
rcu_par_gp                    4       root      txt  unknown             /proc/4/exe (Permission denied)
rcu_par_gp                    4       root      NOFD                     /proc/4/fd (Permission denied)
netns                         5       root      cwd  unknown             /proc/5/cwd (Permission denied)
netns                         5       root      rtd  unknown             /proc/5/root (Permission denied)
netns                         5       root      txt  unknown             /proc/5/exe (Permission denied)
```
```
$ sudo ./hw1 | head -n 20
COMMAND                       PID     USER      FD   TYPE      NODE      NAME
systemd                       1       root      cwd  DIR       2         /
systemd                       1       root      rtd  DIR       2         /
systemd                       1       root      txt  REG       12192065  /usr/lib/systemd/systemd
systemd                       1       root      mem  REG       12196300  /usr/lib/x86_64-linux-gnu/libgpg-error.so.0.32.1
systemd                       1       root      mem  REG       12195927  /usr/lib/x86_64-linux-gnu/libcap-ng.so.0.0.0
systemd                       1       root      mem  REG       12196765  /usr/lib/x86_64-linux-gnu/libpcre2-8.so.0.10.4
systemd                       1       root      mem  REG       12196554  /usr/lib/x86_64-linux-gnu/liblzma.so.5.2.5
systemd                       1       root      mem  REG       12197267  /usr/lib/x86_64-linux-gnu/libzstd.so.1.4.8
systemd                       1       root      mem  REG       12190680  /usr/lib/x86_64-linux-gnu/libcrypto.so.3
systemd                       1       root      mem  REG       12196552  /usr/lib/x86_64-linux-gnu/liblz4.so.1.9.3
systemd                       1       root      mem  REG       12196475  /usr/lib/x86_64-linux-gnu/libip4tc.so.2.0.0
systemd                       1       root      mem  REG       12196218  /usr/lib/x86_64-linux-gnu/libgcrypt.so.20.3.4
systemd                       1       root      mem  REG       12195986  /usr/lib/x86_64-linux-gnu/libcrypt.so.1.1.0
systemd                       1       root      mem  REG       12195929  /usr/lib/x86_64-linux-gnu/libcap.so.2.44
systemd                       1       root      mem  REG       12195881  /usr/lib/x86_64-linux-gnu/libblkid.so.1.1.0
systemd                       1       root      mem  REG       12195788  /usr/lib/x86_64-linux-gnu/libacl.so.1.1.2301
systemd                       1       root      mem  REG       12191841  /usr/lib/x86_64-linux-gnu/libc.so.6
systemd                       1       root      mem  REG       12190279  /usr/lib/x86_64-linux-gnu/libapparmor.so.1.8.2
systemd                       1       root      mem  REG       12196517  /usr/lib/x86_64-linux-gnu/libkmod.so.2.3.7
```
