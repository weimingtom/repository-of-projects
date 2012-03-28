#ifndef _YON_VIDEO_SMATERIALLAYER_H_
#define _YON_VIDEO_SMATERIALLAYER_H_

#include "ITexture.h"

namespace yon{
namespace video{

	enum ENUM_WRAP_MODE
	{
		ENUM_WRAP_MODE_REPEAT = 0,
		ENUM_WRAP_MODE_CLAMP_TO_EDGE,
		ENUM_WRAP_MODE_COUNT
	};
	struct SMaterialLayer{
		ITexture* texture;
		bool bilinearFilter;
		ENUM_WRAP_MODE wrapU,wrapV;

		SMaterialLayer()
			:texture(NULL),
			wrapU(ENUM_WRAP_MODE_REPEAT),
			wrapV(ENUM_WRAP_MODE_REPEAT),
			bilinearFilter(true)
		{}
		SMaterialLayer(const SMaterialLayer& other)
		{
			*this = other;
		}
		SMaterialLayer& operator=(const SMaterialLayer& other)
		{
			// Check for self-assignment!
			if (this == &other)
				return *this;

			texture = other.texture;
			wrapU = other.wrapU;
			wrapV = other.wrapV;
			bilinearFilter = other.bilinearFilter;
			return *this;
		}
		inline bool operator!=(const SMaterialLayer& other) const
		{
			bool different =
				texture != other.texture ||
				wrapU != other.wrapU ||
				wrapV != other.wrapV ||
				bilinearFilter != other.bilinearFilter;
			if (different)
				return true;
			return false;
		}

		inline bool operator==(const SMaterialLayer& other) const
		{ return !(other!=*this); }
	};
}
}
#endif