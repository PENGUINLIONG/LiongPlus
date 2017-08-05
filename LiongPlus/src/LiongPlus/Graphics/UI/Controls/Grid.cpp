// File: Grid.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Graphics/UI/Controls/Grid.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

using namespace std;

namespace details
{
	bool defsValidator(const vector<GridMetric>& val)
	{ return val.size() > 0; }
	void* defsAllocator()
	{ return new vector<GridMetric>{ { 1, GridMetricUnit::Auto } }; }
	void defsDeleter(const void* ptr)
	{ delete (vector<GridMetric>*)ptr; }
}

const UIProperty
Grid::ColumnDefinitionsProperty =
	UIProperty::Make<vector<GridMetric>>("ColumnDefinitions",
		UIActionTrigger::Layout_Render,
		&details::defsValidator,
		&details::defsAllocator,
		&details::defsDeleter).Register<Grid>();
const UIProperty
Grid::RowDefinitionsProperty =
	UIProperty::Make<vector<GridMetric>>("RowDefinitions",
		UIActionTrigger::Layout_Render,
		&details::defsValidator,
		&details::defsAllocator,
		&details::defsDeleter).Register<Grid>();

const UIProperty
Grid::ColumnProperty = UIProperty::MakeDefault<unsigned int>("Grid.Column",
	UIActionTrigger::Layout_Render).RegisterAttached<Grid>();
const UIProperty
Grid::RowProperty = UIProperty::MakeDefault<unsigned int>("Grid.Row",
	UIActionTrigger::Layout_Render).RegisterAttached<Grid>();

namespace details
{
	Float translateGridMetricToPixel(const GridMetric& metric, Float max)
	{
		if (GridMetricUnit::Pixel == metric.Unit)
			return metric.Value;
		else if (GridMetricUnit::Percent == metric.Unit)
			return metric.Value / 100. * max;
		else // Auto.
			return -1.;
	}

	/*
	 * Returns:
	 *   A bool vector indicating the need for automatic allocation of each section and size remains.
	 */
	struct _summarizeDefs_rv_t { vector<size_t> AutoAllocation; Float SizeRemain; }
	summarizeDefs(const vector<GridMetric>& defs, Float maxSize, vector<Float>& cache)
	{
		size_t defCount = defs.size();
		Float sizeRemain = maxSize;
		vector<size_t> autoAlloc(defCount);

		for (size_t i = 0; i < defCount; ++i)
		{
			Float px;
			const GridMetric& colDef = defs[i];
			if ((px = translateGridMetricToPixel(colDef, maxSize)) < 0)
				// Automatically allocate space.
				autoAlloc[i] = true;
			else
			{
				// Pixel or Percent.
				sizeRemain -= px;
				cache[i] = px;
			}
		}
		return { autoAlloc, sizeRemain };
	}

	void accumulateThroughVector(vector<Float>& v)
	{
		Float accu = 0;
		size_t size = v.size();
		auto data = v.data();
		for (size_t i = 0; i < size; ++i)
		{
			auto f = data[i];
			data[i] = accu;
			accu += f;
		}
	}
}


vector<GridMetric> Grid::GetColumnDefinitions() const
{
	return GetProperty<vector<GridMetric>>(ColumnDefinitionsProperty);
}
void Grid::SetColumnDefinitions(const std::vector<GridMetric>& defs)
{
	SetProperty(ColumnDefinitionsProperty, defs);
}
vector<GridMetric> Grid::GetRowDefinitions() const
{
	return GetProperty<vector<GridMetric>>(RowDefinitionsProperty);
}
void Grid::SetRowDefinitions(const std::vector<GridMetric>& defs)
{
	SetProperty(RowDefinitionsProperty, defs);
}


unsigned int Grid::GetColumn(const IUIElement& uie) const
{
	return uie->GetProperty<unsigned int>(ColumnProperty);
}
void Grid::SetColumn(const IUIElement& uie, unsigned int col)
{
	uie->SetProperty(ColumnProperty, col);
}
unsigned int Grid::GetRow(const IUIElement& uie) const
{
	return uie->GetProperty<unsigned int>(RowProperty);
}
void Grid::SetRow(const IUIElement& uie, unsigned int row)
{
	uie->SetProperty(RowProperty, row);
}

