// File: Animation.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <atomic>
#include <functional>
#include <tuple>
#include <type_traits>
#include "_"
#include "UIElement.hpp"
#include "UIFactory.hpp"

_L_NS_BEG_GRAPHICS_UI

enum class AnimationState
{
	Ready, Forward, Backward, Finished
};

enum class EasingMode
{
	EaseIn, EaseOut, EaseInOut
};

class EasingFunction
{
protected:
	EasingMode _Mode;

public:
	EasingFunction(EasingMode mode);

	virtual Float Ease(Float timeRatio) const = 0;
};

using IEasingFunction = I<EasingFunction>;

/*
 * AnimationUpdater
 *
 * This class provide functionality to update animation effect to target object.
 * An updater derived from this class should construct using, or provide with setter method to
 *   store information it needed to update animation.
 */
class AnimationUpdater
{
public:
	virtual void Update(const UIProperty& prop, const IUIElement& target, Float progress) const = 0;
};

using IAnimationUpdater = I<AnimationUpdater>;

class AnimationTimeline
{
public:
	Duration Duration;
	bool ShouldReverse = false;
	int RepeatTimes = 0;
	TimePoint BeginTime = std::chrono::steady_clock::now();
	
	std::tuple<Float, AnimationState> GetTimeRatio(const TimePoint& now);
};

class Animation
{
public:
	AnimationTimeline Timeline;
	IEasingFunction EasingFunction;
	IAnimationUpdater Updater;

	/*
	 * Apply animation to $prop of $target.
	 *
	 * Params:
	 *   $anim: Animation to be applied. The animation will be copied and thus can be reused.
	 *   $prop: Property to be animated.
	 *   $target: Target UI element to be animated.
	 *   $force: A flag indicating whether to interupt existing animation.
	 *      No exception will be thrown if there is an existing animation with $force set false.
	 * Returns:
	 *   True is the animation is applied successfully. Otherwise, false.
	 */
	static bool Apply(const Animation& anim, const UIProperty prop, const IUIElement& target, bool force = false);
	/*
	 * Cancel the animation applied to $prop of $target.
	 */
	static void Cancel(const UIProperty prop, const IUIElement& target);
};

// Easing functions.

/*
 * Implementation of power easing function.
 * 
 *     1 +-  -  -  -X
 *       |         /:
 *  x(t) |        /
 *       |     , `  :
 *     0 +--`-------+
 *       0    t     1
 * The power function is calculated by:
 *   t^{$power}
 * A higher $power will lead to higher acceleration at the end.
 */
class PowerEasingFunction
	: public EasingFunction
{
private:
	Float _Power;

public:
	PowerEasingFunction(Float power, EasingMode mode);

	Float Ease(Float timeRatio) const;
};

template<> struct UIFactory<PowerEasingFunction>
{
public: static std::shared_ptr<PowerEasingFunction> Create(Float power, EasingMode mode = EasingMode::EaseOut);
};

// Updaters.

template<typename T>
class NumericUpdater : public AnimationUpdater
{
private:
	Float _From, _To;
public:
	NumericUpdater(Float from, Float to);
	void Update(const UIProperty& prop, const IUIElement& target, Float progress) const override;
};

template<typename T> struct UIFactory<NumericUpdater<T>>
{
public: static std::shared_ptr<NumericUpdater<T>> Create(T from, T to);
};

class VectorUpdater : public AnimationUpdater
{
private:
	Vector4 _From, _To;
public:
	VectorUpdater(const Vector4& from, const Vector4& to);
	void Update(const UIProperty& prop, const IUIElement& target, Float progress) const override;
};

template<> struct UIFactory<VectorUpdater>
{
public: static std::shared_ptr<VectorUpdater> Create();
};

class ThicknessUpdater : public AnimationUpdater
{
private:
	Thickness _From, _To;
public:
	ThicknessUpdater(const Thickness& from, const Thickness& to);
	void Update(const UIProperty& prop, const IUIElement& target, Float progress) const override;
};

template<> struct UIFactory<ThicknessUpdater>
{
public: static std::shared_ptr<ThicknessUpdater> Create();
};

class SizeUpdater : public AnimationUpdater
{
private:
	Size _From, _To;
public:
	SizeUpdater(const Size& from, const Size& to);
	void Update(const UIProperty& prop, const IUIElement& target, Float progress) const override;
};

template<> struct UIFactory<SizeUpdater>
{
public: static std::shared_ptr<SizeUpdater> Create();
};

_L_NS_END_GRAPHICS_UI
