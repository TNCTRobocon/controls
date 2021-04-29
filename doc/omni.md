# オムニホイール制御例

３つのモーターを半径10cm、120度ごとに配置したロボットを用いて、正方形を描くような動きをさせる実装例。

```c
#include <omni.h>
#include <math.h>

static omni_wheels_t wheels[3];
static int pwmPins[3]={3,5,6}
static time=0;

void setup(){
	// pig assignment
	for (int i=0;i<3;i++){
		pinMode(pwmPins[i],OUTPUT);
	}
	// データ構造を初期化する
	const float angle=2*M_PI/3;//120度
	for (int i=0;i<3;i++){
		omni_wheel_init(&wheels[i],i*angle,100);
	}
}

void move(int x,int y){
	for (int i=0;i<3;i++){
		int value = omni_wheel_solve(&wheels[i],x,y,0);
		analogWrite(pwmPins[i],value);
	}
}

void loop(){
	move(0,100);//右
	delay(1000);
	move(100,0);//上
	delay(1000);
	move(0,-100);//左
	delay(1000);
	move(-100,0);//下
	delay(1000);
}

```