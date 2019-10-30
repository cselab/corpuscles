include(`co.m4')dnl
include(`args.m4')dnl
include(`util.m4')dnl
h_changequote(,)dnl
. co.util

prog=co.geomview
prog0=co.geomview0

usg () {
    cat <<EOF
$prog [-t x y z] [-r x y z] [-a APPEARANCE] [-o FILE] [OFF]..
he geomview wrapper
-t x y z      translation
-r x y z      rotation in degree
-f zoom       field of view (default is 40)
-a APPEARANCE load appearance from a file
-o FILE       write FILE and exit
-O            write all PPM files and exit
-OO           write all oogl files and exit
-format	ppmscreen|ppm|ps|ppmosmesa|ppmosglx
-p command    process every off file by running 'command' < IN.off > OUT.off
-n none|each|all|keep normalization status (see geomview manual)
-c command    run command on every file and write output to stderr, %f is replaced by a file name
-e command    run 'command -t x y z -r x y z -f zoom -i [index of a file] -n  [number of files] < OFF'
              if 'E' is pressed,
-i command    run command on every image, %i replaced by input; %o -- by output; %b --- by basename

Keys:
    q: quit
    s: save snap.ppm
    S: save every snapshot
    e: run command on OFF file (see -e option)
    p: panel
    j/k: switch between off files
    J: dump file name
    [SPC]: dump orientation and field of view

Environment variables:
WX, WY: resolution of the snapshot (default: 800x600)
BACKGROUND: default ('1 1 1')

Examples:
$prog -t 0.25 0.25 0     data/rbc.off
$prog -a data/appearance data/rbc.off
$prog -o snap.ppm        data/rbc.off
$prog                    data/rbc.off data/sph.off
$prog -p co.orient       data/rbc.off data/sph.off
$prog -c off.volume      data/rbc.off data/sph.off
EOF
    exit
}

: ${GEOMVIEW=geomview}
: ${WX=800}
: ${WY=600}
: ${BACKGROUND="1 1 1"}

num0() { "$AWK" -v n="$1" 'BEGIN  {r = !(n + 0 == n); exit r }'; }
num() { if ! num0 "$1"; then err "not a number '$1'"; fi; }
nonzero () { if test "$1" = 0; then err 'cannot be zero'; fi; }
normalizationp () {
    case "$1" in
        none|each|all|keep) ;;
        *) err "wrong -n argument '$1'" ;;
    esac
}
formatp () {
    case "$1" in
        ppmscreen|ppm|ps|ppmosmesa|ppmosglx) ;;
        *) err "wrong -format argument '$1'" ;;
    esac
}

h_changequote()dnl
h_changequote(`, ')dnl
gview () {
    local status translate rotate background
    translate="$tx $ty $tz"
    rotate="$rx $ry $rz"
    trap '' SIGHUP
    "$GEOMVIEW" -wpos $WX,$WY -noinit -nopanels -b $BACKGROUND \
     -run "$prog0" \
       foreach(Args, `\h_qsingle`'"$A"\h_qsingle`'') \
       "$@"
    status=$?
    if test $status -ne 0
    then err "geomview0 failed"
    fi
    return $status
}
h_changequote(,)dnl

off0() {
    test -f "$1" &&
        "$AWK" '{ if ("" $0 ~ /OFF/) exit 0; else exit 1 }' "$1"
}
off()   { if ! off0 "$1"; then err "not an off file '$1'"; fi; }
filep() { if test ! -f "$1"; then err "not a file '$1'"; fi; }

if test $# -ne 0 && test "$1" = -h; then usg; fi
if ! e "$GEOMVIEW" --version '2>/dev/null' '1>/dev/null'; then err "$GEOMVIEW is not found"; fi

tx=0 ty=0 tz=0 rx=0 ry=0 rz=0 fov=40
off= output=- appearance=- process=- command=- icommand=- ecommand=- normalization=- format=ppmscreen
while test $# -ne 0
do case "$1" in
       -t) shift
           if test $# -le 2; then err '-t needs three numbers'; fi
           num "$1"; tx="$1"; shift
           num "$1"; ty="$1"; shift
           num "$1"; tz="$1"; shift
           ;;
       -r) shift
           if test $# -le 2; then err '-r needs three numbers'; fi
           num "$1"; rx="$1"; shift
           num "$1"; ry="$1"; shift
           num "$1"; rz="$1"; shift
           ;;
       -n) shift
           if test $# -eq 0; then err '-n needs an argument'; fi
           normalizationp "$1"; normalization="$1"; shift
           ;;
       -format) shift
           if test $# -eq 0; then err '-p needs an argument'; fi
           formatp "$1"; format="$1"; shift
           ;;
       -f) shift
           if test $# -eq 0; then err '-f needs a number'; fi
           num "$1"; nonzero "$1"; fov="$1"; shift
           ;;
       -a) shift
           if test $# -eq 0; then err '-o needs a file'; fi
           filep "$1"; appearance="$1"; shift
           ;;
       -o) shift
           if test $# -eq 0; then err '-o needs a file'; fi
           output="$1"; shift
           ;;
       -OO) shift; output=-OO ;;
       -O) shift; output=-O ;;
       -p) shift
           if test $# -eq 0; then err '-p needs a command'; fi
           process="$1"; shift
           ;;
       -c) shift
           if test $# -eq 0; then err '-c needs a command'; fi
           command="$1"; shift
           ;;
       -e) shift
           if test $# -eq 0; then err '-e needs a command'; fi
           ecommand="$1"; shift
           ;;
       -i) shift
           if test $# -eq 0; then err '-i needs a command'; fi
           icommand="$1"; shift
           ;;
       -*) err "unknown option '$1'" ;;
        *) break
   esac
done

if test $# -eq 0; then err 'no input files'; fi
if test "$process" = -
then for i; do off "$i"; done
fi

if test "$process" = -
then gview "$@"
else t=/tmp/co.geomview.off.$$
     mkdir -p $t
     trap 'rm -rf $t' 1 2 3 4 15
     i=0
     for f
     do shift
        i=`expr $i + 1`
        o=`printf %s/%05d $t $i`
        e $process "$f" '>' $o
        if test $? -ne 0; then err "'$process $f' failed"; fi
        set -- "$@" $o
     done
     gview "$@"
     status=$?
     rm -rf $t
     exit $status
fi
