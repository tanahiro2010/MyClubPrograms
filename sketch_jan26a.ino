#define R PA3
#define L PA4

#define ML_PWM PA8    // モータ左_PWM
#define ML_IN1 PA15    // モータ左_IN1
#define ML_IN2 PB3    // モータ左_IN2

#define MR_PWM PA9   // モータ右_PWM
#define MR_IN1 PB4   // モータ右_IN1
#define MR_IN2 PB5   // モータ右_IN2

#define MB_PWM PA10
#define MB_IN1 PB8
#define MB_IN2 PB9

const bool mode = 1; //動作モード: true,計測モード: false 1=true, 0=false
const int maxPower = 3100;
const int minPower = 2000;
const int moterPower = 240;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial.println("Start");
  
  pinMode(ML_PWM, OUTPUT);
  pinMode(MR_PWM, OUTPUT);
  pinMode(MB_PWM, OUTPUT);
  
  pinMode(ML_IN1, OUTPUT);
  pinMode(MR_IN1, OUTPUT);
  pinMode(MB_IN1, OUTPUT);
  
  pinMode(ML_IN2, OUTPUT);
  pinMode(MR_IN2, OUTPUT);
  pinMode(MB_IN2, OUTPUT);
  
}


void loop(){
  
  if (mode){
    action();  
  }else{
    notAction();
  }
}

void action()
{
  while (true)
  {
    int temp_l = analogRead(L);
    int temp_r = analogRead(R);
    if (temp_l >= maxPower && temp_r <= minPower){
      reactionR();
   
    }else
    if (temp_r >= maxPower && temp_l <= minPower){
      reactionL();
      
    }else
    if (temp_l <= minPower && temp_r <= minPower ||
          temp_l >= maxPower && temp_r >= maxPower){
      B_ST();
      B_FW(moterPower, 0);
  
    }
  }
}

void notAction()
{
  while (true)
  {
    Serial.println("L: " + String(analogRead(L)));
    Serial.println("R: " + String(analogRead(R)));
    delay(500);
  }
}
//-----------------------------------------------------------------------
// ロボット動作（フリー）※モーターに電流を流していない状態（流れてはいる）
//-----------------------------------------------------------------------

void B_ST(){
  /*
  digitalWrite(ML_IN1,LOW);
  digitalWrite(ML_IN2,LOW);
  digitalWrite(ML_PWM,HIGH);
    
  digitalWrite(MR_IN1,LOW);
  digitalWrite(MR_IN2,LOW);
  digitalWrite(MR_PWM,HIGH);
  */
  digitalWrite(MB_IN1,LOW);
  digitalWrite(MB_IN2,LOW);
  digitalWrite(MB_PWM,HIGH);
}


//---------------------------------------------------------------------------------------
// ロボット動作（ブレーキ）※モーターをショートさせている状態（モーターの両端子を直接繫つないでいる状態）
//---------------------------------------------------------------------------------------

void B_BRK(){

  digitalWrite(ML_IN1,HIGH);
  digitalWrite(ML_IN2,HIGH);

  digitalWrite(MR_IN1,HIGH);
  digitalWrite(MR_IN2,HIGH);
  
} 


//--------------------------------------------
// ロボット動作（前進）
//--------------------------------------------

void B_FW(int MPower,int HV){  //B_FW(モーター出力,補正値)　補正値→「+」は進行方向に対して右に曲がる、「-」は進行方向に対して左に曲がる

  int hv = abs(HV);    
  digitalWrite(ML_IN1,LOW);
  digitalWrite(ML_IN2,HIGH);
  digitalWrite(MR_IN1,HIGH);
  digitalWrite(MR_IN2,LOW);

  if(HV <= 0){

    analogWrite(ML_PWM,MPower - hv);
    analogWrite(MR_PWM,MPower);

  }
  else{

    analogWrite(ML_PWM,MPower);
    analogWrite(MR_PWM,MPower - hv);

  }

}


//--------------------------------------------
// ロボット動作（後進）
//--------------------------------------------

void B_BK(int MPower,int HV){  //B_BK(モーター出力,補正値)　補正値→「+」は進行方向に対して左に曲がる、「-」は進行方向に対して右に曲がる
 
  int hv = abs(HV);
  
  digitalWrite(ML_IN1,HIGH);
  digitalWrite(ML_IN2,LOW);
  digitalWrite(MR_IN1,LOW);
  digitalWrite(MR_IN2,HIGH);
    
  if(HV <= 0){

    analogWrite(ML_PWM,MPower - hv);
    analogWrite(MR_PWM,MPower);

  }
  else{

    analogWrite(ML_PWM,MPower);
    analogWrite(MR_PWM,MPower - hv);

  }

}


//--------------------------------------------
// ロボット動作（左回転）
//--------------------------------------------

void B_LT(int MPower){
  
  digitalWrite(ML_IN1,HIGH);
  digitalWrite(ML_IN2,LOW);

  digitalWrite(MR_IN1,HIGH);
  digitalWrite(MR_IN2,LOW);

  analogWrite(ML_PWM,MPower);
  analogWrite(MR_PWM,MPower);
  
}


//--------------------------------------------
// ロボット動作（右回転）
//--------------------------------------------

void B_RT(int MPower){

  digitalWrite(ML_IN1,LOW);
  digitalWrite(ML_IN2,HIGH);

  digitalWrite(MR_IN1,LOW);
  digitalWrite(MR_IN2,HIGH);

  analogWrite(ML_PWM,MPower);
  analogWrite(MR_PWM,MPower);

}


//Reaction
void reactionR(){
  //Serial.println("Right now");
  B_LT(moterPower - 10);
  digitalWrite(MB_IN1, LOW);
  digitalWrite(MB_IN2, HIGH);
  
  
  analogWrite(MB_PWM, 255);
}
  
  
void reactionL(){
  B_RT(moterPower - 10);
  digitalWrite(MB_IN1, HIGH);
  digitalWrite(MB_IN2, LOW);


  analogWrite(MB_PWM, 255);  
}
