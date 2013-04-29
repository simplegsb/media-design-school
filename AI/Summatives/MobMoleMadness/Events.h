//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// Copyright (c) 2012 Media Design School
//
// Author: Gary Buyn
//
// SVN Info
// $Author: MEDIADESIGN\gary.buyn $
// $Date: 2013-01-23 09:40:30 +1300 (Wed, 23 Jan 2013) $
// $Rev: 1108 $
//
#ifndef EVENTS_H_
#define EVENTS_H_

#include "Common.h"

// Must be included after Common.h or errors will ensue...
#include <string>

const std::string ACORN_FOUND_EVENT = "Acorn Found";

const std::string ANY_EVENT = "Any";

const std::string DESTINATION_REACHED_EVENT = "Destination Reached";

const std::string DISTURBANCE_EVENT = "Disturbance Detected";

const std::string MOLE_RESTED_EVENT = "Mole Rested";

const std::string MOLE_TIRED_EVENT = "Mole Tired";

const std::string SLEEP_EVENT = "Sleep/Wake Up";

const std::string TARGET_LOST_EVENT = "Target Lost";

#endif /* EVENTS_H_ */
