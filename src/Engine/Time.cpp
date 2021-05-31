#include "Engine/Time.h"

namespace Time
{
	static float deltaTime{};
	static float unscaledDeltaTime{};

	static float timeScale{ 1 };

	void SetUDT(float udt)
	{
		unscaledDeltaTime = udt;
		deltaTime = udt * timeScale;
	}

	float GetDeltaTime()
	{
		return deltaTime;
	}

	float GetUnscaledDeltaTime()
	{
		return unscaledDeltaTime;
	}

	void SetTimeScale(float ts)
	{
		timeScale = ts;
	}

	float GetTimeScale()
	{
		return timeScale;
	}
}