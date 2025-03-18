Currently, partially working. Still some leaks, and valgrind errors.

If you have questions, things that you dont understand or things that you can make better/less repetitiv, feel free to contact me.

FUNCTIONS :

getc : reads the given FILE *stream, character by character. Once the char is read makes ++ inside the file.

ungetc : if you want to get the previous read character.

accept : compares the given char, if its the good one it stream++

expect : if the written char is not the good one calls the unexpected function.
