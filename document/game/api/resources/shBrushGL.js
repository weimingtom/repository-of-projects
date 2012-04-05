/**
 * SyntaxHighlighter
 * http://alexgorbatchev.com/SyntaxHighlighter
 *
 * SyntaxHighlighter is donationware. If you are using it, please donate.
 * http://alexgorbatchev.com/SyntaxHighlighter/donate.html
 *
 * @version
 * 3.0.83 (July 02 2010)
 * 
 * @copyright
 * Copyright (C) 2004-2010 Alex Gorbatchev.
 *
 * @license
 * Dual licensed under the MIT and GPL licenses.
 */
;(function()
{
	// CommonJS
	typeof(require) != 'undefined' ? SyntaxHighlighter = require('shCore').SyntaxHighlighter : null;

	function Brush()
	{
		var keywords =
			'int ' +
			'GLuint '
			;
		var functions =
			'glActiveTexture ' +
			'glCompressedTexImage2D ' +
			'glCompressedTexSubImage2D ' +
			'glDisable ' +
			'glEnable ' +
			'glLoadMatrix ' +
			'glMatrixMode ' +
			'glPixelStorei ' +
			'glReadPixels ' +
			'glScissor ' +
			'glTexImage2D ' +
			'glTexSubImage2D '
			;
		var constants =	
			'GL_MAX_TEXTURE_UNITS ' +
			'GL_PACK_ALIGNMENT ' +
			'GL_SCISSOR_TEST ' +
			'GL_TEXTURE0 ' +
			'GL_TEXTUREi ' +
			'GL_UNPACK_ALIGNMENT '
			;

		var r = SyntaxHighlighter.regexLib;
		
		this.regexList = [
			{ regex: r.multiLineDoubleQuotedString,					css: 'string' },			// double quoted strings
			{ regex: r.multiLineSingleQuotedString,					css: 'string' },			// single quoted strings
			{ regex: r.singleLineCComments,							css: 'comments' },			// one line comments
			{ regex: r.multiLineCComments,							css: 'comments' },			// multiline comments
			{ regex: /\s*#.*/gm,									css: 'preprocessor' },		// preprocessor tags like #region and #endregion
			{ regex: /\u2605.*/gm,									css: 'green' },				// preprocessor tags like #region and #endregion
			{ regex: /\u3010.*\u3011/gm,							css: 'h3' },				// h3
			{ regex: new RegExp(this.getKeywords(keywords), 'gm'),	css: 'keyword' },			// keywords
			{ regex: new RegExp(this.getKeywords(constants), 'gm'),	css: 'constants' },			// constants
			{ regex: new RegExp(this.getKeywords(functions), 'gm'),	css: 'functions' }			// functions
			];
	
		this.forHtmlScript(r.scriptScriptTags);
	};

	Brush.prototype	= new SyntaxHighlighter.Highlighter();
	Brush.aliases	= ['gl', 'ogl', 'opengl'];

	SyntaxHighlighter.brushes.JScript = Brush;

	// CommonJS
	typeof(exports) != 'undefined' ? exports.Brush = Brush : null;
})();
