#ifndef INTTREE_H
#define INTTREE_H

#include "Log.h"

//#include <vector>
#include <string>


namespace Utility
{


	template<typename TheStruct>
	struct IntTreeNode
	{
		IntTreeNode()
		{
			storedInfoM = NULL;
			memset(subNodePtrM, 0, sizeof(subNodePtrM));
		}
		~IntTreeNode()
		{
			if(storedInfoM != NULL)
				delete storedInfoM;
			for(int i = 0; i < 10; i++ )
			{
				if( NULL != subNodePtrM[i] )
					delete subNodePtrM[i];
			}
		}

		TheStruct* storedInfoM;
		IntTreeNode<TheStruct>* subNodePtrM[10];
	};

	//---------------------------------------------------
	template<typename TheStruct>
	class IntTree
	{
	public:
		IntTree();
		~IntTree();

		void build(const char* theKey, int theLength, TheStruct* theStructInfo);
		bool find(const char* theKey, int theLength, TheStruct& theReturnInfo);

	private:
		IntTreeNode<TheStruct> rootNodeM;
	};

	//--------------------------------------------------------------------------

	template<typename TheStruct>
	IntTree<TheStruct>::IntTree()
	{
	}

	//-------------------------------------------------------------------------

	template<typename TheStruct>
	IntTree<TheStruct>::~IntTree()
	{
	}

	//--------------------------------------------------------------------------
	//Set new node into the tree.

	template<typename TheStruct>
	void IntTree<TheStruct>::build(const char* theKey, int theLength, TheStruct* theStructInfo)
	{
		int i = 0;
		int intKey = 0;

		IntTreeNode<TheStruct> *currentNodePtr = &rootNodeM;

		if( theKey == NULL )
		{
			LOG_ERROR("the key is null.ignore");
			return;
		}
		for(i = 0;  i < theLength; i++ )
		{
			//The character is converted to an integer
			intKey = theKey[i] - '0';
			if( intKey < 0 || intKey > 9 )
			{
				LOG_ERROR("the key is not number:" << theKey << ". ignore");
				return;
			}

			if( currentNodePtr->subNodePtrM[intKey] == NULL )
			{
				//if sub node is null, add a new null node
				currentNodePtr->subNodePtrM[intKey] = new IntTreeNode<TheStruct>;
			}

			currentNodePtr = currentNodePtr->subNodePtrM[intKey];
		}

		currentNodePtr->storedInfoM = theStructInfo;
	}

	//---------------------------------------------------------------------------
	template<typename TheStruct>
	bool IntTree<TheStruct>::find(const char* theKey, int theLength, TheStruct& theReturnInfo)
	{
		int i = 0;
		int intKey;
		IntTreeNode<TheStruct> *currentNodePtr = &rootNodeM;

		TheStruct* tmpStruct = NULL;

		if( theKey == NULL )
		{
			LOG_ERROR("the key is null.ignore");
			return false;
		}

		for( i = 0; i < theLength; i++ )
		{
			intKey = theKey[i] - '0';		//The character is converted to an integer

			if( intKey < 0 || intKey > 9 )
			{
				LOG_ERROR("the key is not number:" << theKey << ". ignore");
				break;
			}

			if( currentNodePtr->subNodePtrM[intKey] == NULL )
			{
				break;
			}

			currentNodePtr = currentNodePtr->subNodePtrM[intKey];

			if( currentNodePtr->storedInfoM != NULL )
			{
				//covering the previous phone information
				tmpStruct = currentNodePtr->storedInfoM;
			}
		}

		//LOG_TRACE("finding in the imeiseg tree");

		if(tmpStruct == NULL)
        {
            LOG_TRACE("key: " << theKey << "was not found");

			return false;
        }

		theReturnInfo = *tmpStruct;
        LOG_TRACE("found, thekey is: " << theKey);
		return true;
	}

}

#endif
