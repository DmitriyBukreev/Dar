# Labwork №1
Subject: operating systems  
Goal: study Portable Operating System Interface family of standarts

## Task
Make a program that will collect all files and folders into single one and will be able to restore them after that.  
Check the coding style with `checkpatch.pl` script, which can be obtained at <https://www.kernel.org/> (already included into project). Compile the program with `-fsanitize=address` flag.

## Result
Virtual ARchive (var) is able to process following instructions:
* Packing files into archive `var pack file file1 [file2 [...]]`
* Appending files to archive `var append file file1 [file2 [...]]`
* Unpacking files from the archive `var unpack file [path]`
* Getting help `var help`
