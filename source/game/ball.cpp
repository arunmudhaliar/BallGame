#include "ball.h"

ball::ball() :
gxMesh()
{
	m_bTurnRed = false;
	m_fTurnRedColor = 0.0f;
}

ball::~ball()
{
}

void ball::load(const std::string& path, const std::string& texturePath, CTextureManager& textureManager)
{
	gxFile file;
	if (file.OpenFile(path.c_str()))
	{
		readScriptObject(file);

		file.CloseFile();
	}

	//identity();
	float ball_radius = BALL_RADIUS;
	setPosition(0, 0, ball_radius*0.5f);
	setScale(ball_radius, ball_radius, ball_radius);

	if(!texturePath.empty())
	{
		loadTexture(texturePath, textureManager);
	}
}

void ball::update(float dt)
{
	doPhysics(dt);
	doTurnRed(dt);
}

void ball::doTurnRed(float dt)
{
	if(m_bTurnRed)
	{
		m_fTurnRedColor-=dt*10.0f;

		if(m_fTurnRedColor<0.0f)
		{
			m_fTurnRedColor = 0.0f;
			m_bTurnRed=false;
		}

		setColor(vector3f(1.0f, 1.0f-m_fTurnRedColor, 1.0f-m_fTurnRedColor));
	}
}

void ball::doPhysics(float dt)
{
	//F = M*A
	float inverseMass=0.1f;
	vector2f acc(m_cForce*inverseMass);
	vector2f vel(acc*dt);

	m_cVelocity = m_cVelocity+vel;
	vector2f pos(getPosition2());
	auto displacement = m_cVelocity*dt;
	pos = pos + m_cVelocity;

	m_cVelocity=m_cVelocity*0.9f;
	clearForce();

	setPosition(pos);
}

void ball::clearForce()
{
	m_cForce.zero();
}

void ball::addForce(vector2f force)
{
	m_cForce=m_cForce+force;
}

void ball::render(const matrix4x4f& renderMatrix)
{
	gxMesh::render();
}