CC := gcc
build_dir := $(PWD)/build
project := spi_dev
sources := spi.c


objs := $(addprefix $(build_dir)/, $(addsuffix .o, $(basename $(notdir $(sources)))))
LDFLAGS := -lc --entry main
.PHONY: clean

all: $(build_dir)/$(project).elf
	@echo ===============$(notdir $<)========================
	@size $<

$(build_dir)/$(project).elf : $(sources) | $(build_dir)
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

run:
	@$(build_dir)/$(project).elf