#include <Arduino.h>
#include <ESP32Servo.h>
#include <math.h>
#include <vector>

int APin = 13;
Servo servo;
Servo servo2;
int freq = 1000;
const double start_delay = 3.0;
const float end_delay = 12.0;
const float height_min = 5;
const float height_max = 30;
const int height_step = 20;
const float weight_max = 95;
const float weight_min = 50;
const int weight_stepnum = 50;
const int delay_stepnum = weight_stepnum + height_step;

std::vector<float> delay_vector;
std::vector<float> height_vector;
std::vector<float> weight_vector;

void delayfunc(std::vector<float> &deltas,float start = 3, float end = 12, int stepnum = 70)
{
  float b = start;
  float a = (end - b) / pow(stepnum, 2);

  for (int i = 0; i < stepnum; i += 1)
  {
    deltas.push_back(a * pow(i, 2) + b);
  }
}

void heightFunc(std::vector<float> &deltas,float start = 4, float end = 30, int stepnum = 20)
{
  // y = 0.065*x*x+4
  float b = start;
  float a = (end - b) / pow(stepnum, 2);

  for (int i = 0; i < stepnum; i += 1)
  {
    deltas.push_back(a * pow(i, 2) + b);
  }
}

void weightFunc(std::vector<float> &deltas,float start = 50, float end = 95, int stepnum = 50)
{
  float b = start;
  float a = (end - b) / pow(stepnum, 3);
  for (int i = 0; i < stepnum; i += 1)
  {
    deltas.push_back(a * pow(i, 3) + b);
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

  delayfunc(delay_vector,start_delay, end_delay, delay_stepnum);
  heightFunc(height_vector,height_min, height_max, height_step);
  weightFunc(weight_vector,weight_min, weight_max, weight_stepnum);
  delay(1000);
}

double height = 0;
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

  for (; height > height_min; height -= 2, nums++)
  {
    delayTime -= delay_vector[nums];
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
    delayTime -= delay_vector[nums];
    // Serial.println(nums);
    delay(delayTime);
  }
  Serial.println("2");
  for (; height < 30; height += 2, nums++)
  {
    servo.write(height);
    delayTime -= delay_vector[nums];
    delay(delayTime);
  }
  Serial.println("3");
  delay(200);
}
