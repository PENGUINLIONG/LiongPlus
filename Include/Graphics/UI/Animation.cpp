// File: Animation.cpp
// Author: Rendong Liang (Liong)
#include "Animation.hpp"
#include "UIUpdater.Animation.hpp"

_L_NS_BEG_GRAPHICS_UI

using namespace std;

//
// AnimationTimeline
//

tuple<Float, AnimationState> AnimationTimeline::GetTimeRatio(const TimePoint& now)
{
	// Use goto to prevent stack overflow.
Begin:
	auto ratio = double((now - BeginTime).count()) / double(Duration.count());
	// Animation has not begun. $BeginTime is not reached by $now.
	if (ratio < 0) return { 0, AnimationState::Ready };
	// Animation began yet not finished.
	if (ratio < 1) return { ratio, AnimationState::Forward };
	// Forward process is finished. Reverse if necessary.
	if (ShouldReverse)
	{
		// Out of backward stage. If there is no need to repeat, finish the animation.
		if (RepeatTimes == 0) return { 0, AnimationState::Finished };
		// Backward stage.
		if (ratio < 2) return { 2 - ratio, AnimationState::Backward };
		// Limited times of repeating. Reduce $RepeatTimes each time it finished.
		if (RepeatTimes > 0) --RepeatTimes;
		// Forward and backward time should both be counted.
		BeginTime += 2 * Duration;
	}
	// No need for backward.
	else
	{
		if (RepeatTimes == 0) return { 0, AnimationState::Finished };
		if (RepeatTimes > 0) --RepeatTimes;
		BeginTime += Duration;
	}
	goto Begin;
}

bool Animation::Apply(const Animation& anim, const UIProperty prop, const IUIElement& target, bool force)
{
	return UIUpdater::_ApplyAnimation(anim, prop, target, force);
}
void Animation::Cancel(const UIProperty prop, const IUIElement& target)
{
	UIUpdater::_CancelAnimation(prop, target);
}

// Easing functions.

EasingFunction::EasingFunction(EasingMode mode) :
	_Mode(mode)
{
}

PowerEasingFunction::PowerEasingFunction(Float power, EasingMode mode) :
	EasingFunction(mode),
	_Power(power)
{
}
Float PowerEasingFunction::Ease(Float timeRatio) const
{
	Float funcOutput;
	if (EasingMode::EaseInOut == _Mode)
	{
		funcOutput = timeRatio < .5 ?
			std::pow(timeRatio * 2., _Power) / 2. :
			1. - std::pow((1. - timeRatio) * 2., _Power) / 2.;
	}
	else // Ease in or out only.
	{
		funcOutput = EasingMode::EaseIn == _Mode ?
			std::pow(timeRatio, _Power) :
			1. - std::pow(1. - timeRatio, _Power);
	}
	return funcOutput;
}

shared_ptr<PowerEasingFunction> UIFactory<PowerEasingFunction>::Create(Float power, EasingMode mode)
{
	return make_shared<PowerEasingFunction>(power, mode);
}

// Updaters.

template<typename T>
NumericUpdater<T>::NumericUpdater(Float from, Float to) :
	_From(from), _To(to)
{
}
template<typename T>
void NumericUpdater<T>::Update(const UIProperty& prop, const IUIElement& target, Float progress) const
{
	target->SetProperty(prop, T(_From * progress + _To * oneMinusProgress));
}

template<typename T>
shared_ptr<NumericUpdater<T>> UIFactory<NumericUpdater<T>>::Create(T from, T to)
{
	return make_shared<NumericUpdater<T>>(from, to);
}

VectorUpdater::VectorUpdater(const Vector4& from, const Vector4& to) :
	_From(from), _To(to)
{
}
void VectorUpdater::Update(const UIProperty& prop, const IUIElement& target, Float progress) const
{
	Float oneMinusProgress = 1. - progress;
	target->SetProperty(prop, Vector4{
		_From.X * progress + _To.X * oneMinusProgress,
		_From.Y * progress + _To.Y * oneMinusProgress,
		_From.Z * progress + _To.Z * oneMinusProgress,
		_From.W * progress + _To.W * oneMinusProgress
	});
}

ThicknessUpdater::ThicknessUpdater(const Thickness& from, const Thickness& to) :
	_From(from), _To(to)
{
}
void ThicknessUpdater::Update(const UIProperty& prop, const IUIElement& target, Float progress) const
{
	Float oneMinusProgress = 1. - progress;
	target->SetProperty(prop, Thickness{
		_From.Left * progress + _To.Left * oneMinusProgress,
		_From.Top * progress + _To.Top * oneMinusProgress,
		_From.Right * progress + _To.Right * oneMinusProgress,
		_From.Bottom * progress + _To.Bottom * oneMinusProgress
	});
}



SizeUpdater::SizeUpdater(const Size& from, const Size& to) :
	_From(from), _To(to)
{
}
void SizeUpdater::Update(const UIProperty& prop, const IUIElement& target, Float progress) const
{
	Float oneMinusProgress = 1. - progress;
	target->SetProperty(prop, Size{
		_From.Width * progress + _To.Width * oneMinusProgress,
		_From.Height * progress + _To.Height * oneMinusProgress,
	});
}

_L_NS_END_GRAPHICS_UI
