//
// Created by mulai on 2023/7/29.
//

#include "Monitor.h"
//��������������STM32
#include "main.h"
#include "OLED.h"
#include "tim.h"


_Bool Key1_Detected;
_Bool Key2_Detected;
_Bool Key3_Detected;//���������ı�־λ

int CNT=1;
uint8_t Page_Choose=0;
uint8_t Static_Show=0;

void SETUP(void){
  OLED_Init();
}

int Limit_Chooses(int INPUT,int Min,uint8_t Max){//����ÿһҳ��ѡ�����
  if (INPUT>Max){//�������ֵ��Ϊ1
    INPUT=Min;
  }
  if (INPUT<Min){//ΪС����Сֵʱ��Ϊ���
    INPUT=Max;
  }
  return INPUT;
}
void PageMain_Init(void){//��ҳ�ĳ�ʼ��
  OLED_ShowString(0,0,"     Monitor    ",16);//����ʾ
  OLED_ShowString(0,2,"1: Values View  ",16);
  OLED_ShowString(0,4,"2: Values Change",16);
  OLED_ShowString(0,6,"3: Flags View   ",16);
}

void PageMain(void){//���û�仯�Ͳ�ˢ�£�������ʾ��̬�������Լ�����
  static uint8_t CNT_Last;
  CNT=Limit_Chooses(CNT,1,3);//ֻ������ѡ��ʶ������һ������
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
void Page1_Init(void){//��ҳ��һ
  OLED_ShowString(0,0,"      Values    ",16);//��������ſ���ֱ�ӽ������С��6
  OLED_ShowString(0,2,"Data1:          ",16);
  OLED_ShowString(0,4,"Data2:          ",16);
  OLED_ShowString(0,6,"Data3:          ",16);
}
void Page1(void){//�ú���������ʾһЩ��̬������
  OLED_ShowFloatNum(48,2,0.0f,33,16);
  OLED_ShowSignedNum(48,4,0,3,16);
  OLED_ShowSignedNum(48,6,-1,3,16);
}

void Page2_Init(void){//��ҳ��һ
  OLED_ShowString(0,0,"  Values Form   ",16);//��������ſ���ֱ�ӽ������С��6
  OLED_ShowString(0,2,"Data1:          ",16);
  OLED_ShowString(0,4,"Data2:          ",16);
  OLED_ShowString(0,6,"Data3:          ",16);
}

void Page2(void){//�ú���������ʾһЩ��̬������
  CNT=Limit_Chooses(CNT,-RANGE,RANGE);
  OLED_ShowFloatNum(48,2,0.0f,33,16);
  OLED_ShowSignedNum(48,4,-5,3,16);
  OLED_ShowSignedNum(48,6,CNT,3,16);
}


void Page3_Init(void){//��ҳ��һ
  OLED_ShowString(0,0,"      Flags     ",16);//��������ſ���ֱ�ӽ������С��6
  OLED_ShowString(0,2,"Flag1:          ",16);
  OLED_ShowString(0,4,"Flag2:          ",16);
  OLED_ShowString(0,6,"Flag3:          ",16);
}
void Page3(void){//�ú���������ʾһЩ��̬������
  OLED_ShowFloatNum(48,2,0.0f,33,16);
  OLED_ShowSignedNum(48,4,0,3,16);
  OLED_ShowSignedNum(48,6,-1,3,16);
}

/*
 *����Whileѭ��ˢ����Ļ
 * �����Ҫ����µ�ҳ��������ж��ж�Ӽ���
 * ����PageN_Initָ��Nҳ�ľ�̬��ʾ���ָò���ֻ���ڽ��뵱ǰҳ��ʱ��ʼ��һ�Σ�
 *                      ��PageN���ǵ�Nҳ��Ҫ��̬��ʾ�Ĳ��֣����Խ�һЩ�仯��ֵ����������ʾ
 * */
void LOOP(void){
  static uint8_t PageSelected;
  if (Key1_Detected){//�����ж�
    CNT++;
    Key1_Detected=CLEAR;//�����־λ
  } else if (Key2_Detected){
    Page_Choose++;
    Page_Choose%=2;
    if (Page_Choose==0){CNT++;}//����˻������棬��ҪCNT++�ſɽ���
    Key2_Detected=CLEAR;
  } else if (Key3_Detected){
    CNT--;
    Key3_Detected=CLEAR;
  }


  if (Page_Choose!=TRUE){//����ҳ�жϣ��������Ӧ��ҳ��
    PageMain();
    PageSelected=CNT;//��ʱ�洢��ǰѡ�е�Page
    Static_Show=TRUE;
  }
  else{//���ѡ����뵱ǰҳ��
    switch (PageSelected) {
      case 1:
        if (Static_Show==TRUE){
          Page1_Init();//������ʾ�ĵĹ��ർ����Ļˢ���ʽ���
          Static_Show=CLEAR;
        }
        Page1();break;
      case 2:
        if (Static_Show==TRUE){
          Page2_Init();//������ʾ�ĵĹ��ർ����Ļˢ���ʽ���
          CNT=CLEAR;//��������ڽ���ֵ��λ
          Static_Show=CLEAR;
        }
        Page2();break;
      case 3:
        if (Static_Show==TRUE){
          Page3_Init();//������ʾ�ĵĹ��ർ����Ļˢ���ʽ���
          Static_Show=CLEAR;
        }
        Page3();break;
      default:
        break;
    }
//    if (PageSelected==1){
//      if (Static_Show==TRUE){
//        Page1_Init();//������ʾ�ĵĹ��ർ����Ļˢ���ʽ���
//        Static_Show=CLEAR;
//      }Page1();
//    } else if (PageSelected==2){
//      if (Static_Show==TRUE){
//        Page2_Init();//������ʾ�ĵĹ��ർ����Ļˢ���ʽ���
//        CNT=CLEAR;//��������ڽ���ֵ��λ
//        Static_Show=CLEAR;
//      }Page2();
//    } else if (PageSelected==3){
//      if (Static_Show==TRUE){
//        Page3_Init();//������ʾ�ĵĹ��ർ����Ļˢ���ʽ���
//        Static_Show=CLEAR;
//      }Page3();
//    }
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){//��ʱ���ж����ڼ�ⰴ��
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

/*�������ؼ�ⰴ��*/
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//  if (GPIO_Pin==Key1_Pin){
//    Key1_Detected=TRUE;
//  } else if (GPIO_Pin==Key2_Pin){
//    Key2_Detected=TRUE;
//  } else if (GPIO_Pin==Key3_Pin){
//    Key3_Detected=TRUE;
//  }
//}