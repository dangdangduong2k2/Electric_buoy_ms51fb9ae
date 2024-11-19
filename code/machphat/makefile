# makefile for keil c, by Tran-Luyen
# V1.2
# Date modified: 240926
# Author: Tran-Luyen
# Github: https://github.com/Tran-Luyen
# File Path: file:///D:\Works\Github_Projects\Code-Lib\NUVOTON\Example_makefile\makefile

CC := "$(firstword $(wildcard C:/**/**/UV4.exe))"
PRJ_NAME := $(basename $(firstword $(wildcard *.uvproj)))
PRJ_DIR := "$(CURDIR)/$(PRJ_NAME).uvproj"
OUTPUT_DIR := Objects

b: build log

cr: rebuild log

f: write_flash log

c: clean

log:
	@type .\$(OUTPUT_DIR)\uv4.log

build:
	@$(CC) -b $(PRJ_DIR) -j0 -t $(PRJ_NAME) -o "$(CURDIR)/$(OUTPUT_DIR)/uv4.log" && echo "Build success" || echo "Build error"

rebuild:
	@$(CC) -cr $(PRJ_DIR) -j0 -t $(PRJ_NAME) -o "$(CURDIR)/$(OUTPUT_DIR)/uv4.log" && echo "Build success" || echo "Build error"

write_flash:
	@$(CC) -f $(PRJ_DIR) -j0 -t $(PRJ_NAME) -o "$(CURDIR)/$(OUTPUT_DIR)/uv4.log" && echo "Write Flash success" || echo "Write Flash error"

clean:
	rm -r -f $(OUTPUT_DIR)