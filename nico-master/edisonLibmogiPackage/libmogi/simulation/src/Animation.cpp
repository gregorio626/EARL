/******************************************************************************
 *                                                                            *
 *             Copyright (C) 2016 Mogi, LLC - All Rights Reserved             *
 *                            Author: Matt Bunting                            *
 *                                                                            *
 *   Proprietary and confidential.                                            *
 *                                                                            *
 *   Unauthorized copying of this file via any medium is strictly prohibited  *
 *   without the explicit permission of Mogi, LLC.                            *
 *                                                                            *
 *   See license in root directory for terms.                                 *
 *   http://www.binpress.com/license/view/l/0088eb4b29b2fcff36e42134b0949f93  *
 *                                                                            *
 *****************************************************************************/

#include "animation.h"

#ifdef _cplusplus
extern "C" {
#endif

using namespace Mogi;
using namespace Simulation;

Animation::Animation() {
	init();
}

Animation::~Animation() {
	for (std::vector<Channel*>::iterator it = channels.begin();
			it != channels.end(); it++) {
		delete *it;
	}
}

void Animation::init() {
	loopTime = 0;
}

void Animation::matchChannelsToNodes(Math::Node* rootNode) {
	for (std::vector<Channel*>::iterator it = channels.begin();
			it != channels.end(); it++) {
		(*it)->findNode(rootNode);
	}
}

void Animation::play() {
	timer.initialize();
}

// This should be called upon every render cycle:
void Animation::update() {
	timer.update();
	loopTime += timer.dTime();
	if (loopTime > duration) {
		loopTime -= duration;
	}

	for (int i = 0; i < channels.size(); i++) {
		channels[i]->update(loopTime);
	}
}

#ifdef _cplusplus
}
#endif
