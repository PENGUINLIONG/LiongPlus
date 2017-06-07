// File: Grid.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <vector>
#include "_"
#include "../Panel.hpp"
#include "../UIFactory.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

enum class GridMetricUnit
{
	Auto, Pixel, Percent
};

struct GridMetric
{
public:
	Float Value = 0.;
	GridMetricUnit Unit = GridMetricUnit::Auto;
};

class Grid :
	public Panel,
	public DependencyObject<Grid>
{
private:
	std::vector<Float> _CachedColOffset, _CachedRowOffset;

public:
	static const UIProperty
		ColumnDefinitionsProperty,
		RowDefinitionsProperty,
		ColumnProperty,
		RowProperty;

	std::vector<GridMetric> GetColumnDefinitions() const;
	void SetColumnDefinitions(const std::vector<GridMetric>& defs);
	std::vector<GridMetric> GetRowDefinitions() const;
	void SetRowDefinitions(const std::vector<GridMetric>& defs);
	
	unsigned int GetColumn(const IUIElement& uie) const;
	void SetColumn(IUIElement& uie, unsigned int col);
	unsigned int GetRow(const IUIElement& uie) const;
	void SetRow(IUIElement& uie, unsigned int row);

	void DoRender(const ICanvas& canvas) override;
	Size Layout(const Size& containerSize) override;
	void LogicTree() override;
};

_L_NS_END_GRAPHICS_UI_CONTROLS

_L_NS_BEG_GRAPHICS_UI

template<>
struct UIFactory<Controls::Grid>
{
	static std::shared_ptr<Controls::Grid> Create();
};

_L_NS_END_GRAPHICS_UI
