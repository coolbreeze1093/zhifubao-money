#include <Arduino.h>
#include <ESP32Servo.h>
#include <math.h>
#include <vector>

int APin = 13;
Servo servo;
Servo servo2;
int freq = 1000;
const double start_delay = 12.0;
std::vector<double> deltas;
std::vector<float> deltas2;
std::vector<float> deltas3;
void delayfunc()
{
  const double end = 3;
  const int steps = 50;

  // 计算二次函数系数k
  double sum_squares = 0;
  for (int i = 0; i < steps; ++i)
  {
    sum_squares += pow(i, 2);
  }
  const double k = (start_delay - end) / sum_squares;

  // 生成步长序列

  for (int i = 0; i < steps; ++i)
  {
    deltas.push_back(k * pow(i, 2));
  }
}

void heightFunc(float start=4,float end=30,int stepnum=20,std::vector<float> &deltas)
{
  //y = 0.065*x*x+4
  float b=4;
  float a=(end-b)/pow(stepnum,2);

  for(int i=0;i<stepnum;i+=1)
  {
    deltas.push_back(a*pow(i, 2)+b);
  }
}

void weightFunc(float start=50,float end=95,int stepnum=50,std::vector<float> &deltas)
{
  float b=50;
  float a=(end-b)/pow(stepnum,3);
  for(int i=0;i<stepnum;i+=1)
  {
    deltas.push_back(a*pow(i, 3)+b);
  }
}

void setup()
{
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  Serial.begin(115200);

  servo.attach(APin);
  servo2.attach(12);

  delayfunc();
  delay(1000);
}

double height = 0;
double height_min = 5;
double orig = 1.0;
double coe = 0.5;
double weight_start = 50;
double delayTime = start_delay;
double width = weight_start;
void loop()
{
  int rand_val = random(4, 8);
  delay(rand_val * 1000);

  Serial.println(rand_val);

  int nums = 0;
  delayTime = start_delay;
  servo2.write(weight_start);

  for (; height > height_min; height-=2, nums++)
  {
    delayTime -= deltas[nums];
    delay(delayTime);
    servo.write(height);
  }
  Serial.println("1");
  height = height_min;
  for (int i = weight_start; i < 95; i += 2, nums++)
  {
    height = height + (orig + sin(height * (M_PI / 180.0))) * coe;
    servo.write(height);
    servo2.write(i);
    delayTime -= deltas[nums];
    //Serial.println(nums);
    delay(delayTime);
  }
  Serial.println("2");
  for (; height < 30; height+=2, nums++)
  {
    servo.write(height);
    delayTime -= deltas[nums];
    delay(delayTime);
  }
  Serial.println("3");
  delay(200);
  
  
}
