#pragma once

class IViewEvents
{
public:
	virtual ~IViewEvents() = default;
	virtual void onWelcomeKeyPressed(char value) = 0;
};
