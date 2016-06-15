#include "entityManager.h"

entityBase::entityBase(int ID):
m_iID(ID)
{
	setActive(true);
}

entityBase::~entityBase()
{

}

void entityBase::update(float dt)
{

}

void entityBase::render(const matrix4x4f& renderMatrix)
{

}

//=================================================================
//=================================================================
//=================================================================

entityManager::entityManager()
{
	m_pObserver=NULL;
}

entityManager::~entityManager()
{
	reset();
}

void entityManager::reset()
{
	stLinkNode<entityBase*>* entitynode=m_cEntityList.getHead();
	while(entitynode)
	{
		entityBase* entity=entitynode->getData();
		GX_DELETE(entity);
		entitynode=entitynode->getNext();
	}
	m_cEntityList.clearAll();
	m_pObserver=NULL;
}

void entityManager::init(MEntityObserver* pObserver)
{
	m_pObserver = pObserver;
}

entityBase* entityManager::appendEntity(entityBase* entity)
{
	m_cEntityList.insertTail(entity);
	return entity;
}

void entityManager::update(float dt)
{
	stLinkNode<entityBase*>* entitynode=m_cEntityList.getHead();
	while(entitynode)
	{
		entityBase* entity=entitynode->getData();
		m_pObserver->onPreUpdateEntity(entity);
		entity->update(dt);
		m_pObserver->onPostUpdateEntity(entity);

		if(!entity->isActive())
		{
			entitynode=entitynode->getNext();
			m_cEntityList.remove(entity);
			GX_DELETE(entity);
			continue;
		}
		entitynode=entitynode->getNext();
	}
}

void entityManager::render(const matrix4x4f& renderMatrix)
{
	stLinkNode<entityBase*>* entitynode=m_cEntityList.getHead();
	while(entitynode)
	{
		entityBase* entity=entitynode->getData();
		entity->render(renderMatrix);
		entitynode=entitynode->getNext();
	}
}
