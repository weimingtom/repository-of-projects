#ifndef _YON_EVENT_SKEYMAP_H_
#define _YON_EVENT_SKEYMAP_H_

#include "keycodes.h"

namespace yon{
namespace event{

	enum ENUM_KEY_ACTION{
		ENUM_KEY_ACTION_MOVE_FORWARD=0,
		ENUM_KEY_ACTION_MOVE_BACKWARD,
		ENUM_KEY_ACTION_COUNT
	};

	struct SKeyMap{
		ENUM_KEY_ACTION action;
		ENUM_KEY keycode;
	};
}
}
#endif