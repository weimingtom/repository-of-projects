#ifndef _YON_VIDEO_ITEXTURE_H_
#define _YON_VIDEO_ITEXTURE_H_

#include "IReferencable.h"
#include "path.h"
#include "dimension2d.h"
#include "IImage.h"

namespace yon{
namespace video{

	class ITexture : public virtual core::IReferencable{
	protected:
		io::SNamedPath m_namePath;
	public:
		ITexture(const io::path& p) : m_namePath(p)
		{}
		virtual ~ITexture(){}
		virtual const core::dimension2du& getSize() const = 0;
		const io::path& getPath() const { return m_namePath.getPath(); }

		virtual ENUM_COLOR_FORMAT getColorFormat() const = 0;
	};
}
}
#endif