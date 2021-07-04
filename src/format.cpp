#include "format.h"




using namespace std;
using std::string;

string Format::ElapsedTime(long seconds) { 
    chrono::seconds sec(seconds);
    long hr = chrono::duration_cast<chrono::hours>(sec).count();
    long min = chrono::duration_cast<chrono::minutes>(chrono::seconds(seconds -(hr*3600))).count();         
    long seco = seconds -((hr*3600)+(min*60));
    return to_string(hr) +"::"+ to_string(min)+"::"+ to_string(seco); }  