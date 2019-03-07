#pragma once

#include "BasicActors.h"
#include "CompoundActors.h"
#include <iostream>
#include <iomanip>

// Charlie Volland-Butler 14467072 University of Lincoln

namespace PhysicsEngine
{
	using namespace std;

	static int score = 0;
	//pyramid vertices
	static PxVec3 pyramid_verts[] = { PxVec3(0,1,0), PxVec3(1,0,0), PxVec3(-1,0,0), PxVec3(0,0,1), PxVec3(0,0,-1) };
	//pyramid triangles: a list of three vertices for each triangle e.g. the first triangle consists of vertices 1, 4 and 0
	//vertices have to be specified in a counter-clockwise order to assure the correct shading in rendering
	static PxU32 pyramid_trigs[] = { 1, 4, 0, 3, 1, 0, 2, 3, 0, 4, 2, 0, 3, 2, 1, 2, 4, 1 };

	class Pyramid : public ConvexMesh
	{
	public:
		Pyramid(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.f) :
			ConvexMesh(vector<PxVec3>(begin(pyramid_verts), end(pyramid_verts)), pose, density)
		{
		}
	};

	class PyramidStatic : public TriangleMesh
	{
	public:
		PyramidStatic(PxTransform pose = PxTransform(PxIdentity)) :
			TriangleMesh(vector<PxVec3>(begin(pyramid_verts), end(pyramid_verts)), vector<PxU32>(begin(pyramid_trigs), end(pyramid_trigs)), pose)
		{
		}
	};

	struct FilterGroup
	{
		enum Enum
		{
			ACTOR0 = (1 << 0),
			ACTOR1 = (1 << 1),
			ACTOR2 = (1 << 2)
			//add more if you need
		};
	};

	///An example class showing the use of springs (distance joints).
	class Trampoline
	{
	public:
		vector<DistanceJoint*> springs;
		Box *bottom, *top;

