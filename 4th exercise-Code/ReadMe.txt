4.1
This program implements a scheduler for the execution of a number of processes. The processes print a simple iterative message, and the frequency of these iterations is random.

The scheduler starts a process, lets it run for TQ seconds and then stops it. It then continues with the next one.

All processes are in a circular FIFO linked queue. When a process terminates it is removed from the list. When all processes terminate, the scheduler terminates too.

Usage:
eg:

./scheduler prog prog prog prog
The scheduler schedules four "prog" executables

4.2/4.3
The scheduler also includes another process, the "shell". The source code is in the file shell.c and request.h has the declarations of the requests from the shell to the scheduler.
The only difference between queue.c and queue2.c is the field of priority which is necessary for 4.3
The available commands are:

"? or Enter": list of available commands
"k id": kill process
"e exec": spawn a new process
"p": print all processes and the current one
"q": exit
"h id": make process a high priority
"l id": make process a low priority

(The last two commands are asked in 4.3)

Usage:
eg:

./scheduler-shell prog prog prog
and during the execution I can use the available shell commands.