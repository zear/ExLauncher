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

#include "ScreenShutdown.h"
#include "../global.h"
#include "../Views/FramePanel.h"
#include "../Xml.h"

using namespace std;

ScreenShutdown::ScreenShutdown()
{
	SetTransitionTime(0.0, 0.0);

	contentView = NULL;
	reboot = false;
	hasDrawnLastFrame = false;
}

ScreenShutdown::ScreenShutdown(bool reboot)
{
	SetTransitionTime(0.0, 0.0);

	contentView = NULL;
	this->reboot = reboot;
	hasDrawnLastFrame = false;
}

ScreenShutdown::~ScreenShutdown()
{
	if (contentView != NULL)
		delete contentView;
	contentView = NULL;
}

bool ScreenShutdown::Initialize()
{
	contentView = new FramePanel();
	contentView->SetSize(Size(SIZE_FILL_PARENT, SIZE_FILL_PARENT));

	Label* label = new Label();
	label->SetSize(Size(SIZE_FILL_PARENT, SIZE_FILL_PARENT));
	label->SetGravity(GRAVITY_HCENTER | GRAVITY_VCENTER);
	label->SetFont("light");
	label->SetTextSize(24);

	if (reboot)
		label->SetText("Rebooting...");
	else
		label->SetText("Turning off...");

	contentView->AddChildView(label);

	contentView->InitializeAll(this);
	contentView->CalculateLayout(screenManager->GetDisplaySize());
	contentView->CalculateAbsolutePosition(Position(0, 0));

	return true;
}

void ScreenShutdown::HandleInput(InputState* input)
{
}

void ScreenShutdown::Update(bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	Screen::Update(otherScreenHasFocus, coveredByOtherScreen);

	if (hasDrawnLastFrame)
	{
		if (reboot)
			screenManager->GetAppManager()->SetCommandToLaunch({ "/sbin/reboot" });
		else
			screenManager->GetAppManager()->SetCommandToLaunch({ "/sbin/poweroff" });

		screenManager->Exit();
	}
}

void ScreenShutdown::SetReboot(bool reboot)
{
	this->reboot = reboot;
}

void ScreenShutdown::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	if (contentView != NULL)
		contentView->Draw();
        
	if (TransitionHasFinished())
		hasDrawnLastFrame = true;
}
