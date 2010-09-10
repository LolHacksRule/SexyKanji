// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
#ifndef __TRIVERTEX_H__
#define __TRIVERTEX_H__

#ifndef DWORD
#define DWORD unsigned long 
#endif

namespace Sexy
{
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class SexyTriVertex
	{
	public:
		float x,y,u,v;
		DWORD color; //ARGB (0 = use color specified in function call)

	public:
		SexyTriVertex() { color = 0; }
		SexyTriVertex(float theX, float theY) : x(theX), y(theY) { color = 0; }
		SexyTriVertex(float theX, float theY, float theU, float theV) : x(theX), y(theY), u(theU), v(theV) { color = 0; }
		SexyTriVertex(float theX, float theY, float theU, float theV, DWORD theColor) : x(theX), y(theY), u(theU), v(theV), color(theColor) { }
	};

} // namespace Sexy


#endif