/*
Copyright 2016 Andreas Bjerkeholt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _SCROLL_VIEW_H_
#define _SCROLL_VIEW_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../ViewSystem/CollectionView.h"
#include "../structures.h"
#include "../ScreenSystem/InputState.h"
#include <vector>

/*********************************************/

class ScrollView : public CollectionView
{
private:
	int scrollOffset;
	int scrollToTargetOffset;
	Orientation orientation;
	int leadingContentInset;
	int trailingContentInset;

	void DrawChildren(Position offset, Rectangle viewBounds);
protected:
	void SetOrientation(Orientation orientation);
	void SetContentInset(int leading, int trailing);
public:
	ScrollView();
	virtual void OnUpdate();

	void ScrollTo(int offset);
	void ScrollTo(View* child);
};

/*********************************************/

#endif
