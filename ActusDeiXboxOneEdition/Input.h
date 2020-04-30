#pragma once

#include <windows.gaming.input.h>

using namespace Windows::Gaming::Input;

#define TOLERANCE .5

class Input
{
public:
	enum class TRIGGERS
	{
		RIGHT_TRIGGER,
		LEFT_TRIGGER
	};

	enum class THUMBSTICKS
	{
		RIGHT_THUMBSTICK,
		LEFT_THUMBSTICK
	};

	enum class DIRECTION
	{
		UP = 1,
		DOWN = -1,
		LEFT = -1,
		RIGHT = 1,
	};

	static void OnGamepadAdded(Platform::Object^ sender, Gamepad^ args);
	static void OnGamepadRemoved(Platform::Object^ sender, Gamepad^ args);

	static bool ControllerPresent();

	static GamepadReading GetControllerState();

	// Queries
	static bool QueryButtonDown(GamepadButtons button);
	static bool QueryButtonUp(GamepadButtons button);
	static int QueryTriggerUpDown(TRIGGERS trigger, float tolerance = TOLERANCE);
	static float QueryTriggerValueExact(TRIGGERS trigger);
	static float QueryTriggerValueWithTolerance(TRIGGERS trigger, float tolerance = TOLERANCE);
	static int QueryThumbStickLeftRightX(THUMBSTICKS stick);
	static float QueryThumbStickValueExactX(THUMBSTICKS stick);
	static float QueryThumbStickValueWithToleranceX(THUMBSTICKS stick, float tolerance = TOLERANCE);
	static int QueryThumbStickUpDownY(THUMBSTICKS stick);
	static float QueryThumbStickValueExactY(THUMBSTICKS stick);
	static float QueryThumbStickValueWithToleranceY(THUMBSTICKS stick, float tolerance = TOLERANCE);
private:
	static Gamepad^ controller;
	static float epsilon;
};