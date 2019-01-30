#include "Omron2SMPB02E.h"
#include <M5Stack.h>


Omron2SMPB02E prs;
#define WIDTH 320//横サイズ
#define HEIGHT 240//縦サイズ
#define GRAPH_X 25 //グラフ描画領域X座標
#define GRAPH_Y 50 //グラフ描画領域Y座標
#define GRAPH_SPACE 2
#define GRAPH_W WIDTH - GRAPH_X - GRAPH_SPACE
#define GRAPH_H HEIGHT - GRAPH_Y - GRAPH_SPACE
//絶対圧の下限と上限を設定
#define GRAPH_A_MAX 103000
#define GRAPH_A_MIN 101000
// 温度の下限と上限を設定
#define GRAPH_B_MAX 40
#define GRAPH_B_MIN 0


/* 色の定義 */
#define RED     0xf800
#define REDd        0xf800
#define GREEN       0x07e0
#define BLUE        0x001f
#define BLACK       0x0000
#define WHITE       0xffff
#define GRAY        0x8c51
#define YELLOW      0xFFE0
#define CYAN        0x07FF
#define PURPLR      0xF81F

uint16_t GraphBuff_A[int(GRAPH_W)] = {0};
uint16_t GraphBuff_B[int(GRAPH_W)] = {0};
// グラフ開始ポイントを設定 
uint16_t graphStartPos[2] = {
  GRAPH_X + 1,
  GRAPH_Y - 1 + int(GRAPH_H)
};

/* 描画用バッファ領域をずらす*/
void slideBuff(uint16_t buff[], uint16_t size){
  for(int i = size - 1; i >= 0; i--) buff[i] = buff[i - 1];
}

/* テキスト描画 */
void drawText(uint32_t x, uint32_t y, String text, uint32_t color , uint8_t size){
  M5.Lcd.setTextColor(color);
  M5.Lcd.setTextSize(size);
  M5.Lcd.setCursor(x, y);
  M5.Lcd.print(text);
}

/*グラフ描画 */
void updateGraph(uint16_t color,uint16_t *GraphBuff){
  for(int i = 0; i < sizeof(GraphBuff_A); i++){
    M5.Lcd.drawPixel(graphStartPos[0] + i, graphStartPos[1] - *GraphBuff++, color);
  }
}



void setup()
{
  prs.begin();
  Serial.begin(9600);
   M5.begin();
  prs.set_mode(MODE_NORMAL);
  delay(300);
}

void loop()
{
  //気圧と温度を読み込む
  float tmp = prs.read_temp();
  float pressure = prs.read_pressure();
  Serial.println(pressure);

  M5.Lcd.fillRect(0, 0, 120, 20, BLACK);//文字領域クリア
 //気圧と温度を表示する
 drawText(0, 0, "pressure: " + String(pressure), WHITE, 1);
 drawText(0, 10, "TEMP: " + String(tmp),GREEN, 1);

 M5.Lcd.fillRect(GRAPH_X + 1, GRAPH_Y + 1, GRAPH_W - GRAPH_SPACE, GRAPH_H - GRAPH_SPACE, 0);//グラフ領域をいったんクリアする

 //気圧を描画する
  slideBuff(GraphBuff_A, sizeof(GraphBuff_A) / 2); 
  GraphBuff_A[0] = map(pressure, GRAPH_A_MIN, GRAPH_A_MAX, 0, GRAPH_H - 2);
  updateGraph(WHITE,GraphBuff_A);
 //温度を描画する
  slideBuff(GraphBuff_B, sizeof(GraphBuff_B) / 2); 
  GraphBuff_B[0] = map(tmp, GRAPH_B_MIN, GRAPH_B_MAX, 0, GRAPH_H - 2);
  updateGraph(GREEN,GraphBuff_B);

  delay(100);
}

