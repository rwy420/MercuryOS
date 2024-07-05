kernel_clean:
	cd kernel && make clean

kernel_iso:
	cd kernel && make mercury.iso
