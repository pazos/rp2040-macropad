all:
	$(SHELL) $(.SHELLFLAGS) "\
		rm -rf build;                               \
		mkdir -v build;                             \
		cd build;                                   \
		PICO_SDK_PATH=/usr/share/pico-sdk cmake ..; \
		make -j8"
