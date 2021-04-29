#include "im920s.h"
#include <assert.h>

#define lengthof(x) (sizeof(x) / sizeof(x[0]))

static const char newline[] = "\r\n";

const char *im920s_command_to_str(im920s_command_t command) {
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

void im920s_init(im920s_t *im, putc_t p, getc_t g) {
	assert(im && p && g);
	im->getc = g;
	im->putc = p;

	//スタートアップ・メッセージを読み飛ばす

	char buffer[64];
	im920s_inner_gets(im, buffer, sizeof(buffer));
}

void im920s_inner_puts(const im920s_t *im, const char *str) {
	assert(im && str);
	for (; *str != '\0'; str++) {
		im920s_inner_putc(im, *str);
	}
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