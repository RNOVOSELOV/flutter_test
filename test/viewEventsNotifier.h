#pragma once

template <typename TViewEvents>
class ViewEventsNotifier
{
private:
	TViewEvents* presenterListener;
public:
	ViewEventsNotifier() :
		presenterListener(nullptr)
	{}

	virtual ~ViewEventsNotifier()
	{}

	void setListener(TViewEvents* listener)
	{
		presenterListener = listener;
	}

	void eraseListener()
	{
		presenterListener = nullptr;
	}

	void notifyPresenter(void (TViewEvents::* pFn)(void))
	{
		if (presenterListener)
		{
			(presenterListener->pFn)();
		}
	}

	template <typename T1>
	void notifyPresenter(void (TViewEvents::* pFn)(T1), T1 p1)
	{
		if (presenterListener)
		{
			(presenterListener->*pFn)(p1);
		}
	}
};

