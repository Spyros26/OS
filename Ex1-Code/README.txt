1.1
Compile and link properly the files.

1.2
Take the content of two given files and put them in a third one using system calls

example input-output:
$ ./fconc A
Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)]
$ ./fconc A B
A: No such file or directory
$ echo 'Goodbye,' > A
$ echo 'and thanks for all the fish!' > B
$ ./fconc A B
$ cat fconc.out
Goodbye,
and thanks for all the fish!
$ ./fconc A B C
$ cat C
Goodbye,
and thanks for all the fish!

 1.3
 Use the same logic for more files...
