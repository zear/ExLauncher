#ifndef _SELECTOR_H_
#define _SELECTOR_H_

/*********************************************/

#include "../ViewSystem/View.h"
#include "../structures.h"
#include "FramePanel.h"

/*********************************************/

class Selector : public FramePanel
{
public:
	Selector();
	~Selector();
	View* Copy();

	bool SetProperty(std::string name, std::string value);
};

/*********************************************/

#endif