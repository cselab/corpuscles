include(`he.m4')dnl
h_define(`f', `$1')dnl
h_define(`h_key', `h_map_sep(`h_echo', ` ', `$1')')dnl
h_key(`a, b, c')
h_key(a, b, c, (x, y))
