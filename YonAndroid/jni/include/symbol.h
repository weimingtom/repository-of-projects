#ifndef _YON_GUI_SYMBOL_H_
#define _YON_GUI_SYMBOL_H_

#include "yonTypes.h"

namespace yon{
namespace gui{

	class symbol{
	public:

		enum ENUM_TAG{
			UNKNOWN = 0,
			HTML,
			META,
			STYLE,
			HEAD,
			TITLE,
			BODY,
			FRAME,
			DIV,
			INPUT,
			TABLE,
			THEAD,
			TBODY,
			TR,
			TD,
			IMG,
			PRE,
			P,
			A,
			BR,
			TAG_COUNT
		};

		enum ENUM_ATTRIBUTE{
			ID = TAG_COUNT,
			NAME,
			CLASS,
			TYPE,
			VALUE,
			READONLY,
			DISABLED,
			CHECKED,
			TARGET,
			SRC,
			ALT,
			COLS,
			ROWS,
			MAX_LENGTH,
			ATTRIBUTE_COUNT
		};

		enum ENUM_EVENT{
			MOUSEDOWN,
			MOUSESCROLL,
			MOUSEOVER,
			MOUSEOUT,
			FOCUS,
			BLUR,
			KEYDOWN,
			MOUSEUP,
			CLICK,
			DRAGSTART,
			LOAD,
			UNLOAD,
			KEYUP,
			TEXTINPUT,
			MOUSEMOVE,
			DRAGMOVE,
			DBLCLICK,
			DRAGDROP,
			DRAGOUT,
			DRAGEND,
			DRAGOVER,
			RESIZE,
		};

		enum ENUM_INPUTTYPE{
			BUTTON = ATTRIBUTE_COUNT,
			TEXT,
			PASSWORD,
			RADIO,
			CHECKBOX,
			FILE,
			INPUTTYPE_COUNT
		};
		
		
		enum ENUM_PROPERTY{
			MARGIN_TOP = INPUTTYPE_COUNT,
			MARGIN_RIGHT,
			MARGIN_BOTTOM,
			MARGIN_LEFT,
			MARGIN,
			PADDING_TOP,
			PADDING_RIGHT,
			PADDING_BOTTOM,
			PADDING_LEFT,
			PADDING,
			BORDER_TOP_WIDTH,
			BORDER_RIGHT_WIDTH,
			BORDER_BOTTOM_WIDTH,
			BORDER_LEFT_WIDTH,
			BORDER_WIDTH,
			BORDER_TOP_COLOR,
			BORDER_RIGHT_COLOR,
			BORDER_BOTTOM_COLOR,
			BORDER_LEFT_COLOR,
			BORDER_COLOR,
			BORDER_TOP,
			BORDER_RIGHT,
			BORDER_BOTTOM,
			BORDER_LEFT,
			DISPLAY,
			POSITION,
			TOP,
			RIGHT,
			BOTTOM,
			LEFT,
			FLOAT,
			CLEAR,
			Z_INDEX,
			WIDTH,
			MIN_WIDTH,
			MAX_WIDTH,
			HEIGHT,
			MIN_HEIGHT,
			MAX_HEIGHT,
			LINE_HEIGHT,
			VERTICAL_ALIGN,
			OVERFLOW_X,
			OVERFLOW_Y,
			CLIP,
			VISIBILITY,
			BACKGROUND_COLOR,
			BACKGROUND,
			COLOR,
			FONT_FAMILY,
			FONT_CHARSET,
			FONT_STYLE,
			FONT_WEIGHT,
			FONT_SIZE,
			FONT,
			TEXT_ALIGN,
			TEXT_DECORATION,
			TEXT_TRANSFORM,
			WHITE_SPACE,
			CURSOR,
			DRAG,
			TAB_INDEX,
			SCROLLBAR_MARGIN,

			PROPERTY_COUNT
		};

		YON_API static const c8* const NAMES[PROPERTY_COUNT];
	};
}
}
#endif