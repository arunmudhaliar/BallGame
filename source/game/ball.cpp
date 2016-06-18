#include "ball.h"

ball::ball() :
gxMesh()
{
}

ball::~ball()
{
}

void ball::load(const std::string& path)
{
	gxFile file;
	if (file.OpenFile(path.c_str()))
	{
		//read meta header
		struct tmpStruct
		{
			__int64 headerTime[3];
			int headerType;
		};
		tmpStruct header;
		file.ReadBuffer((unsigned char*)&header, sizeof(header));

		//read object ID
		int objID = 0;
		file.Read(objID);
		readHeader(file);
		int nChild = 0;
		file.Read(nChild);

		file.Read(objID);
		readHeader(file);

		char* scriptname = file.ReadString();
		GX_DELETE_ARY(scriptname);

		readScriptObject(file);

		file.CloseFile();
	}

	//identity();
	setPosition(0, 0, 5.0f);
	setScale(10.0f, 10.0f, 10.0f);
}

void ball::readHeader(gxFile& file)
{
	//core - loop
	int baseflag = 0;
	file.Read(baseflag);
	char* temp = file.ReadString();
	GX_DELETE_ARY(temp);
	file.ReadBuffer((unsigned char*)m, sizeof(m));
	float oobb[6];
	file.ReadBuffer((unsigned char*)&oobb, sizeof(oobb));
	int crc;
	file.Read(crc);

	bool bAnimationController = false;
	file.Read(bAnimationController);

	int nAttachedScripts = 0;
	file.Read(nAttachedScripts);
	//
}

void ball::update(float dt)
{
	updatePhysics(dt);
}

void ball::updatePhysics(float dt)
{
	//F = M*A
	float inverseMass=1.0f;
	vector2f acc(m_cForce*inverseMass);
	vector2f vel(acc*dt);

	m_cVelocity = m_cVelocity+vel;
	vector2f pos(getPosition2());
	//float speed=(m_cVelocity*dt).length();
	//pos=pos-getYAxis()*speed;
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
	//objectBase::render(&renderMatrix);
	gxMesh::render();
}
