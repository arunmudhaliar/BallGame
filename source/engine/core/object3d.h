#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "types.h"
#include "transformf.h"
#include "../util/gxFile.h"
#include "../util/bxLinkedList.h"
#include "../util/gxDebug.h"

#include "../renderer/renderer.h"


class objectBase : public transformf
{
private:
    objectBase():
    transformf()
    {
    }
public:
	enum EOBJECTFLAGS
	{
		OBJF_USE_TM	= (1<<0),		//if it is set the object is dynamic
		OBJF_VISIBLE = (1<<1)
	};

	objectBase(int ID):
    transformf()
	{
		m_eBaseFlags=OBJF_VISIBLE;
		m_iID=ID;
		memset(m_cszName, 0, 128);
		m_pParentPtr=NULL;
		m_bVisited=false;
	}

	virtual ~objectBase()
	{
		reset();
	}

	virtual void reset()
	{
		m_eBaseFlags=OBJF_VISIBLE;
		stLinkNode<objectBase*>* node=m_cChildList.getHead();
		while(node)
		{
			objectBase* child=node->getData();
			GX_DELETE(child);
			node=node->getNext();
		}
		m_cChildList.clearAll();
		m_pParentPtr=NULL;
		m_bVisited=false;
        m_cRenderMatrix.identity();
	}

	const char* getName()			{	return m_cszName;	}
	void setName(const char* name)	{	strncpy_s(m_cszName, name, sizeof(m_cszName));	}

	virtual bool read(const gxFile& file)
	{
		return true;
	}

	virtual void update(float dt)
	{
	}

	virtual void render(const matrix4x4f* parentTM)//=0;
	{
		if(isBaseFlag(OBJF_USE_TM))
		{
            m_cRenderMatrix = (*parentTM * (matrix4x4f)*this);
		}
        else
        {
            m_cRenderMatrix=*parentTM;
        }

		//render child nodes
		stLinkNode<objectBase*>* node=m_cChildList.getHead();
		while(node)
		{
			objectBase* obj=node->getData();
			obj->render(&m_cRenderMatrix);
			node=node->getNext();
		}
	}

	objectBase* findObject(const char* name)
	{
		if(!name) return NULL;

		if(strcmp(getName(), name)==0)
			return this;

		//find child nodes
		stLinkNode<objectBase*>* node=m_cChildList.getHead();
		while(node)
		{
			objectBase* obj=node->getData();
			objectBase* retobj=obj->findObject(name);
			if(retobj)
				return retobj;
			node=node->getNext();
		}

		return NULL;
	}

	void appendChild(objectBase* child)
	{
		child->setParent(this);
		m_cChildList.insertTail(child);
	}

	void	resetAllBaseFlags()					{	m_eBaseFlags=0;						}
	void	setBaseFlag(EOBJECTFLAGS eFlags)	{	m_eBaseFlags=m_eBaseFlags|eFlags;	};
	void	reSetBaseFlag(EOBJECTFLAGS eFlags)	{	m_eBaseFlags=m_eBaseFlags&~eFlags;	};
	bool	isBaseFlag(EOBJECTFLAGS eFlags)		{	return (m_eBaseFlags&eFlags)?true:false;	};
	int		getBaseFlag()						{	return m_eBaseFlags;				}

	int		getBaseID()		{	return m_iID;	}

	void	setParent(objectBase* parent)		{	m_pParentPtr=parent;	}
	objectBase* getParent()				{	return m_pParentPtr;	}


	void setVisited(bool flag)	{	m_bVisited=flag;	}
	bool isVisited()			{	return m_bVisited;	}

    virtual void transformationChangedf()
    {
        stLinkNode<objectBase*>* node=m_cChildList.getHead();
		while(node)
		{
			objectBase* obj=node->getData();
			obj->transformationChangedf();
			node=node->getNext();
		}
    }
    
protected:
	int m_iID;
	char m_cszName[128];
	unsigned int m_eBaseFlags;
	bxLinkedList<objectBase*> m_cChildList;
	objectBase* m_pParentPtr;	//must not delete this pointer
	bool	m_bVisited;
    matrix4x4f m_cRenderMatrix;

public:
	static rendererBase* getRenderer()	{	return g_pRendererPtr;	}
	static rendererBase* g_pRendererPtr;
};


#endif