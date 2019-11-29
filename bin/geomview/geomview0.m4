include(`co.m4')dnl
include(`util.m4')dnl
include(`args.m4')dnl
#!/bin/sh
. co.util

prog=co.geomview0

usg () {
    msg "$prog"
    msg 'he geomview module'
    exit
}

if test $# -ne 0 && test "$1" = -h; then usg; fi
h_foreach_sep(`A', `
', Args, `A=`$1'; shift')dnl

Tmp=/tmp/co.geomview.$$.ppm
trap 'rm -f $Tmp; echo 2' 1 2 3 15

"$AWK" -v Q=\' -v prog="$prog" -v LOG="$LOG" -v Tmp=$Tmp \
foreach(Args, `-v A="$A"') \
h_changequote()dnl
'
function ini(   i) {
    noff = ARGC - 1; ioff = 1
    for (i = 1; i in ARGV; i++)
        offs[i] = ARGV[i]
    ARGC = 1
    Save = 0 # snapshot every draw()?

    SaveType = eq(output, "-OO") ? "oogl" : "image"
    Interactive = eq(output, "-")

    Icommand = !eq(icommand, "-")
    Command = !eq(command, "-")

    Ecommand = !eq(ecommand, "-")
    EcommandArgs = sub(/^\|/, "", ecommand) # is |?

    Report  = Command

    Format = format
    Suffix = eq(format, "ps") ? "ps" : "ppm"

    X = 1; Y = 2; Z = 3; W = 4
    KEY_J = 106
    KEY_JJ = 74
    KEY_K = 107
    KEY_P = 112
    KEY_Q = 113
    KEY_S = 115
    KEY_SPACE = 32
    KEY_SS = 83
    KEY_E = 101
    OK = 0; EOF = 1; ERR = -1
    split(translate, Translate)
    split(rotate,    Rotate)
}

function parse(s,   a, n, key, pick, tri, ver, edg) {
    n = sub(/^\(/, "", s)
    if (n != 1) return ERR
    n = sub(/\)$/, "", s)
    if (n != 1) return ERR
    n = split(s, a)
    key = a[1]
    if (eq(key, "rawevent"))
       parse_key(a[2])
    else if (eq(key, "pick")) {
        parse_pick(s, pick)
        msg0("")
        msg0("tri " pick["FI"])
        if ((ver = pick["VI"]) != -1)
            msg0("ver " ver)
        if (!eq(edg = pick["EI"], "nil"))
            msg0("edg " edg)
    }
    return OK
}

function m_row(s, D, T,  a, i) {
    n = split(s, a)
    for (i = 1; i <= n; i++)
        T[D, i] = a[i]
}

function read_transform(T,   s) {
    g("write transform - worldgeom universe")
    if (getline s == ERR) err("fail to parse transform")
    if (s !~ /^transform {/)
        err(sprintf("expecting transform, got: `%s`", s))
    getline s; m_row(s, X, T)
    getline s; m_row(s, Y, T)
    getline s; m_row(s, Z, T)
    getline s; m_row(s, W, T)
    while (getline s != ERR && s !~ /[\t ]}/)  ;
}

function read_fov(  fov, n) {
    g("write camera - focus")
    if (getline s == ERR)
        err("fail to parse camera reply")
    if (s !~ /^camera {/)
        err(sprintf("expecting camera, got: `%s`", s))
    for (n = 1; n > 0; ) {
        if (getline == ERR)
            err("fail to parse camera info")
        if (/{/) n++
        if (/}/) n--
        if (n == 0) break
        if (/[ \t]*fov[ \t]/) fov = $2
    }
    if (emptyp(fov))
        err("have not found fov")
    return fov
}

function write_command(T, fov,   tx, ty, tz, rx, ry, rz, c, fmt) {
    tx = T[W, X]; ty = T[W, Y]; tz = T[W, Z]
    rx = atan2(-T[Y,Z], T[Z,Z])
    ry = atan2(T[X,Z], sqrt(T[Y,Z]^2 + T[Z,Z]^2))
    rz = atan2(-T[X,Y], T[X,X])
    rx = rad2ang(rx)
    ry = rad2ang(ry)
    rz = rad2ang(rz)
    fmt = "-t %g %g %g -r %g %g %g -f %g"
    c = sprintf(fmt, tx, ty, tz, rx, ry, rz, fov)
    msg0(c)
}

function run_ecommand(T, fov,   tx, ty, tz, rx, ry, rz, off, fmt, c) {
    off = singleq(offs[ioff])
    tx = T[W, X]; ty = T[W, Y]; tz = T[W, Z]
    rx = atan2(-T[Y,Z], T[Z,Z])
    ry = atan2(T[X,Z], sqrt(T[Y,Z]^2 + T[Z,Z]^2))
    rz = atan2(-T[X,Y], T[X,X])
    rx = rad2ang(rx)
    ry = rad2ang(ry)
    rz = rad2ang(rz)
    if (EcommandArgs) {
        fmt = "<%s %s"
        c = sprintf(fmt, off, ecommand)
    } else {
        fmt = "<%s %s -t %g %g %g -r %g %g %g -f %g -i %d -n %d"
        c = sprintf(fmt, off, ecommand, tx, ty, tz, rx, ry, rz, fov, ioff - 1, noff)
    }
    sys0(c)
}

function parse_key(k,   s,  T, fov, file) {
    if (k == KEY_Q)
        gexit(0)
    else if (k == KEY_P)
         g("ui-panel geomview on")
    else if (k == KEY_E) {
        if (Ecommand) {
             read_transform(T)
             fov = read_fov()
             run_ecommand(T, fov)
        }
    }
    else if (k == KEY_S) {
        file = sprintf("snap.%s", Suffix)
        msg0(file)
        snap(file)
    } else if (k == KEY_SPACE) {
        read_transform(T)
        fov = read_fov()
        write_command(T, fov)
    } else if (k == KEY_K) {
        if (--ioff < 1) ioff = noff
        draw()
    } else if (k == KEY_J) {
        if (++ioff > noff) ioff = 1
        draw()
    } else if (k == KEY_JJ) {
        Report = !Report
        draw()
    } else if (k == KEY_SS) {
        Save = !Save
        draw()
    }
    else err("unknown key: " k)
}

function key0(k) { g(sprintf("interest (rawevent %s)", k)) }
function key() {
    key0(KEY_E)
    key0(KEY_J)
    key0(KEY_JJ)
    key0(KEY_K)
    key0(KEY_P)
    key0(KEY_Q)
    key0(KEY_S)
    key0(KEY_SPACE)
    key0(KEY_SS)
}

function geom() {
    g("geometry obj appearance { : appearance } { : off }")
    if (!eq(normalization, "-"))
        g(sprintf("normalization obj %s", normalization))
    g("bbox-draw obj no")
    if (eq(appearance, "-"))
        g("read appearance {define appearance { +edge } material {ks 0}}")
    else
        g(sprintf("read appearance { define appearance < `%s`}",
                  appearance))
}

function draw(   off, file) {
    off = offs[ioff]
    g(sprintf("read geometry   { define off        < `%s` }", off))
    if (Save) {
        if (eq(SaveType, "image")) {
            file = sprintf("%05d.%s", ioff, Suffix)
            msg0(file); snap(file)
        } else if (eq(SaveType, "oogl")) {
            file = sprintf("%05d", ioff)
            msg0(file); oogl(file)
        } else
            err(sprintf("unknown SaveType \"%s\"", SaveType))
    }

    if (Report && Command)
        sys(command)
    else if (Report)
        msg0(off)
}

function singleq(s) { return Q s Q }
function quote(s) { sub(/`/, "\"", s); sub(/`/, "\"", s); return s }
function g(s) { g0("(" s ")") }
function g0(s) {
    if (!eq(LOG, "0")) msg("gcl: " quote(s))
    printf "%s\n", quote(s)
    fflush("/dev/stdout")
}

function m_ident(A,   i, j) {
    for (i = 1; i <= W; i++)
        for (j = 1; j <= W; j++)
            A[i,j] = (i == j) + 0
}
function m_str(A,   i, j, s) {
    for (i = 1; (i, 1) in A; i++) {
        if (i > 1) s = s "\n"
        for (j = 1; (i, j) in A; j++) {
            if (j > 0) s = s " "
            s = s sprintf("%.16g", A[i,j])
        }
    }
    return s
}

function m_mult(A, B,   i, j, k, C) { # A *= B
    for (i = 1; (i, 1) in A; i++)
        for (j = 1; (i, j) in A; j++)
            for (k = 1; (j, k) in B; k++)
                C[i,k] += A[i,j] * B[j,k]
    m_copy(C, A)
}

function m_copy(A, B,  k, cnt) {
    for (k in A) B[k] = A[k]
}
function m_translate(x, y, z, A) { A[W,X] += x; A[W,Y] += y; A[W,Z] += z }
function m_rotate(x, y, z, A) { m_rotx(x, A); m_roty(y, A); m_rotz(z, A) }
function m_rotx(p, A,  T) { m_rot0(p, T, Y, Z); m_mult(A, T) }
function m_roty(p, A,  T) { m_rot0(p, T, Z, X); m_mult(A, T) }
function m_rotz(p, A,  T) { m_rot0(p, T, X, Y); m_mult(A, T) }
function m_rot0(p, A, P, Q,   c, s) {
    c = cos(p); s = sin(p)
    m_ident(A)
    A[P, P] = c; A[P, Q] = -s
    A[Q, P] = s; A[Q, Q] =  c
}

function transform(   A) {
    m_ident(A)
    m_rotate(ang2rad(Rotate[X]), ang2rad(Rotate[Y]), ang2rad(Rotate[Z]), A)
    m_translate(Translate[X], Translate[Y], Translate[Z], A)
    g("xform-set `worldgeom` transform {" m_str(A) "}")
}
function set_fov() {
    g(sprintf("merge camera focus {fov %s}", fov))
}

function snap(file,   c, out) {
    off = "\"" offs[ioff] "\""
    out = "\"" file "\""
    input = Tmp
    if (!Icommand) {
        snap0(input)
        c = sprintf("mv %s %s", input, out)
        sys0(c)
    }
    else {
        snap0(input)
        c = icommand
        gsub(/%f/, off, c)
        gsub(/%i/, input, c)
        gsub(/%o/, out, c)
        gsub(/%b/, basename(file), c)
        sys0(c)
    }
}

function snap0(file,   cmd, reply, status) {
    cmd = sprintf("snapshot Camera `%s` %s", file, Format)
    g(cmd)
    g(cmd) # TODO
    g("echo done\\n") # synchronize
    status = getline reply
    if (!eq(reply, "done"))
        err("snap0 failed")
}

function oogl(file,   reply, status) {
    g(sprintf("write geometry `%s.geometry` self universe", file))
    g(sprintf("write camera `%s.camera` Camera universe", file))
    g(sprintf("write transform `%s.transform` self universe", file))
    g("echo done\\n") # synchronize
    status = getline reply
    if (!eq(reply, "done"))
        err("oogl failed")
}

BEGIN {
    ini()
    key()
    geom()
    transform()
    set_fov()
    draw()
    if (eq(output, "-")) {
        g("interest (pick world)")
        for (;;) {
            if (getline s == ERR) break
            if (parse(s) != OK) {
                msg("fail to parse: " quote(s))
                break
            }
        }
    } else if (eq(output, "-OO") || eq(output, "-O")) {
        Save = 1
        for (ioff = 1; ioff <= noff; ioff++)
            draw()
        gexit(0)
    } else {
        snap(output)
        gexit(0)
    }
}
function gexit(s) {
    g("exit")
    exit(s)
}
function msg(s)  { printf "%s: %s\n", prog, s | "cat >&2" }
function msg0(s) { printf "%s\n", s | "cat >&2" }

function err(s) { msg(s); gexit(2) }
function emptyp(s) { return length(s) == 0 }
function ang2rad(s) { return pi()*s/180 }
function rad2ang(s) { return 180*s/pi() }
function pi() { return 3.141592653589793 }
function eq(a ,b) { return "" a == "" b }
function sys(c,  file) {
    file = "\""  offs[ioff]  "\""
    if (c ~ /%/)
        gsub("%f", file, c)
    else
        c = c " " file
    sys0(c)
}

function sys0(c,   s) {
    c = sprintf("(%s) >&2", c)
    s = system(c)
    if (!eq(LOG, "0"))
        msg("system: " c)
    if (s != 0) {
        msg(sprintf("command: \"%s\" failed", c))
        gexit(2)
    }
}
function basename(s) {
    sub(/\.[^\.]*$/, "", s)
    return s
}

function parse_pick(s, q,   f, vi, ei, fi) {
    parse_lisp(s, f)
    vi = f[9]
    ei = f[10]
    fi = f[11]
    q["VI"] = vi + 0
    q["EI"] = unbrackets(trim(ei))
    q["FI"] = fi + 0
}

function parse_lisp(s, f,   n, nf, i, j, cnt, c) {
    delete f
    n = length(s)
    for (;;) {
        while (i < n && whitespace(c = ch(s, ++i))) ;
        if (!(i < n))
            break
        if (eq(c, "(")) {
            cnt = 1
            j = i
            do {
                c = ch(s, ++i)
                if (eq(c, "("))
                    cnt++
                else if (eq(c, ")"))
                    cnt--
            } while (i < n && cnt > 0)
            f[++nf] = substr(s, j, i - j + 1)
        } else {
            j = i
            do {
                c = ch(s, ++i)
            } while (i < n && !whitespace(c))
            f[++nf] = substr(s, j, i - j + 1)            
        }
    }
}

function ch(s, i) {
    return substr(s, i, 1)
}

function whitespace(s) {
    return s ~ /^[ \t]$/
}

function trim(s) {
    sub(/^[ \t]*/, "", s)
    sub(/[ \t]*$/, "", s)
    return s
}

function unbrackets(s) {
    sub(/^\(/, "" ,s)
    sub(/\)$/, "" ,s)
    return s
}
' "$@"

status=$?
rm -f $Tmp
exit $status
