driver_dir := ..

common_sources = ${driver_dir}/sensirion_config.h ${driver_dir}/sensirion_common.h ${driver_dir}/sensirion_common.c
uart_sources = ${driver_dir}/sensirion_uart_hal.h ${driver_dir}/sensirion_shdlc.h ${driver_dir}/sensirion_shdlc.c
sensirion_test_sources = sensirion_test_setup.cpp


uart_impl_src = ${driver_dir}/sample-implementations/linux_user_space/sensirion_uart_hal.c

sen44_sources = ${driver_dir}/sen44_uart.h ${driver_dir}/sen44_uart.c

CXXFLAGS ?= $(CFLAGS) -fsanitize=address -I${driver_dir}
ifdef CI
	CXXFLAGS += -Werror
endif
LDFLAGS ?= -lasan -lstdc++ -lCppUTest -lCppUTestExt

.PHONY: clean test

sen44-test-uart: sen44_uart_test.cpp ${sen44_sources} ${sensirion_test_sources} ${uart_sources} ${uart_impl_src} ${common_sources}
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

test: sen44-test-uart
	set -ex; ./sen44-test-uart; echo;

clean:
	$(RM) ${sen44_test_binaries}
