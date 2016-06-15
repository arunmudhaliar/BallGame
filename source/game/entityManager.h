#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "../engine/core/matrix4x4f.h"
#include "../engine/util/bxLinkedList.h"



class entityBase
{
public:
	entityBase(int ID);
	virtual ~entityBase();

	virtual void update(float dt);
	virtual void render(const matrix4x4f& renderMatrix);

	bool isActive()				{	return m_bActive;	}
	void setActive(bool flag)	{	m_bActive=flag;		}
	int getID()					{	return m_iID;		}

private:
	bool m_bActive;
	int m_iID;
};

class MEntityObserver
{
public:
	virtual void onPreUpdateEntity(entityBase* entity)=0;
	virtual void onPostUpdateEntity(entityBase* entity)=0;
};

class entityManager
{
public:
	entityManager();
	~entityManager();

	void reset();

	void init(MEntityObserver* pObserver);
	void update(float dt);
	void render(const matrix4x4f& renderMatrix);
	entityBase* appendEntity(entityBase* entity);

	bxLinkedList<entityBase*>* getEntityList()	{	return &m_cEntityList;	}
private:
	bxLinkedList<entityBase*>	m_cEntityList;
	MEntityObserver* m_pObserver;
};

#endif