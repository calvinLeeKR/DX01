#include "pch.h"
#include "StepTimer.h"

float StepTimer::Tick()
{
    ULONGLONG timeCur = GetTickCount64();
    if (timeStart == 0)
        timeStart = timeCur;
    timeElapsed = (timeCur - timeLast);
    fElapsed = timeElapsed / 1000.0f;
    timeLast = timeCur;
    ++countFrame;

    return fElapsed;
}

StepTimer g_StepTimer;