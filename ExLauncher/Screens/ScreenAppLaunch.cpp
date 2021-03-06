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

#include "ScreenAppLaunch.h"
#include "../global.h"

using namespace std;

ScreenAppLaunch::ScreenAppLaunch()
{
	exec = vector<string>();
	popup = true;
	SetTransitionTime(0.2, 0.1);
}

ScreenAppLaunch::~ScreenAppLaunch()
{
}

void ScreenAppLaunch::SetStartRectangle(int x, int y, int width, int height)
{
	origBox = Box(y, y + height, x, x + width);
	curBox = origBox;
}

void ScreenAppLaunch::SetAppId(std::string appId)
{
	this->appId = appId;
}

void ScreenAppLaunch::SetExec(vector<string> exec)
{
	this->exec = exec;
}

bool ScreenAppLaunch::Initialize()
{
	return true;
}

void ScreenAppLaunch::HandleInput(InputState* input)
{
	//
}

void ScreenAppLaunch::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	Size dispSize = screenManager->GetDisplaySize();

	// FIXME use sigmoid curve for transition instead of linear, or possibly only sigmoid end
	// FIXME transition in with alpha as well

	Box dBox;
	dBox.top = origBox.top;
	dBox.bottom = dispSize.h - origBox.bottom;
	dBox.left = origBox.left;
	dBox.right = dispSize.w - origBox.right;

	curBox.top = dBox.top * transitionPosition;
	curBox.bottom = dispSize.h - dBox.bottom * transitionPosition;
	curBox.left = dBox.left * transitionPosition;
	curBox.right = dispSize.w - dBox.right * transitionPosition;

	if (TransitionHasFinished())
	{
		if (appId.empty())
			screenManager->GetAppManager()->SetCommandToLaunch(exec);
		else
			screenManager->GetAppManager()->SetAppToLaunch(appId, exec);

		screenManager->Exit();
	}
}

void ScreenAppLaunch::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);

	SDL_Rect r;
	r.x = curBox.left;
	r.y = curBox.top;
	r.w = curBox.right - curBox.left;
	r.h = curBox.bottom - curBox.top;

	SDL_RenderFillRect(renderer, &r);
}
