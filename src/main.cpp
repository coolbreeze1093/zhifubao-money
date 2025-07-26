#include <Arduino.h>
#include <ESP32Servo.h>
#include <math.h>
#include <vector>

int APin = 13;
Servo servo_h;
Servo servo_w;
int freq = 1000;
const double start_delay = 3.0;
const float end_delay = 12.0;
const float height_start = 15;
const float height_min =12;
const float height_max = 130;
const float weight_max = 90;
const float weight_min = 45;
const int step = 50;

std::vector<float> delay_vector;
std::vector<float> height_vector;
std::vector<float> weight_vector;

void delayfunc(std::vector<float> &deltas,float start = 5, float end = 12, int stepnum = 70)
{
  //float b = end;
  //float a = (start - b) / pow(stepnum, 2);

  for (int i = 0; i < stepnum; i += 1)
  {
    deltas.push_back(0.015 * pow(i-18, 2)+  start);
  }
}

void heightFunc(std::vector<float> &deltas,float start = 13, float end = 30,float min=4, int stepnum = 20)
{
  // y = 0.065*x*x+4
  double b = min;
  double c = (double)stepnum/4.0;
  double a = (start - b) / pow(c, 2);

  Serial.print("heightFunc: a:");
  Serial.print(a);
  Serial.print("  b:");
  Serial.print(b);
  Serial.print("  c:");
  Serial.println(c);


  for (int i = 0; i < stepnum; i += 1)
  {
    double _r=a * pow((i-c), 2) + b;
    deltas.push_back(_r);
    Serial.print("heightFunc: ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(deltas[i]);
  }
}

void weightFunc(std::vector<float> &deltas,float start = 50, float end = 95, int stepnum = 50)
{
  float b = end;
  float c = stepnum;
  float a = (start - b) / pow(c, 2);
  for (int i = 0; i < stepnum; i += 1)
  {
    deltas.push_back(a * pow((i-c), 2) + b);
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

  servo_h.attach(16);
  servo_w.attach(17);

  delayfunc(delay_vector,start_delay, end_delay, step);
  heightFunc(height_vector,height_start, height_max,height_min, step);
  weightFunc(weight_vector,weight_min, weight_max, step);
  delay(1000);
}
void loop()
{
  int rand_val = random(2, 8);
  delay(rand_val * 1000);

  //Serial.println(rand_val);

  for(int i = weight_vector[step-1]; i > weight_vector[0]; i-=2)
  {
     servo_w.write(i);
     delay(12);
  }
  
  for(int i=height_vector[step-1];i>height_vector[0];i-=2)
  {
    servo_h.write(i);
    delay(12);
  }
  delay(50);
  servo_w.release();
  servo_h.release();  
  delay(500);
  
  for (int i = 0; i < step; i++)
  {
    servo_w.write(weight_vector[i]);
    servo_h.write(height_vector[i]);
    delay(delay_vector[i]);
    //Serial.print("delay:  ");
    //Serial.println(delay_vector[i]);
    Serial.print("height: ");
    Serial.println(height_vector[i]);
    Serial.print("weight: ");
    Serial.println(weight_vector[i]);
    //Serial.println("***********");
  }
  delay(100);
  servo_w.release();
  servo_h.release();  

}
