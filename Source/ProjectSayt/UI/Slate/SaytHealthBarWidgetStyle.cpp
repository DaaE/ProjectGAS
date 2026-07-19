// Copyright (c) 2026 Sungmin JI. All Rights Reserved.


#include "SaytHealthBarWidgetStyle.h"

const FName FSaytHealthBarStyle::TypeName(TEXT("FSaytHealthBarStyle"));

FSaytHealthBarStyle::FSaytHealthBarStyle()
{
}

FSaytHealthBarStyle::~FSaytHealthBarStyle()
{
}

const FSaytHealthBarStyle& FSaytHealthBarStyle::GetDefault()
{
	static FSaytHealthBarStyle Default;
	return Default;
}

void FSaytHealthBarStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
	OutBrushes.Add(&BackgroundBrush);
	OutBrushes.Add(&FillBrush);
	OutBrushes.Add(&GhostBrush);
	OutBrushes.Add(&FrameBrush);
	OutBrushes.Add(&HighlightBrush);
}
