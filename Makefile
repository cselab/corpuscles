D = tool lib example/punto

include make/dir.mk
install: dir
test: dir
clean: dir

#test:; atest test/*

.PHONY: test install clean
