#pragma once
#include <wtypes.h>

bool IsStartupEnabled();

void AddToStartup();

void RemoveFromStartup();

bool IsFirstRun();

void SetFirstRunComplete();
