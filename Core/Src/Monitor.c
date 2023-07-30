//
// Created by mulai on 2023/7/29.
//

#include "Monitor.h"
//以下引用适用于STM32
#include "main.h"
#include "OLED.h"
#include "tim.h"


_Bool Key1_Detected;
_Bool Key2_Detected;
_Bool Key3_Detected;//三个按键的标志位

int CNT=1;
uint8_t Page_Choose=0;
uint8_t Static_Show=0;

void SETUP(void){
  OLED_Init();
}

int Limit_Chooses(int INPUT,int Min,uint8_t Max){//限制每一页的选项个数
  if (INPUT>Max){//超过最大值置为1
    INPUT=Min;
  }
  if (INPUT<Min){//为小于最小值时赋为最大
    INPUT=Max;
  }
  return INPUT;
}
void PageMain_Init(void){//主页的初始化
  OLED_ShowString(0,0,"     Monitor    ",16);//主显示
  OLED_ShowString(0,2,"1: Values View  ",16);
  OLED_ShowString(0,4,"2: Values Change",16);
  OLED_ShowString(0,6,"3: Flags View   ",16);
}

void PageMain(void){//如果没变化就不刷新，用于显示静态的数据以及文字
  static uint8_t CNT_Last;
  CNT=Limit_Chooses(CNT,1,3);//只有三个选项故对其添加一个限制
  if (CNT!=CNT_Last) {
    if (CNT == 1) {
      PageMain_Init();
      OLED_ShowString(0, 2, "1: Values View  ", 15);
    } else if (CNT == 2) {
      PageMain_Init();
      OLED_ShowString(0, 4, "2: Values Change", 15);
    } else if (CNT == 3) {
      PageMain_Init();
      OLED_ShowString(0, 6, "3: Flags View   ", 15);
    }
  }
  CNT_Last=CNT;
}
void Page1_Init(void){//副页面一
  OLED_ShowString(0,0,"      Values    ",16);//如果不够放可以直接将字体减小到6
  OLED_ShowString(0,2,"Data1:          ",16);
  OLED_ShowString(0,4,"Data2:          ",16);
  OLED_ShowString(0,6,"Data3:          ",16);
}
void Page1(void){//该函数用于显示一些动态的数据
  OLED_ShowFloatNum(48,2,0.0f,33,16);
  OLED_ShowSignedNum(48,4,0,3,16);
  OLED_ShowSignedNum(48,6,-1,3,16);
}

void Page2_Init(void){//副页面一
  OLED_ShowString(0,0,"  Values Form   ",16);//如果不够放可以直接将字体减小到6
  OLED_ShowString(0,2,"Data1:          ",16);
  OLED_ShowString(0,4,"Data2:          ",16);
  OLED_ShowString(0,6,"Data3:          ",16);
}

void Page2(void){//该函数用于显示一些动态的数据
  CNT=Limit_Chooses(CNT,-RANGE,RANGE);
  OLED_ShowFloatNum(48,2,0.0f,33,16);
  OLED_ShowSignedNum(48,4,-5,3,16);
  OLED_ShowSignedNum(48,6,CNT,3,16);
}


void Page3_Init(void){//副页面一
  OLED_ShowString(0,0,"      Flags     ",16);//如果不够放可以直接将字体减小到6
  OLED_ShowString(0,2,"Flag1:          ",16);
  OLED_ShowString(0,4,"Flag2:          ",16);
  OLED_ShowString(0,6,"Flag3:          ",16);
}
void Page3(void){//该函数用于显示一些动态的数据
  OLED_ShowFloatNum(48,2,0.0f,33,16);
  OLED_ShowSignedNum(48,4,0,3,16);
  OLED_ShowSignedNum(48,6,-1,3,16);
}

/*
 *置于While循环刷新屏幕
 * 如果需要添加新的页面仅需在判断中多加即可
 * 其中PageN_Init指第N页的静态显示部分该部分只会在进入当前页面时初始化一次，
 *                      而PageN则是第N页需要动态显示的部分，可以将一些变化的值放在里面显示
 * */
void LOOP(void){
  static uint8_t PageSelected;
  if (Key1_Detected){//按键判断
    CNT++;
    Key1_Detected=CLEAR;//清除标志位
  } else if (Key2_Detected){
    Page_Choose++;
    Page_Choose%=2;
    if (Page_Choose==0){CNT++;}//如果退回主界面，需要CNT++才可进入
    Key2_Detected=CLEAR;
  } else if (Key3_Detected){
    CNT--;
    Key3_Detected=CLEAR;
  }


  if (Page_Choose!=TRUE){//主副页判断，并进入对应的页面
    PageMain();
    PageSelected=CNT;//临时存储当前选中的Page
    Static_Show=TRUE;
  }
  else{//如果选择进入当前页面
    switch (PageSelected) {
      case 1:
        if (Static_Show==TRUE){
          Page1_Init();//避免显示的的过多导致屏幕刷新率降低
          Static_Show=CLEAR;
        }
        Page1();break;
      case 2:
        if (Static_Show==TRUE){
          Page2_Init();//避免显示的的过多导致屏幕刷新率降低
          CNT=CLEAR;//清除，便于将初值复位
          Static_Show=CLEAR;
        }
        Page2();break;
      case 3:
        if (Static_Show==TRUE){
          Page3_Init();//避免显示的的过多导致屏幕刷新率降低
          Static_Show=CLEAR;
        }
        Page3();break;
      default:
        break;
    }
//    if (PageSelected==1){
//      if (Static_Show==TRUE){
//        Page1_Init();//避免显示的的过多导致屏幕刷新率降低
//        Static_Show=CLEAR;
//      }Page1();
//    } else if (PageSelected==2){
//      if (Static_Show==TRUE){
//        Page2_Init();//避免显示的的过多导致屏幕刷新率降低
//        CNT=CLEAR;//清除，便于将初值复位
//        Static_Show=CLEAR;
//      }Page2();
//    } else if (PageSelected==3){
//      if (Static_Show==TRUE){
//        Page3_Init();//避免显示的的过多导致屏幕刷新率降低
//        Static_Show=CLEAR;
//      }Page3();
//    }
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){//定时器中断用于检测按键
  if (htim==&htim1){
    if (HAL_GPIO_ReadPin(Key1_GPIO_Port,Key1_Pin)==0){
      Key1_Detected=TRUE;
    } else if (HAL_GPIO_ReadPin(Key2_GPIO_Port,Key2_Pin)==0){
      Key2_Detected=TRUE;
    } else if (HAL_GPIO_ReadPin(Key3_GPIO_Port,Key3_Pin)==0){
      Key3_Detected=TRUE;
    }
  }
}

/*以上升沿检测按键*/
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//  if (GPIO_Pin==Key1_Pin){
//    Key1_Detected=TRUE;
//  } else if (GPIO_Pin==Key2_Pin){
//    Key2_Detected=TRUE;
//  } else if (GPIO_Pin==Key3_Pin){
//    Key3_Detected=TRUE;
//  }
//}