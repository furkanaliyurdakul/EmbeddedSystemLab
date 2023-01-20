#include <string>
#include <vector>

//#define INCLUDE_vTaskSuspend 1  // message handlers are blocking

using namespace std;

void task1(void * parameter);
void task2(void * parameter);
void task3(void * parameter);
void task4(void * parameter);

vector<int> tokenizeMessage(const string& message);
double getDistance(double x1, double y1, double x2, double y2);
double getAngle(double x1, double y1, double x2, double y2);
