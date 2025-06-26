#include <Arduino.h>

#define IR1 1
#define IR2 2
#define IR3 3
#define IR4 4
#define IR5 5
#define IR6 6
#define IR7 7
#define IR8 8

#define IRoutput1 0
#define IRoutput2 10


int n=0,m=0;
int dis_count = 0;
int IR_disx;
int dis;

void setup(){
  analogWriteResolution(12);
  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(IR3,INPUT);
  pinMode(IR4,INPUT);
  pinMode(IR5,INPUT);
  pinMode(IR6,INPUT);
  pinMode(IR7,INPUT);
  pinMode(IR8,INPUT);

  Serial.begin(9600);

}

void loop(){
    
  float IRmax1 = 1023;
  float IRmax2 = 1023;
  float IRmax3 = 1023;
  float IRval  = 1023;
  float IR_No1 = 8.00;
  float IR_No2 = 8.00;
  float IR_No3 = 8.00;
  for (int i = 1; i < 9; i++){
    IRval = analogRead(i); 
    IRmax1 = analogRead(IR_No1);
    IRmax2 = analogRead(IR_No2);
    IRmax3 = analogRead(IR_No3);

    if(IRval<IRmax1){
      IR_No3 = IR_No2;
      IR_No2 = IR_No1;
      IR_No1 = i;
    }else if(IRval<IRmax2){
      IR_No3 = IR_No2;
      IR_No2 = i;
    }else if(IRval<IRmax3){
      IR_No3 = i;
    }
  }


IRmax1 = analogRead(IR_No1);
IRmax2 = analogRead(IR_No2);
IRmax3 = analogRead(IR_No3);

IR_No1 = (IR_No1 - 1) * 45;
IR_No2 = (IR_No2 - 1) * 45;
IR_No3 = (IR_No3 - 1) * 45;


/*if(IR_No1 == 0){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}




if(IR_No1 == 0){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}





if(IR_No1 == 0){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}else if(IR_No1 == 1){
  if(IRmax1 < 200){
    IRmax1 = 100;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }else if(IRmax1 < 300){
    IRmax1 = 300;
  }
}a*/



float IR_No1x = IRmax1*sin(radians(IR_No1));
float IR_No1y = IRmax1*cos(radians(IR_No1));
float IR_No2x = IRmax2*sin(radians(IR_No2));
float IR_No2y = IRmax2*cos(radians(IR_No2));
float IR_No3x = IRmax3*sin(radians(IR_No3));
float IR_No3y = IRmax3*cos(radians(IR_No3));

float IRx;
float IRy;

IRx = IR_No1x + IR_No2x ;
IRy = IR_No1y + IR_No2y ;

int IR_dis = sqrt(pow(IRx,2)+pow(IRy,2));//距離
int IR_dir = degrees(atan2(-IRx,IRy)+ PI);//角度 atan2(y,x)
//IR_dir = (IR_dir + 180);

IR_dis = IR_dis/5;

if(IRmax1>800){
  IR_dir=360;
  IR_dis=4000;
}


  Serial.print(analogRead(IR1));
  Serial.print(" ");
  Serial.print(analogRead(IR2));
  Serial.print(" ");
  Serial.print(analogRead(IR3));
  Serial.print(" ");
  Serial.print(analogRead(IR4));
  Serial.print(" ");
  Serial.print(analogRead(IR5));
  Serial.print(" ");
  Serial.print(analogRead(IR6));
  Serial.print(" ");
  Serial.print(analogRead(IR7));
  Serial.print(" ");
  Serial.print(analogRead(IR8));
  Serial.print("            ");
  Serial.println(IR_dis);
  Serial.print("                                                            ");
  Serial.println(IR_dir);
  Serial.print("                                                                     ");
  Serial.print(IR_No1);
  Serial.print(" ");
  Serial.print(IR_No2);
  Serial.print(" ");
  Serial.print(IR_No3);
  Serial.println("      ");
  delay(100);

  if(IRmax1>800){
    analogWrite(IRoutput1,1022);
    analogWrite(IRoutput2,1022);
  }else{
    analogWrite(IRoutput1,(IR_dir*2));
    analogWrite(IRoutput2,((IR_dis-100)*5));
  }
  if(IR_dir > 355){
    analogWrite(IRoutput1,(0));
    analogWrite(IRoutput2,((IR_dis-100)*5));
  }
}


  

