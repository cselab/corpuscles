include(`co.m4')dnl
h_cmd(`echo 42')dnl
h_cmd(`echo a b c d')dnl
h_cmd(`echo 42')dnl
h_cmd(echo 42)dnl
h_cmd(echo)dnl
h_define(`a', h_cmd(`echo preved'))dnl
a
h_dumpdef
