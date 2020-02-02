zipinfo  -1 a.pptx  | grep '\.png$' > .list
unzip -q -o a.pptx `cat .list`

mogrify -colorspace Gray `cat .list`
zip -r a.pptx `cat .list`
