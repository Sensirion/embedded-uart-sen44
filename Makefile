common_sources = sensirion_config.h sensirion_common.h sensirion_common.c
uart_sources = sensirion_uart_hal.h sensirion_shdlc.h sensirion_shdlc.c
sen44_sources = sen44_uart.h sen44_uart.c

uart_implementation ?= sensirion_uart_hal.c

CFLAGS = -Os -Wall -fstrict-aliasing -Wstrict-aliasing=1 -Wsign-conversion -fPIC -I.

ifdef CI
    CFLAGS += -Werror
endif

.PHONY: all clean

all: sen44_uart_example_usage

sen44_uart_example_usage: clean
	$(CC) $(CFLAGS) -o $@ ${sen44_sources} ${uart_sources} \
		${uart_implementation} ${common_sources} sen44_uart_example_usage.c

clean:
	$(RM) sen44_uart_example_usage
