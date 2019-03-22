#pragma once

#include "PhysicsEngine.h"
#include "BasicActors.h"
#include <iostream>
#include <iomanip>

namespace PhysicsEngine
{
	/*class CompoundShape : public DynamicActor
	{
	public:
		CompoundShape(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(0.5f, 0.5f, 0.5f), PxReal density = 1.0f)
			: DynamicActor(pose)
		{
			CreateShape(PxBoxGeometry(dimensions), density);
			CreateShape(PxBoxGeometry(dimensions), density);
		}
	};*/

	class tryPost : public DynamicActor
	{
	public:
		tryPost(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(.5f, 3.f, .5), PxReal density = 0.05f)
			: DynamicActor(pose)
		{
			CreateShape(PxBoxGeometry(5.0f, .5f, 0.5f), density);
			CreateShape(PxBoxGeometry(.5f, 7.0f, 0.5f), density);
			CreateShape(PxBoxGeometry(.5f, 7.0f, 0.5f), density);

			GetShape(0)->setLocalPose(PxTransform(PxVec3(.2f, 4.5f, .0f)));
			GetShape(1)->setLocalPose(PxTransform(PxVec3(5.f, 7.f, .0f)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(-5.f, 7.f, .0f)));
		}
	};

	class castle : public DynamicActor
	{
	public:
		castle(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(.5f, 3.f, .5f), PxReal density = 1.0f)
			: DynamicActor(pose)
		{
			CreateShape(PxBoxGeometry(4.0f, 6.0f, 4.0f), density);//Castle Block
			//Front battlement
			CreateShape(PxBoxGeometry(1.0f, .5f, .5f), density);
			CreateShape(PxBoxGeometry(1.0f, .5f, .5f), density);
			CreateShape(PxBoxGeometry(1.0f, .5f, .5f), density);
			
			GetShape(1)->setLocalPose(PxTransform(PxVec3(-3.f, 6.5f, -3.5f)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(0.0f, 6.5f, -3.5f)));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(3.f, 6.5f, -3.5f)));
			
			//Right battlement
			CreateShape(PxBoxGeometry(.5f, .5f, .5f), density);
			CreateShape(PxBoxGeometry(.5f, .5f, 1.f), density);
			CreateShape(PxBoxGeometry(.5f, .5f, 1.f), density);

			GetShape(4)->setLocalPose(PxTransform(PxVec3(-3.5f, 6.5f, -2.5f)));
			GetShape(5)->setLocalPose(PxTransform(PxVec3(-3.5f, 6.5f, 0.f)));
			GetShape(6)->setLocalPose(PxTransform(PxVec3(-3.5f, 6.5f, 3.f)));

			//Rear battlement
			CreateShape(PxBoxGeometry(.5f, .5f, .5f), density);
			CreateShape(PxBoxGeometry(1.0f, .5f, .5f), density);
			CreateShape(PxBoxGeometry(1.0f, .5f, .5f), density);

			GetShape(7)->setLocalPose(PxTransform(PxVec3(-2.5f, 6.5f, 3.5f)));
			GetShape(8)->setLocalPose(PxTransform(PxVec3(0.0f, 6.5f, 3.5f)));
			GetShape(9)->setLocalPose(PxTransform(PxVec3(3.f, 6.5f, 3.5f)));

			//Left battlement
			CreateShape(PxBoxGeometry(.5f, .5f, .5f), density);
			CreateShape(PxBoxGeometry(.5f, .5f, 1.f), density);
			CreateShape(PxBoxGeometry(.5f, .5f, .5f), density);

