#ifndef Time_Sync_H
#define Time_Sync_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

//this is my synchronisation class put the Sync function inside a conditional statment (while, if,etc...) to sync a loop.
class TimeSync
{
public:
    float t1{};
    float t2{};
    float SyncClock{};
    float ElapsedTime{};
    //if argument is set to zero or below zero then the function will execute as fast as it can while still providing the ElapsedTime
    bool Sync(float frames)
    {

        t2 = glfwGetTime();
        SyncClock += t2 - t1; //calculates the ElapsedTime
        ElapsedTime = SyncClock;
        t1 = t2;
        if(frames > 0){
            if (SyncClock > (1.0 / (frames+1.0f)))
            {
                SyncClock = 0;
                return true;
            }
            return false;
            }
        SyncClock = 0;
        return true;//if frametime is zero then the code executes as fast as it can
    }
    
};


#endif // !Time_Sync_H