void Grid::DoRender(const ICanvas& canvas)
{
	Control::DoRender(canvas);
	for (const auto& child : GetChildren())
	{
		auto xOffset = _CachedColOffset[GetColumn(child)];
		auto yOffset = _CachedRowOffset[GetRow(child)];
		canvas->PushTransform(MatrixTransform::Translate(xOffset, yOffset));
		child->Render(canvas);
		canvas->PopTransform();
	}
}

Size Grid::Layout(const Size& containerSize)
{
	using namespace details;
	auto rv = Panel::Layout(containerSize);
	const auto& colDefs = GetColumnDefinitions();
	const auto& rowDefs = GetRowDefinitions();
	auto size = GetSize();
	auto padding = GetPadding();

	_CachedColOffset.resize(colDefs.size());
	_CachedRowOffset.resize(rowDefs.size());

	auto colInfo = summarizeDefs(colDefs, size.Width - padding.Left - padding.Right, _CachedColOffset);
	auto rowInfo = summarizeDefs(rowDefs, size.Height - padding.Top - padding.Bottom, _CachedRowOffset);
	auto colAutoAlloc = colInfo.AutoAllocation.data();
	auto rowAutoAlloc = rowInfo.AutoAllocation.data();

	multimap<unsigned int, const IUIElement&> childrenByCol, childrenByRow;

	for (const auto& child : GetChildren())
	{
		auto col = GetColumn(child);
		childrenByCol.insert({ col, child });
		auto row = GetRow(child);
		childrenByRow.insert({ row, child });

		// Auto allocated space.
		if (colAutoAlloc[col] || rowAutoAlloc[row])
		{
			Float colPx = colAutoAlloc[col] ?
				colInfo.SizeRemain :
				translateGridMetricToPixel(colDefs[col], size.Width - padding.Left - padding.Right);
			Float rowPx = rowAutoAlloc[row] ?
				rowInfo.SizeRemain :
				translateGridMetricToPixel(rowDefs[row], size.Height - padding.Top - padding.Bottom);
			
			Size sizeAfter = child->Layout({ colPx, rowPx });
			if (_CachedColOffset[col] < sizeAfter.Width)
			{
				//colInfo.SizeRemain -= (sizeAfter.Width - _CachedColOffset[col]);
				_CachedColOffset[col] = sizeAfter.Width;
			}
			if (_CachedRowOffset[row] < sizeAfter.Height)
			{
				//rowInfo.SizeRemain -= (sizeAfter.Height - _CachedRowOffset[row]);
				_CachedRowOffset[row] = sizeAfter.Height;
			}
		}
		// Fixed space.
		else
		{
			Float colPx = translateGridMetricToPixel(colDefs[col], size.Width - padding.Left - padding.Right);
			Float rowPx = translateGridMetricToPixel(rowDefs[row], size.Height - padding.Top - padding.Bottom);
			child->Layout({ colPx, rowPx });
		}
	}
	for (const auto& child : GetChildren())
	{
		auto col = GetColumn(child);
		auto row = GetRow(child);
		if (colAutoAlloc[col] || rowAutoAlloc[row])
		{
			// Set up size to fit the real boundary.
			child->Layout({_CachedColOffset[col], _CachedRowOffset[row]});
		}
	}
	accumulateThroughVector(_CachedColOffset);
	accumulateThroughVector(_CachedRowOffset);

	return rv;
}

void Grid::LogicTree()
{
	for (const auto& child : GetChildren())
		child->AttachProperty<Grid>();
}

_L_NS_END_GRAPHICS_UI_CONTROLS

_L_NS_BEG_GRAPHICS_UI

std::shared_ptr<Controls::Grid> UIFactory<Controls::Grid>::Create()
{
	return std::make_shared<Controls::Grid>();
}

_L_NS_END_GRAPHICS_UI
