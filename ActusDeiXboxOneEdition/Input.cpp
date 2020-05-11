#include "pch.h"
#include "Input.h"

Windows::Gaming::Input::Gamepad^ Input::controller = nullptr;
float Input::epsilon = 0.001;

void Input::OnGamepadAdded(Platform::Object^ sender, Gamepad^ args)
{
	// If no controller connected, do the thing
	if (Input::controller == nullptr)
		Input::controller = args;
}

void Input::OnGamepadRemoved(Platform::Object^ sender, Gamepad^ args)
{
	if (Input::controller == args)
		Input::controller = nullptr;
}

bool Input::ControllerPresent()
{
	return Input::controller != nullptr;
}

Windows::Gaming::Input::GamepadReading Input::GetControllerState()
{
	GamepadReading reading = { 0 };

	if (Input::controller)
	{
		reading = Input::controller->GetCurrentReading();
	}
	
	return reading;
}

bool Input::QueryButtonDown(GamepadButtons button)
{
	bool button_state = false;

	if (Input::controller)
	{
		button_state = (button == (Input::controller->GetCurrentReading().Buttons & button));
	}

	return button_state;
}

bool Input::QueryButtonUp(GamepadButtons button)
{
	bool button_state = true;

	if (Input::controller)
	{
		button_state = (GamepadButtons::None == (Input::controller->GetCurrentReading().Buttons & button));
	}

	return button_state;
}

int Input::QueryTriggerUpDown(TRIGGERS trigger, float tolerance /*= 0*/)
{
	int button_state = 0;
	if (Input::controller)
	{
		float trigger_value = 0;
		if (trigger == TRIGGERS::RIGHT_TRIGGER)
			trigger_value = Input::controller->GetCurrentReading().RightTrigger;
		else if (trigger == TRIGGERS::LEFT_TRIGGER)
			trigger_value = Input::controller->GetCurrentReading().LeftTrigger;

		if (trigger_value > tolerance)
			button_state = 1;
	}

	return button_state;
}

float Input::QueryTriggerValueExact(TRIGGERS trigger)
{
	return QueryTriggerValueWithTolerance(trigger, 0);
}

float Input::QueryTriggerValueWithTolerance(TRIGGERS trigger, float tolerance /*= 0.6*/)
{
	float button_value = 0;
	if (Input::controller)
	{
		if (trigger == TRIGGERS::RIGHT_TRIGGER)
			button_value = Input::controller->GetCurrentReading().RightTrigger;
		else if (trigger == TRIGGERS::LEFT_TRIGGER)
			button_value = Input::controller->GetCurrentReading().LeftTrigger;

		// Filter for tolerance
		if (fabs(button_value) < tolerance) button_value = 0;
	}

	return button_value;
}

int Input::QueryThumbStickLeftRightX(THUMBSTICKS stick)
{
	int stick_state = 0;

	if (QueryThumbStickValueWithToleranceX(stick) > epsilon) stick_state = 1;
	else if (QueryThumbStickValueWithToleranceX(stick) < -epsilon) stick_state = -1;

	return stick_state;
}

float Input::QueryThumbStickValueExactX(THUMBSTICKS stick)
{
	return QueryThumbStickValueWithToleranceX(stick, 0);
}

float Input::QueryThumbStickValueWithToleranceX(THUMBSTICKS stick, float tolerance /*= 0.6*/)
{
	float thumb_value = 0;

	if (Input::controller)
	{
		if (stick == THUMBSTICKS::LEFT_THUMBSTICK)
			thumb_value = Input::controller->GetCurrentReading().LeftThumbstickX;
		else if (stick == THUMBSTICKS::RIGHT_THUMBSTICK)
			thumb_value = Input::controller->GetCurrentReading().RightThumbstickX;

		// Filter for tolerance
		if (fabs(thumb_value) < tolerance) thumb_value = 0;
	}

	return thumb_value;
}

int Input::QueryThumbStickUpDownY(THUMBSTICKS stick)
{
	int stick_state = 0;

	if (QueryThumbStickValueWithToleranceY(stick) > epsilon) stick_state = 1;
	else if (QueryThumbStickValueWithToleranceY(stick) < -epsilon) stick_state = -1;

	return stick_state;
}

float Input::QueryThumbStickValueExactY(THUMBSTICKS stick)
{
	return QueryThumbStickValueWithToleranceY(stick, 0);
}

float Input::QueryThumbStickValueWithToleranceY(THUMBSTICKS stick, float tolerance /*= 0.6*/)
{
	float thumb_value = 0;

	if (Input::controller)
	{
		if (stick == THUMBSTICKS::LEFT_THUMBSTICK)
			thumb_value = Input::controller->GetCurrentReading().LeftThumbstickY;
		else if (stick == THUMBSTICKS::RIGHT_THUMBSTICK)
			thumb_value = Input::controller->GetCurrentReading().RightThumbstickY;

		// Filter for tolerance
		if (fabs(thumb_value) < tolerance) thumb_value = 0;
	}

	return thumb_value;
}