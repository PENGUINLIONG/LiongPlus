// File: UICommonDefs.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"

_L_NS_BEG_GRAPHICS_UI

struct Point
{
public:
	Float X, Y;
};

struct Size
{
public:
	Float Width, Height;
};

struct Rect
{
public:
	Float Left, Top, Right, Bottom;

	Float Width() const;
	Float Height() const;
	Size Size() const;
	Float X() const;
	Float Y() const;
	Point Position() const;
	Point Center() const;
};

struct Thickness
{
public:
	Float Left, Top, Right, Bottom;
};

const Float SEGMENT_PER_INCH = (Float)20.;
const Float TAU = (Float)(3.14159265358979323846 * 2);
const Float FULL_ANGLE = (Float)360.;
const Float RAD_PER_DEG = TAU / FULL_ANGLE;

_L_NS_END_GRAPHICS_UI
