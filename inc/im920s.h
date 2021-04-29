#pragma once
#ifndef __HEADER_GUARD_IM929S__
#define __HEADER_GUARD_IM929S__

#include <stddef.h>
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

const char *im920s_command_to_str(im920s_command_t);

typedef void (*putc_t)(char);
typedef char (*getc_t)();

typedef struct im920s {
	putc_t putc;
	getc_t getc;
} im920s_t;

void im920s_init(im920s_t *, putc_t, getc_t);
/**
 * @brief
 * 文字を出力する。
 * @param im 初期化されたim920s
 * @param c 出力する文字
 */
static inline void im920s_inner_putc(const im920s_t *im, char c) {
	im->putc(c);
}
/**
 * @brief 文字を入力する
 *
 * @param im 初期化されたim920s
 * @return char　入力された文字、存在しない場合は0
 */
static inline char im920s_inner_getc(const im920s_t *im) { return im->getc(); }
/**
 * @brief
 * 文字列を出力する
 * @param im 初期化されたim920s
 * @param str 出力する文字列
 */
void im920s_inner_puts(const im920s_t *im, const char *str);
/**
 * @brief
 *　改行文字(\r)もしくはbufferの大きさまで文字列を入力する。
 * @param im 初期化されたim920s
 * @param buffer 入力された文字列を格納するバッファ
 * @param size bufferの大きさ
 * @return char*
 */
char *im920s_inner_gets(const im920s_t *im, char *buffer, size_t size);

#endif