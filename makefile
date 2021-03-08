CC := gcc
build_dir := $(PWD)/build
project := spi_dev
sources := spi.c main.c


src_dir := $(PWD)/src
incl_dir := $(PWD)/include
srcs := $(addprefix $(src_dir)/, $(sources))
objs := $(addprefix $(build_dir)/, $(addsuffix .o, $(basename $(notdir $(sources)))))
LDFLAGS := -lc --entry main
CFLAGS := -I $(incl_dir) 
.PHONY: clean test

all: $(build_dir)/$(project).elf
	@echo ===============$(notdir $<)========================
	@size $<

test:
	@echo $(srcs)

$(build_dir)/$(project).elf : $(srcs) | $(build_dir)
	@echo $(LD) $(notdir $@)
	@$(CC) $(CFLAGS) -o $@ $^
# $(build_dir)/$(project).elf : $(objs) | $(build_dir)
#     @echo $(LD) $(notdir $@)
#     @$(LD) $(LDFLAGS) -o $@ $^
#
# $(build_dir)/%.o: %.c | $(build_dir)
#     @echo $(CC) $(notdir $@)
#     @$(CC) $(CFLAGS) -o $@ $<

$(build_dir):
	@mkdir -p $@

clean:
	rm -rf $(build_dir)

run: $(build_dir)/$(project).elf
	@$(build_dir)/$(project).elf

sync:
	@rsync gireesh@10.0.1.12:projects/spi_dev/ -r /home/gireesh/dev/spi_dev -e ssh --exclude=".git/"
