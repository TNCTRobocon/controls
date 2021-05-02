#pragma once
#ifndef __HEADER_GUARD_IM929S__
#define __HEADER_GUARD_IM929S__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/**
 * @brief
 * im920sに登録されているコマンドの一覧
 */
typedef enum im920s_command {
	IM920S_TXDA,
	IM920S_TXDT,
	IM920S_TXDU,
	IM920S_TXSB,
	IM920S_STNN,
	IM920S_RDNN,
	IM920S_STGN,
	IM920S_RDGN,
	IM920S_RDID,
	IM920S_STCH,
	IM920S_RDCH,
	IM920S_STPO,
	IM920S_RDPO,
	IM920S_STNM,
	IM920S_RDNM,
	IM920S_STTL,
	IM920S_RTTL,
	IM920S_STTH,
	IM920S_RDTH,
	IM920S_ENAK,
	IM920S_DSAK,
	IM920S_ECIO,
	IM920S_DCIO,
	IM920S_ESNF,
	IM920S_DSNF,
	IM920S_RDRS,
	IM920S_ENWR,
	IM920S_DSWR,
	IM920S_ERXI,
	IM920S_DRXI,
	IM920S_DSRX,
	IM920S_ENRX,
	IM920S_SSTM,
	IM920S_RSTM,
	IM920S_SWTM,
	IM920S_RWTM,
	IM920S_ENSS,
	IM920S_DSSS,
	IM920S_RDCK,
	IM920S_STCK,
	IM920S_SYCK,
	IM920S_RPRM,
	IM920S_RDVR,
	IM920S_PCLR,
	IM920S_SBRT,
	IM920S_SRST
} im920s_command_t;

typedef void (*putc_t)(char);
typedef char (*getc_t)();
#define IM920S_BUFFER_SIZE (120)
typedef struct im920s {
	putc_t putc;
	getc_t getc;
	char line[IM920S_BUFFER_SIZE]; //一行分の受信用バッファ
	size_t line_in, line_out;
} im920s_t;

bool im920s_init(im920s_t *, putc_t, getc_t);

/**
 * @brief 文字を出力する。
 * @param im 初期化されたim920s
 * @param c 出力する文字
 */
static inline void im920s_inner_putc(const im920s_t *im, char c) {
	im->putc(c);
}

char im920_inter_getc(im920s_t *im);
/**
 * @brief 文字列を出力する
 * @param im 初期化されたim920s
 * @param str 出力する文字列
 */
void im920s_inner_puts(const im920s_t *im, const char *str);
/**
 * @brief 改行する
 * @param im 初期化されたim920s
 */
void im920s_inner_put_command(const im920s_t *im, im920s_command_t command);
void im920s_inner_printf(const im920s_t *im, const char *fmt, ...);
void im920s_inner_put_newline(const im920s_t *im);

/**
 * @brief
 *　改行文字(\r)もしくはbufferの大きさまで文字列を入力する。
 * @param im 初期化されたim920s
 * @param buffer 入力された文字列を格納するバッファ
 * @param size bufferの大きさ
 * @return char*
 */
char *im920s_inner_gets(im920s_t *im, char *buffer, size_t size);
/**
 * @brief
 *　改行文字(\r)もしくはbufferの大きさまで文字列を入力する。(ブロック付き)
 * @param im 初期化されたim920s
 * @param buffer 入力された文字列を格納するバッファ
 * @param size bufferの大きさ
 * @return char*
 */
static inline char *im920s_inner_gets_blocked(im920s_t *im, char *buffer,
											  size_t size) {
	char *line = NULL;
	while (line == NULL) {
		line = im920s_inner_gets(im, buffer, size);
	}
	return line;
}

/**
 * @brief 入力バッファをクリアする
 *
 * @param im 初期化されたim920s
 * @return char*
 */
void im920s_inner_clean(im920s_t *im);
/**
 * @brief 文字列を受け取り、その文字列がOK,NGかを判別する。
 * @param im　初期化されたim920s
 * @return true　OK
 * @return false NG
 */
bool im920s_verify_response(im920s_t *im);
/**
 * @brief ソフトウェアリセットを行う
 *
 * @param im 初期化されたim920s
 * @return true 成功
 * @return false 失敗
 */
bool im920s_sw_reset(im920s_t *im);

bool im920s_write_node(im920s_t *im, int node);
bool im920s_read_node(im920s_t *im, int *node);

typedef struct im920s_receive_packet {
	uint16_t node; // who send this packet?
	uint8_t rssi;
	size_t size;
	uint8_t bytes[32];
} im920s_receive_packet_t;

im920s_receive_packet_t *im920s_receive_packet(im920s_t *im,
											   im920s_receive_packet_t *packet);

typedef struct im920s_transmit_packet {};


#endif