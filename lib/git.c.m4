include(`co.m4')dnl
h_define(Git, h_translit(h_esyscmd(`git rev-parse HEAD'), `
'))dnl
int __git_`'Git = 1;