		Trampoline(const PxVec3& dimensions = PxVec3(1.f, 1.f, 1.f), PxReal stiffness = 1.f, PxReal damping = 1.f, PxTransform botPos = PxTransform(PxIdentity), PxTransform topPos = PxTransform(PxIdentity))
		{
			PxReal thickness = 0.08f;
			bottom = new Box(PxTransform(PxVec3(botPos.p.x, topPos.p.y + thickness, botPos.p.z), PxQuat(botPos.q)), PxVec3(dimensions.x, thickness, dimensions.z));
			top = new Box(PxTransform(PxVec3(topPos.p.x, topPos.p.y + dimensions.y + thickness, topPos.p.z), PxQuat(topPos.q)), PxVec3(dimensions.x, thickness, dimensions.z));
			springs.resize(4);
			springs[0] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, dimensions.z)));
			springs[1] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, -dimensions.z)));
			springs[2] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, dimensions.z)));
			springs[3] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, -dimensions.z)));

			for (unsigned int i = 0; i < springs.size(); i++)
			{
				springs[i]->Stiffness(stiffness);
				springs[i]->Damping(damping);
			}
			bottom->SetKinematic(true);
		}

		void AddToScene(Scene* scene)
		{
			scene->Add(bottom);
			scene->Add(top);
		}

		~Trampoline()
		{
			for (unsigned int i = 0; i < springs.size(); i++)
				delete springs[i];
		}
	};

	///A customised collision class, implemneting various callbacks
	class MySimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		//an example variable that will be checked in the main simulation loop
		bool trigger;

		MySimulationEventCallback() : trigger(false) {}

		///Method called when the contact with the trigger object is detected.
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count)
		{
			//you can read the trigger information here
			for (PxU32 i = 0; i < count; i++)
			{
				//filter out contact with the planes
				if (pairs[i].otherShape->getGeometryType() != PxGeometryType::ePLANE)
				{
					//check if eNOTIFY_TOUCH_FOUND trigger
					if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
					{
						cerr << "onTrigger::eNOTIFY_TOUCH_FOUND" << endl;
						trigger = true;
					}
					//check if eNOTIFY_TOUCH_LOST trigger
					if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
					{
						cerr << "onTrigger::eNOTIFY_TOUCH_LOST" << endl;
						trigger = false;
					}
				}
			}
		}

		///Method called when the contact by the filter shader is detected.
		virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs)
		{
			cerr << "Contact found between " << pairHeader.actors[0]->getName() << " " << pairHeader.actors[1]->getName() << endl;

			//check all pairs
			for (PxU32 i = 0; i < nbPairs; i++)
			{
				switch (pairs[i].shapes[0]->getSimulationFilterData().word0)
				{
				case FilterGroup::ACTOR0:
					cerr << "tits" << endl;
					break;
				case FilterGroup::ACTOR1:
					score += 10;
					break;
				case FilterGroup::ACTOR2:
					score += 5;
					break;
				}
			}
		}

		virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
		virtual void onWake(PxActor **actors, PxU32 count) {}
		virtual void onSleep(PxActor **actors, PxU32 count) {}
	};

	//A simple filter shader based on PxDefaultSimulationFilterShader - without group filtering
	static PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1,
		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
	{
		// let triggers through
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlags();
		}

		//enable continous collision detection
		pairFlags = PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;


		//customise collision filtering here
		//e.g.

		// trigger the contact callback for pairs (A,B) where 
		// the filtermask of A contains the ID of B and vice versa.
		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			//trigger onContact callback for this pair of objects
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
			//			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
		}

		return PxFilterFlags();
	};

	///Custom scene class
	class MyScene : public Scene
	{
		MySimulationEventCallback* my_callback;

		Plane* plane;
		Box* base;

		Walls *walls;

		Trampoline *launcher;

		DistanceJoint *spring;

		bool pullSpring = false;
		float springStr = 0.0f;

		Wedge *leftPaddle, *rightPaddle;
		RevoluteJoint *LPjoint, *RPjoint;


		Box *box1, *box2, *box3, *box4, *box5, *box6, *box7;

	public:
		Sphere* ball;
		//specify your custom filter shader here
		//PxDefaultSimulationFilterShader by default
		MyScene() : Scene(CustomFilterShader) {};

		///A custom scene class
		void SetVisualisation()
		{
			px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);

		}

		//Custom scene initialisation
		virtual void CustomInit()
		{
			SetVisualisation();
			CreateMaterial(0.94, 0.40, 0.1);
			CreateMaterial(1.10, 0.15, 0.2);
			CreateMaterial(0.25, 0.1, 0.4);
			CreateMaterial(0.78, 0.42, 0.2);
			CreateMaterial(0.04, 0.04, 0.1);
			CreateMaterial(0.38, 0.20, 0.4);
			CreateMaterial(0.10, 0.03, 0.1);
			CreateMaterial(0.10, 0.08, 0.2);

			float tableAngle = PxPi / 180;

			///Initialise and set the customised event callback
			my_callback = new MySimulationEventCallback();
			px_scene->setSimulationEventCallback(my_callback);
			px_scene->setFlag(PxSceneFlag::eENABLE_CCD, true);

			// actor 1 plane
			plane = new Plane();
			plane->Color(PxVec3(40.f / 55.f, 210.f / 255.f, 210.f / 255.f));
			plane->Material(GetMaterial(2));
			Add(plane);

			// actor 2 base
			base = new Box(PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(tableAngle, PxVec3(0.0f, 0.0f, 1.0f))), PxVec3(100.0f, 0.5f, 10.0f));
			base->Color(PxVec3(50.f / 50.f, 30.f / 55.f, 20.f / 255.f));
			base->Material(GetMaterial(6));
			base->SetKinematic(true);
			Add(base);

			// actor 3 ball
			ball = new Sphere(PxTransform(PxVec3(-8.0f, 12.0f, 9.0f), PxQuat(PxIdentity)), 0.3f);
			ball->Material(GetMaterial(3));
			Add(ball);
			ball->Get()->isRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
			ball->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR1);
			ball->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR2);

			// actor 4 left paddle
			leftPaddle = new Wedge(4.0f, 1.5f, 1.f, PxTransform(PxVec3(-12.0f, 6.0f, -5.0f), PxQuat(tableAngle, PxVec3(0.0f, 0.0f, 1.0f))), 1.0f);
			leftPaddle->mesh->GetShape()->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxPi / 2, PxVec3(1.0f, 0.0f, 0.0f))));
			leftPaddle->mesh->SetKinematic(false);
			Add(leftPaddle->mesh);
			LPjoint = new RevoluteJoint(base, PxTransform(PxVec3(-12.f, 1.f, -5.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f))), leftPaddle->mesh, PxTransform(PxVec3(0.5f, 0.f, 0.5f), PxQuat(PxPi / 2, PxVec3(0.0f, 0.0f, 1.0f))));
			LPjoint->SetLimits(-PxPi / 6, PxPi / 6);
			leftPaddle->mesh->SetupFiltering(FilterGroup::ACTOR2, FilterGroup::ACTOR0);

			// actor 5 right paddle
			rightPaddle = new Wedge(4.0f, 1.5f, 1.f, PxTransform(PxVec3(-12.0f, 6.0f, 5.0f), PxQuat(tableAngle, PxVec3(0.0f, 0.0f, 1.0f))));
			rightPaddle->mesh->GetShape()->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(-PxPi / 2, PxVec3(1.0f, 0.0f, 0.0f))));
			rightPaddle->mesh->SetKinematic(false);
			Add(rightPaddle->mesh);
			RPjoint = new RevoluteJoint(base, PxTransform(PxVec3(-12.f, 0.5f, 5.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f))), rightPaddle->mesh, PxTransform(PxVec3(0.5f, 0.f, -0.5f), PxQuat(PxPi / 2, PxVec3(0.0f, 0.0f, 1.0f))));
			RPjoint->SetLimits(-PxPi / 6, PxPi / 6);
			rightPaddle->mesh->SetupFiltering(FilterGroup::ACTOR2, FilterGroup::ACTOR0);

			// actor 6 and 7 bottom and top respective plunger components
			launcher = new Trampoline(PxVec3(.5f, 0.5f, 0.5f), 100.0f, 10.0f, PxTransform(PxVec3(-35.0f, 3.6f, 0.0f), PxQuat(-tableAngle * 2, PxVec3(0.0f, 0.0f, 1.0f))), PxTransform(PxVec3(-35.0f, 3.6f, 0.0f), PxQuat(-tableAngle * 2, PxVec3(0.0f, 0.0f, 1.0f))));
			launcher->AddToScene(this);

			walls = new Walls(PxTransform(PxVec3(PxIdentity), PxQuat(PxPi / 180, PxVec3(0.0f, 0.0f, 1.0f))), PxVec3(30.0f, 2.0f, 0.5f));
			walls->GetShape(0)->setLocalPose(PxTransform(PxVec3(4.0f, 1.0f, 10.5f), PxQuat(PxIdentity))); //(y, z, x) 
			walls->GetShape(1)->setLocalPose(PxTransform(PxVec3(4.0f, 1.0f, -10.5f), PxQuat(PxIdentity)));
			walls->GetShape(2)->setLocalPose(PxTransform(PxVec3(-29.0f, 1.0f, 0.0f), PxQuat(PxIdentity)));
			walls->GetShape(3)->setLocalPose(PxTransform(PxVec3(100.0f, 1.5f, 0.0f), PxQuat(PxIdentity)));
			walls->SetKinematic(true);
			Add(walls);

			box1 = new Box(PxTransform(PxVec3(13.0f, 20.0f, -8.0f), PxQuat(tableAngle, PxVec3(0.0f, 0.0f, 1.0f))), PxVec3(4.0f, 1.0f, 0.5f));//top left
			box1->GetShape()->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(-PxPi / 4, PxVec3(0.0f, 1.0f, 0.0f))));
			box1->SetKinematic(true);
			Add(box1);

			box2 = new Box(PxTransform(PxVec3(13.0f, 20.0f, 8.0f), PxQuat(tableAngle, PxVec3(0.0f, 0.0f, 1.0f))), PxVec3(4.0f, 1.0f, 0.5f));//To right
			box2->GetShape()->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxPi / 4, PxVec3(0.0f, 1.0f, 0.0f))));
			box2->SetKinematic(true);
			Add(box2);

			box3 = new Box(PxTransform(PxVec3(-8.0f, 8.0f, -8.0f), PxQuat(tableAngle, PxVec3(0.0f, 0.0f, 1.0f))), PxVec3(3.0f, 1.0f, 0.5f));//bottom left
			box3->GetShape()->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxPi / 3, PxVec3(0.0f, 1.0f, 0.0f))));
			box3->SetKinematic(true);
			Add(box3);

			box4 = new Box(PxTransform(PxVec3(-9.0f, 8.0f, 7.0f), PxQuat(tableAngle, PxVec3(0.0f, 0.0f, 1.0f))), PxVec3(1.25f, 1.0f, 0.5f));//bottom right
			box4->GetShape()->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(-PxPi / 3, PxVec3(0.0f, 1.0f, 0.0f))));
			box4->SetKinematic(true);
			Add(box4);

			//canon wall
			box5 = new Box(PxTransform(PxVec3(-35.0f, 3.5f, 1.3f), PxQuat(tableAngle, PxVec3(0.0f, 0.0f, 1.0f))), PxVec3(7.f, 1.0f, 0.5f)); //middle right
			box5->SetKinematic(true);
			Add(box5);
			
			//canon wall
			box6 = new Box(PxTransform(PxVec3(-35.0f, 3.5f, -1.3f), PxQuat(tableAngle, PxVec3(0.0f, 0.0f, 1.0f))), PxVec3(7.f, 1.0f, 0.5f)); //middle right
			box6->SetKinematic(true);
			Add(box6);

			//canon wall
			box7 = new Box(PxTransform(PxVec3(-35.5f, 1.0f, 0.0f), PxQuat(tableAngle, PxVec3(0.0f, 0.0f, 1.0f))), PxVec3(3.f, 2.5f, 15.1f)); //(y, z, x)
			box7->SetKinematic(true);
			Add(box7);
		}

		//Custom udpate function
		virtual void CustomUpdate()
		{

			if (pullSpring)
			{
				springStr += 2.0f;
				if (springStr >= 200.0f) { springStr = 200.0f; }
			}
		}

		virtual int GetScore()
		{
			return score;
		}

		void KeyPressR()
		{
			RPjoint->DriveVelocity(-10.0f);
		}
		void KeyReleaseR()
		{
			RPjoint->DriveVelocity(10.0f);
		}

		void KeyPressE()
		{
			LPjoint->DriveVelocity(10.0f);
		}
		void KeyReleaseE()
		{
			LPjoint->DriveVelocity(-10.0f);
		}

		void KeyPressB()
		{
			pullSpring = true;
		}


		void KeyReleaseB()
		{
			pullSpring = false;
			this->SelectActor(5);
			this->GetSelectedActor()->addForce(PxVec3(10.0f, 1.0f, 0.0f) * springStr);
			springStr = 0.0f;

		}
	};
}
