// Portions Copyright (c) 2005-2010 PopCap Games. All rights reserved.
// This file is part of the "Sexy Kanji Game Engine".
// For conditions of distribution and use, see copyright notice in SexyAppBase.h
/********************************************************************
created:	2010/04/24
created:	24:4:2010   8:20
filename: 	SexyKanji\SharedItem.h
author:		James Poag, Andrei Lazutin

purpose:	Provide the reference counting to be used
in the FontManager, ImageManager, other *Manager
*********************************************************************/
#pragma once

namespace Sexy
{

    /*!***********************************
    // \class   	SharedItem
    //
    // \brief		Pairs a reference count with a <T>*. Do not use directly.
    // \details 	<b>Description:</b> To keep a 'global' reference count for a <T>
    //				object, we created this class.  This get's 'newed' up on the program 
    //				heap so that the reference count will persist as a 'global' object. 
    //				DO NO USE DIRECTLY, use SharedItemRef instead.
    //************************************/
    template <class T>
    class SharedItem
    {
    public:
        T* mItem;
        int mRefCount;		

        SharedItem()
        {
            mItem = NULL;
            mRefCount = 0;
        }

        virtual ~SharedItem(){};
    };

    /*!***********************************
    // \class   	SharedItemRef
    //
    // \brief		Manages the reference count on a <T>.  Also masquerades as a <T>.
    // \details 	<b>Description:</b> Manages the reference count on a <T>.  Most of the reference
    //				count management is done when the object goes out of scope or gets a new assignment.
    //************************************/
    template <class T>
    class SharedItemRef
    {
    public:
        SharedItem<T>* mSharedItem;
        T* mUnSharedItem;
        bool mOwnsUnshared;

    public:
        SharedItemRef()
        {
            mSharedItem = NULL;
            mUnSharedItem = NULL;
            mOwnsUnshared = false;
        }

        SharedItemRef(const SharedItemRef& theSharedItemRef)
        {
            mSharedItem = theSharedItemRef.mSharedItem;
            if (mSharedItem != NULL)
                mSharedItem->mRefCount++;
            mUnSharedItem = theSharedItemRef.mUnSharedItem;	
            mOwnsUnshared = false;
        }
        
        SharedItemRef(SharedItem<T>* theSharedItem)
        {
            mSharedItem = theSharedItem;
            if (theSharedItem != NULL)
                mSharedItem->mRefCount++;

            mUnSharedItem = NULL;
            mOwnsUnshared = false;
        }

        virtual ~SharedItemRef()
        {
            Release();
        }

        void Release()
        {
            if (mOwnsUnshared)
                delete mUnSharedItem;
            mUnSharedItem = NULL;
            if (mSharedItem != NULL)
            {
                mSharedItem->mRefCount--;
            }
            mSharedItem = NULL;
        }

        SharedItemRef& operator=(const SharedItemRef& theSharedItemRef)
        {
            Release();
            mSharedItem = theSharedItemRef.mSharedItem;
            if (mSharedItem != NULL)
                mSharedItem->mRefCount++;
            return *this;
        }

        SharedItemRef& operator=(SharedItem<T>* theSharedItem)
        {
            Release();
            mSharedItem = theSharedItem;
            mSharedItem->mRefCount++;
            return *this;
        }

        SharedItemRef& operator=(T* theUnSharedItem)
        {
            Release();
            mUnSharedItem = theUnSharedItem;	
            return *this;
        }

        T* operator->()
        {
            return (T*) *this;
        }

        operator T*()
        {
            if (mUnSharedItem != NULL)
                return mUnSharedItem;
            else if (mSharedItem != NULL)
                return mSharedItem->mItem;
            else
                return NULL;
        }
    };
}