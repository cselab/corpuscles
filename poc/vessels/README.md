f () {
    for i in /v/*.tif
    do echo >&2 "$i"
       eval "$@" $i
    done
}

s="/v/CD13-647_OV_X-2900_Y-5222_647_nm_405-488-561-640-Quadrupleblock_1x_Both_000000.tif"
identify -format '%p\n' "$s"

convert "$s"[100] /dev/stdout | feh -F /dev/stdin