/*
if(IR_No1 == 8){
  IRmax1 = IRmax1 * 370/345;
} else if(IR_No1 == 7){
  IRmax1 = IRmax1 * 370/382;
} else if(IR_No1 == 6){
  IRmax1 = IRmax1 * 370/362.5;
} else if(IR_No1 == 5){
  IRmax1 = IRmax1 * 370/367.5;
} else if(IR_No1 == 4){
  IRmax1 = IRmax1 * 370/387.5;
} else if(IR_No1 == 3){
  IRmax1 = IRmax1 * 370/370;
} else if(IR_No1 == 2){
  IRmax1 = IRmax1 * 370/390;
} else if(IR_No1 == 1){
  IRmax1 = IRmax1 * 370/360;
}

if(IR_No2 == 8){
  IRmax2 = IRmax2 * 370/350;
} else if(IR_No2 == 7){
  IRmax2 = IRmax2 * 370/382;
} else if(IR_No2 == 6){
  IRmax2 = IRmax2 * 370/362.5;
} else if(IR_No2 == 5){
  IRmax2 = IRmax2 * 370/350.5;
} else if(IR_No2 == 4){
  IRmax2 = IRmax2 * 370/387.5;
} else if(IR_No2 == 3){
  IRmax2 = IRmax2 * 370/370;
} else if(IR_No2 == 2){
  IRmax2 = IRmax2 * 370/390;
} else if(IR_No2 == 1){
  IRmax2 = IRmax2 * 370/360;
}

if(IR_No3 == 8){
  IRmax3 = IRmax3 * 370/350;
} else if(IR_No3 == 7){
  IRmax3 = IRmax3 * 370/382;
} else if(IR_No3 == 6){
  IRmax3 = IRmax3 * 370/362.5;
} else if(IR_No3 == 5){
  IRmax3 = IRmax3 * 370/363.5;
} else if(IR_No3 == 4){
  IRmax3 = IRmax3 * 370/387.5;
} else if(IR_No3 == 3){
  IRmax3 = IRmax3 * 370/370;
} else if(IR_No3 == 2){
  IRmax3 = IRmax3 * 370/390;
} else if(IR_No3 == 1){
  IRmax3 = IRmax3 * 370/360;
}



//1~8 → 0~7
IR_No1 = ((IR_No1 - 9) * -1) - 1;
IR_No2 = ((IR_No2 - 9) * -1) - 1;
IR_No3 = ((IR_No3 - 9) * -1) - 1;

//それぞれxy方向に分解して新しい変数に保存する
IR_No1 = IR_No1 * 45;
IR_No2 = IR_No2 * 45;
IR_No3 = IR_No3 * 45;


float IR_No1x = IRmax1*sin(radians(IR_No1));
float IR_No1y = IRmax1*cos(radians(IR_No1));
float IR_No2x = IRmax2*sin(radians(IR_No2));
float IR_No2y = IRmax2*cos(radians(IR_No2));
float IR_No3x = IRmax3*sin(radians(IR_No3));
float IR_No3y = IRmax3*cos(radians(IR_No3));

float IRx;
float IRy;
int DIS;


if(abs(IRmax2 - IRmax1) > 50){
  IRx = IR_No1x;
  IRy = IR_No1y;
  DIS = 3;
} else if(abs(IRmax1 - IRmax3) > 70){
  IRx = IR_No1x + IR_No2x;
  IRy = IR_No1y + IR_No2y;
  DIS = 1.5;
}else{
  IRx = IR_No1x + IR_No2x + IR_No3x;
  IRy = IR_No1y + IR_No2y + IR_No3y;
  DIS = 1;
}

int IR_dis = sqrt(pow(IRx,2)+pow(IRy,2));//距離
int IR_dir = degrees(atan2(-IRx,-IRy));//角度
IR_dir = (IR_dir + 180);

IR_dis = IR_dis/5*DIS;

if(IRmax1>800){
  IR_dir=360;
  IR_dis=4000;
}
*/