include(`co.m4')dnl
h_define(`add', `h_eval((`$1') + (`$2'))')dnl
add(1, 2)
h_define(`add_one', `h_curry(`add', `1')')dnl
add_one()(4)
add_one()(45)
