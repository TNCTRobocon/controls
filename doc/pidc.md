# PID制御の実装例
レファレンス実装として、arudinoを仮定する。

基本的な流れは、

1. 初期化時にpidc_initでパラメーターを初期化する
2. タイマー割り込みでpidc_pidを呼び出す

という流れになる。

```c

#include "pidc.h"
const static int currentPin=0xA0;
const static int pwmPin=0x3;
static int duty=0;

pidc_t pidc;
void init(){
	//pin assignment
	pinMode(currentPin,INPUT);
	pinMode(pwmPin,OUTPUT);

	//initilize data
	pidc_init(&pidc,4.0,1.0,1.0,10);
}

int clamp(int x,int low,int high){
	if (x>high)return high;
	if (x<low)return low;
	return x;
}

void control(){
	int real= analogRead(currentPin);
	const int target=100;
	duty+=pidc_pid(&pidc,real-target);
	duty=clamp(duty,0,255);
	analogWrite(pwmPin,duty);
}

void loop(){
	//本来はタイマー割り込みで呼び出すべき
	control();
	delay(1);//wait 10 ms
}
```