			GetShape(10)->setLocalPose(PxTransform(PxVec3(3.5f, 6.5f, 2.5f)));
			GetShape(11)->setLocalPose(PxTransform(PxVec3(3.5f, 6.5f, 0.f)));
			GetShape(12)->setLocalPose(PxTransform(PxVec3(3.5f, 6.5f, -2.5f)));
		}
	};

	//
	class CastleWall : public DynamicActor
	{
	public:
		CastleWall(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.0f, 3.0f, 26.f), PxReal density = 1.0f)
			: DynamicActor(pose)
		{
			CreateShape(PxBoxGeometry(dimensions), density);//Castle wall
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 2.f), density);

			//This is positioning the battlement walls along the wall to give it a castle look
			GetShape(1)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, 23.f)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, 15.f)));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, 7.f)));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, -1.f)));
			GetShape(5)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, -9.f)));
			GetShape(6)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, -17.f)));
			GetShape(7)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, -24.f)));
		}
	};

	//Repeat of class above (had trouble of making the wall longer to fit in the gap)
	class RearCastleWall : public DynamicActor
	{
	public:
		RearCastleWall(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.0f, 3.0f, 36.f), PxReal density = 1.0f)
			: DynamicActor(pose)
		{
			CreateShape(PxBoxGeometry(dimensions), density);//Castle wall
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			CreateShape(PxBoxGeometry(.5f, .5, 3.f), density);
			
			//This is positioning the battlement walls along the wall to give it a castle look
			GetShape(1)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, 31.f)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, 23.f)));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, 15.f)));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, 7.f)));
			GetShape(5)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, -1.f)));
			GetShape(6)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, -9.f)));
			GetShape(7)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, -17.f)));
			GetShape(8)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, -25.f)));
			GetShape(9)->setLocalPose(PxTransform(PxVec3(0.5f, 3.5f, -33.f)));
		}
	};

	class Wedge
	{
	public:
		vector<PxVec3> verts = { PxVec3(0.5,1,0), PxVec3(0.5,1,1), PxVec3(1,0,0), PxVec3(0,0,0), PxVec3(0,0,1), PxVec3(1,0,1) };
		ConvexMesh* mesh = new ConvexMesh(vector<PxVec3>(begin(verts), end(verts)));

		Wedge(float h = 1.0f, float w = 1.0f, float l = 1.0f, PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.0f)
		{
			verts[0].y *= h;
			verts[0].x *= w;
			verts[0].z *= l;
			verts[1].y *= h;
			verts[1].x *= w;
			verts[1].z *= l;
			verts[2].x *= w;
			verts[4].z *= l;
			verts[5].x *= w;
			verts[5].z *= l;
			mesh = new ConvexMesh(vector<PxVec3>(begin(verts), end(verts)), pose, density);
		}
	};

	class Walls : public DynamicActor
	{
	public:
		Walls(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(0.5f, 0.5f, 0.5f), PxReal density = 1.0f)
			: DynamicActor(pose)
		{
			CreateShape(PxBoxGeometry(dimensions), density);
			CreateShape(PxBoxGeometry(dimensions), density);
			//CreateShape(PxBoxGeometry(PxVec3(dimensions.z, dimensions.y, dimensions.x / 2)), density);
			//CreateShape(PxBoxGeometry(PxVec3(dimensions.z, dimensions.y, dimensions.x / 2)), density);
		}
	};

	class scorePost : public DynamicActor
	{
	public:
		scorePost(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(0.5, 0.5, 0.5), PxReal density = 1.0f)
			: DynamicActor(pose)
		{

		}
	};

	class CurvedWall
	{
	public:
		vector<PxVec3> verts{ PxVec3(1,0,0), PxVec3(0,0,1), PxVec3(0,0,0) };
		ConvexMesh* mesh = new ConvexMesh(vector<PxVec3>(begin(verts), end(verts)));

		CurvedWall(float l, int divisions, PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.0f)
		{
			float theta = 0.0f;

			verts[0] *= l;
			verts[1] *= l;
			for (int i = 0; i < divisions; i++)
			{
				theta = i * 2.0f * PxPi / (divisions * 4);
				verts.push_back(PxVec3((l * cosf(theta)), 0.0f, (l * sinf(theta))));
			}
			for (int i = 0; i < divisions + 3; i++)
			{
				verts.push_back(PxVec3(verts[i].x, verts[i].y + 1.0f, verts[i].z));
			}
			mesh = new ConvexMesh(vector<PxVec3>(begin(verts), end(verts)), pose, density);
		}
	};

	class Cloth : public Actor
	{
		PxClothMeshDesc mesh_desc;

	public:
		//constructor
		Cloth(PxTransform pose = PxTransform(PxIdentity), const PxVec2& size = PxVec2(1.f, 1.f), PxU32 width = 1, PxU32 height = 1, bool fix_top = true)
		{
			//prepare vertices
			PxReal w_step = size.x / width;
			PxReal h_step = size.y / height;

			PxClothParticle* vertices = new PxClothParticle[(width + 1)*(height + 1) * 4];
			PxU32* quads = new PxU32[width*height * 4];

			for (PxU32 j = 0; j < (height + 1); j++)
			{
				for (PxU32 i = 0; i < (width + 1); i++)
				{
					PxU32 offset = i + j * (width + 1);
					vertices[offset].pos = PxVec3(w_step*i, 0.f, h_step*j);
					if (fix_top && (j == 0)) //fix the top row of vertices
						vertices[offset].invWeight = 0.f;
					else
						vertices[offset].invWeight = 1.f;
				}

				for (PxU32 j = 0; j < height; j++)
				{
					for (PxU32 i = 0; i < width; i++)
					{
						PxU32 offset = (i + j * width) * 4;
						quads[offset + 0] = (i + 0) + (j + 0)*(width + 1);
						quads[offset + 1] = (i + 1) + (j + 0)*(width + 1);
						quads[offset + 2] = (i + 1) + (j + 1)*(width + 1);
						quads[offset + 3] = (i + 0) + (j + 1)*(width + 1);
					}
				}
			}

			//init cloth mesh description
			mesh_desc.points.data = vertices;
			mesh_desc.points.count = (width + 1)*(height + 1);
			mesh_desc.points.stride = sizeof(PxClothParticle);

			mesh_desc.invMasses.data = &vertices->invWeight;
			mesh_desc.invMasses.count = (width + 1)*(height + 1);
			mesh_desc.invMasses.stride = sizeof(PxClothParticle);

			mesh_desc.quads.data = quads;
			mesh_desc.quads.count = width * height;
			mesh_desc.quads.stride = sizeof(PxU32) * 4;

			//create cloth fabric (cooking)
			PxClothFabric* fabric = PxClothFabricCreate(*GetPhysics(), mesh_desc, PxVec3(0, -1, 0));

			//create cloth
			actor = (PxActor*)GetPhysics()->createCloth(pose, *fabric, vertices, PxClothFlags());
			//collisions with the scene objects
			((PxCloth*)actor)->setClothFlag(PxClothFlag::eSCENE_COLLISION, true);

			colors.push_back(default_color);
			actor->userData = new UserData(&colors.back(), &mesh_desc);
		}

		~Cloth()
		{
			delete (UserData*)actor->userData;
		}
	};
}