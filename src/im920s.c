#include "im920s.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#define lengthof(x) (sizeof(x) / sizeof(x[0]))
#define BUFFER_SIZE (64)

static const char newline[] = "\r\n";

const char *command_to_str(im920s_command_t command) {
	static const char *commands[] = {
		"TXDA", "TXDT", "TXDU", "TXSB", "STNN", "RDNN", "STGN", "RDGN",
		"RDID", "STCH", "RDCH", "STPO", "RDPO", "STNM", "RDNM", "STTL",
		"RTTL", "STTH", "RDTH", "ENAK", "DSAK", "ECIO", "DCIO", "ESNF",
		"DSNF", "RDRS", "ENWR", "DSWR", "ERXI", "DRXI", "DSRX", "ENRX",
		"SSTM", "RSTM", "SWTM", "RWTM", "ENSS", "DSSS", "RDCK", "STCK",
		"SYCK", "RPRM", "RDVR", "PCLR", "SBRT", "SRST",
	};
	assert(command < lengthof(commands));
	return commands[(size_t)command];
}

bool im920s_init(im920s_t *im, putc_t p, getc_t g) {
	assert(im && p && g);
	im->getc = g;
	im->putc = p;

	//スタートアップ・メッセージを読み飛ばす
	im920s_inner_clean(im);
	return im920s_sw_reset(im);
}

void im920s_inner_puts(const im920s_t *im, const char *str) {
	assert(im && str);
	for (; *str != '\0'; str++) {
		im920s_inner_putc(im, *str);
	}
}

void im920s_inner_put_command(const im920s_t *im, im920s_command_t command) {
	assert(im);
	im920s_inner_puts(im, command_to_str(command));
}

void im920s_inner_printf(const im920s_t *im, const char *fmt, ...) {
	char buffer[BUFFER_SIZE];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	im920s_inner_puts(im, buffer);
	va_end(args);
}

void im920s_inner_put_newline(const im920s_t *im) {
	im920s_inner_puts(im, newline);
}

char *im920s_inner_gets(const im920s_t *im, char *buffer, size_t size) {
	assert(im && buffer && size > 1);
	for (size_t i = 0; i < size - 1; i++) {
		//有効な文字を得る
		char c;
		do {
			c = im920s_inner_getc(im);
		} while (c != '\0');
		//改行ならば終了する。
		if (c == newline[0]) {
			im920s_inner_getc(im); // newline
			buffer[i] = '\0';
			return buffer;
		}
		buffer[i] = c;
	}
	buffer[size - 1] = '\0';
	return buffer;
}

void im920s_inner_clean(const im920s_t *im) {
	assert(im);
	char c;
	do {
		c = im920s_inner_getc(im);
	} while (c != '\0');
}

bool im920s_verify_response(const im920s_t *im) {
	assert(im);
	char buffer[BUFFER_SIZE];
	im920s_inner_gets(im, buffer, sizeof(buffer));
	return strcmp(buffer, "OK") == 0;
}

bool im920s_sw_reset(const im920s_t *im) {
	assert(im);
	char buffer[BUFFER_SIZE];
	// clean up
	im920s_inner_clean(im);
	// send command
	im920s_inner_puts(im, command_to_str(IM920S_SRST));
	im920s_inner_put_newline(im);
	// receive responce expect "IM920s Ver .01.00"
	im920s_inner_gets(im, buffer, sizeof(buffer));
	const char *product = strtok(buffer, " ");
	// versionはファームウェアによって異なるので無視する
	return !strcmp(product, "IM920s");
}

bool im920s_write_node(const im920s_t *im, int node) {
	assert(im);
	assert(1 <= node && node <= 0xFFEF);
	// clean up
	im920s_inner_clean(im);
	// send
	im920s_inner_put_command(im, IM920S_STNN);
	im920s_inner_printf(im, " %04x", node);
	im920s_inner_put_newline(im);
	// receive
	return im920s_verify_response(im);
}

bool im920s_read_node(const im920s_t *im, int *node) {
	assert(im);
	assert(node);
	char buffer[BUFFER_SIZE];
	// clean up
	im920s_inner_clean(im);
	// send
	im920s_inner_put_command(im, IM920S_RDNN);
	im920s_inner_put_newline(im);
	// receive
	im920s_inner_gets(im, buffer, sizeof(buffer));
	*node = atoi(buffer);
	return true;
}