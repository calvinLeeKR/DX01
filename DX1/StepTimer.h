#pragma once

struct StepTimer
{
    ULONGLONG countFrame = 0;
    ULONGLONG timeStart = 0;
    ULONGLONG timeLast = 0;
    ULONG timeElapsed = 0;
    float fElapsed = 0.f;

    float Tick();
};

extern StepTimer g_StepTimer;


