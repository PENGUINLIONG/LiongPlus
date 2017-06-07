// File: UIUpdater.cpp
// Author: Rendong Liang (Liong)
#pragma once
#include <algorithm>
#include <vector>
#include <map>
#include <memory>
#include "UIUpdater.Animation.hpp"
#include "UIUpdater.UIAction.hpp"
#include "../../Threading/Timer.hpp"

_L_NS_BEG_GRAPHICS_UI

namespace UIUpdater
{
	namespace details
	{
		using namespace std;
		using namespace std::chrono;
		using namespace Threading;

		const milliseconds frameInterval = 15ms;


		struct UpdaterData
		{
			shared_ptr<Timer<milliseconds>> _Timer;
			vector<I<UIActionHost>> _Targets;
			bool
				_NeedRender = false,
				_NeedLayout = false,
				_NeedLogicTree = false;
			multimap<IUIElement, tuple<UIProperty, Animation>> _Animations;
		};

		shared_ptr<UpdaterData> getUpdaterDataOfCurrentThread()
		{
			thread_local static shared_ptr<UpdaterData> data;
			return data == nullptr ? (data = make_shared<UpdaterData>()) : data;
		}
		
		inline void updateAnimations(UpdaterData* data)
		{
			auto now = std::chrono::steady_clock::now();
			auto animIter = data->_Animations.begin();
			const auto animEnd = data->_Animations.end();
			while (animEnd != animIter)
			{
				// Remove invalid animations.
				const auto& target = animIter->first;
				if (!target.IsValid())
				{
					data->_Animations.erase(animIter++);
					continue;
				}

				auto& anim = get<1>(animIter->second);
				const auto& prop = get<0>(animIter->second);

				auto ratioAndState = anim.Timeline.GetTimeRatio(now);
				if (get<1>(ratioAndState) == AnimationState::Finished)
					data->_Animations.erase(animIter++);
				else if (get<1>(ratioAndState) != AnimationState::Ready)
				{
					anim.Updater->Update(prop, target, anim.EasingFunction->Ease(get<0>(ratioAndState)));
					++animIter;
				}
			}
		}
		inline void logicTree(UpdaterData* data)
		{
			for (auto target : data->_Targets)
				target->TriggerLogicTree();
			data->_NeedLogicTree = false;
		}
		inline void layout(UpdaterData* data)
		{
			for (auto target : data->_Targets)
				target->TriggerLayout();
			data->_NeedLayout = false;
		}
		inline void render(UpdaterData* data)
		{
			for (auto target : data->_Targets)
				target->TriggerRender();
			data->_NeedRender = false;
		}

		void doUpdate(const shared_ptr<Timer<milliseconds>>& sender,
			const TimeElapsedEventArgs<milliseconds>& args)
		{
			auto data = getUpdaterDataOfCurrentThread().get();
			// Discard requests sent too long time ago.
			if (steady_clock::now() - args.LastTrigger > frameInterval * 2)
				return;
			// Update animation effects at first as it may influence UI elements' sizes and appearance.
			if (!data->_Animations.empty()) updateAnimations(data);
			// Update logic tree.
			if (data->_NeedLogicTree) logicTree(data);
			// Calculate layout.
			if (data->_NeedLayout) layout(data);
			// Render UI.
			if (data->_NeedRender) render(data);
		}
	}

	void _Register(const I<UIActionHost>& target)
	{
		using namespace details;
		using namespace std::chrono;

		auto data = getUpdaterDataOfCurrentThread().get();

		if (data->_Timer == nullptr)
		{
			data->_Timer = std::make_shared<Threading::Timer<milliseconds>>();
			data->_Timer->SetInterval(frameInterval);
			data->_Timer->Elapse += &doUpdate;
			data->_Timer->Start();
		}

		data->_NeedLogicTree = true;

		data->_Targets.push_back(target);
	}
	void _Unregister(const I<UIActionHost>& target)
	{
		using namespace details;
		auto data = getUpdaterDataOfCurrentThread().get();

		auto it = std::find(data->_Targets.begin(), data->_Targets.end(), target);
		std::swap(*it, data->_Targets.back());
		data->_Targets.resize(data->_Targets.size() - 1);
	}

	void _RequestRender()
	{
		details::getUpdaterDataOfCurrentThread()->_NeedRender = true;
	}
	void _RequestLayout()
	{
		details::getUpdaterDataOfCurrentThread()->_NeedLayout = true;
	}
	void _RequestLogicTree()
	{
		details::getUpdaterDataOfCurrentThread()->_NeedLogicTree = true;
	}

	bool _ApplyAnimation(const Animation& animation, const UIProperty& prop, const IUIElement& target, bool force)
	{
		auto& anim = details::getUpdaterDataOfCurrentThread()->_Animations;
		auto range = anim.equal_range(target);
		// In case of 0-sized range. Insert directly as there is no animation already there.
		if (range.first != range.second)
		{
			// There are existing animations.
			auto result = std::find_if(range.first, range.second, [&](const auto& iter) { return iter.first == target; });
			if (force)
			{
				// Force remove existing animation.
				if (result != range.second)
					anim.erase(result);
			}
			else if (result != range.second)
				return false;
		}
		anim.insert({ target, { prop, animation } });
		return true;
	}
	void _CancelAnimation(const UIProperty& prop, const IUIElement& target)
	{
		auto& anim = details::getUpdaterDataOfCurrentThread()->_Animations;

		auto range = anim.equal_range(target);
		// In case of 0-sized range. Insert directly as there is no animation already there.
		if (range.first != range.second)
		{
			// There are existing animations.
			auto result = std::find_if(range.first, range.second, [&](const auto& iter) { return iter.first == target; });
			// Cancel target animation.
			if (result != range.second)
				anim.erase(result);
		}
	}
}

_L_NS_END_GRAPHICS_UI
