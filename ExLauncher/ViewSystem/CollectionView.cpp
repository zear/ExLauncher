#include "CollectionView.h"
#include <algorithm>

using namespace std;

CollectionView::CollectionView()
{
	contentSize.w = 0;
	contentSize.h = 0;

	selectedPosition = Position(0, 0);
	selectedItem = NULL;
}

CollectionView::~CollectionView()
{
}

void CollectionView::HandleInput(InputState* input)
{
	if (input->GameKeyJustPressed(GAMEKEY_UP))
		SelectNext(DirectionUp);
	else if (input->GameKeyJustPressed(GAMEKEY_DOWN))
		SelectNext(DirectionDown);
	else if (input->GameKeyJustPressed(GAMEKEY_LEFT))
		SelectNext(DirectionLeft);
	else if (input->GameKeyJustPressed(GAMEKEY_RIGHT))
		SelectNext(DirectionRight);
}

Position CollectionView::GetSelectedPosition()
{
	return selectedPosition;
}