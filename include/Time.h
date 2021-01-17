#pragma once
namespace Time
{
	void SetUDT(float udt);
	float GetDeltaTime();
	float GetUnscaledDeltaTime();

	void SetTimeScale(float ts);
	float GetTimeScale();
};

