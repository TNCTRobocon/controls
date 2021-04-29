#include "im920s.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define lengthof(x) (sizeof(x) / sizeof(x[0]))

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
	//管理領域の初期化
	im->getc = g;
	im->putc = p;
	//受信バッファ初期化
	memset(im->line, 0, sizeof(im->line));
	im->line_in = im->line_out = 0;
	//スタートアップ・メッセージを読み飛ばす
	im920s_inner_clean(im);
	return im920s_sw_reset(im);
}

char im920_inter_getc(im920s_t *im) {
	assert(im);
	//バッファに要素があるならバッファから取り出す
	if (im->line_in < im->line_out) {
		return im->line[im->line_out++];
	}
	return im->getc();
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
	char buffer[IM920S_BUFFER_SIZE];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	im920s_inner_puts(im, buffer);
	va_end(args);
}

void im920s_inner_put_newline(const im920s_t *im) {
	im920s_inner_puts(im, newline);
}

char *im920s_inner_gets(im920s_t *im, char *buffer, size_t size) {
	assert(im && buffer && size > 1);
	//改行文字が来るまでbufferに積む。
	for (;;) {
		char next = im->getc();
		if (next == '\0') {
			return NULL; //まだデータが揃っていない。
		} else if (next == newline[0]) {
			//一文字読み捨てる。
			do {
				next = im->getc();
			} while (next != '\0');
			im->line[im->line_in] = '\0';
			break;
		} else {
			im->line[im->line_in++] = '\0';
		}
	}
	//コピー
	strncpy(buffer, &im->line[im->line_out], size);
	//バッファを初期位置に戻す
	im->line_in = im->line_out = 0;
	return buffer;
}

void im920s_inner_clean(im920s_t *im) {
	assert(im);
	im->line_in = im->line_out = 0;
	char c;
	do {
		c = im->getc();
	} while (c != '\0');
}

bool im920s_verify_response(im920s_t *im) {
	assert(im);
	char buffer[IM920S_BUFFER_SIZE];
	im920s_inner_gets_blocked(im, buffer, sizeof(buffer));
	return strcmp(buffer, "OK") == 0;
}

bool im920s_sw_reset(im920s_t *im) {
	assert(im);
	char buffer[IM920S_BUFFER_SIZE];
	// clean up
	im920s_inner_clean(im);
	// send command
	im920s_inner_puts(im, command_to_str(IM920S_SRST));
	im920s_inner_put_newline(im);
	// receive responce expect "IM920s Ver .01.00"
	im920s_inner_gets_blocked(im, buffer, sizeof(buffer));
	const char *product = strtok(buffer, " ");
	// versionはファームウェアによって異なるので無視する
	return !strcmp(product, "IM920s");
}

bool im920s_write_node(im920s_t *im, int node) {
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

bool im920s_read_node(im920s_t *im, int *node) {
	assert(im);
	assert(node);
	char buffer[IM920S_BUFFER_SIZE];
	// clean up
	im920s_inner_clean(im);
	// send
	im920s_inner_put_command(im, IM920S_RDNN);
	im920s_inner_put_newline(im);
	// receive
	im920s_inner_gets_blocked(im, buffer, sizeof(buffer));
	*node = atoi(buffer);
	return true;
}

im920s_packet_t *im920s_receive(im920s_t *im, im920s_packet_t *packet) {
	assert(im && packet);
	char line[IM920S_BUFFER_SIZE];
	//データ到着確認
	if (im920s_inner_gets(im, line, sizeof(line)) == NULL) {
		return NULL;
	}
	//項目ごとに分解する。"aa,bbbb,dd:受信データ[,受信データ]*"
	char *head_str = strtok(line, ":");
	char *body_str = strtok(NULL, ":");
	if (!head_str || !body_str) {
		return NULL;
	}
	// head
	char *dummy_str = strtok(head_str, ",");
	char *node_str = strtok(NULL, ",");
	char *rssi_str = strtok(NULL, ",");
	if (!dummy_str || !node_str || !rssi_str) {
		return NULL;
	}
	// body
	size_t index;
	char *token;
	for (token = strtok(body_str, ','), index = 0; token != NULL;
		 token = strtok(NULL, ','), index++) {
		packet->bytes[index] = strtol(token, NULL, 16);
	}
	packet->size = index;
	return packet;
}