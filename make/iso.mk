$(B)/tacos64.iso: $(B)/iso/boot/tacos64 $(B)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(B)/iso

$(B)/iso/boot/tacos64: $(B)/tacos64 | $(B)/iso/boot
	mkdir -p $(dir $@)
	cp -f $< $@

$(B)/iso/boot/grub/grub.cfg: $(C)/grub.cfg | $(B)/iso/boot/grub
	mkdir -p $(dir $@)
	cp -f $< $@

$(B)/iso:
	mkdir -p $@

$(B)/iso/boot: | $(B)/iso
	mkdir -p $@

$(B)/iso/boot/grub: | $(B)/iso/boot
	mkdir -p $@
