include(`co.m4')dnl
h_changecom(,)dnl
h_define(`Git', h_translit(h_cmd(`"${GIT=git}" rev-parse HEAD'), `
'))dnl
#define GIT_SYMBOL int __git_`'Git = 1
#define GIT_STRING "Git"
