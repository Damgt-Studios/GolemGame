#pragma once
#include "ADAI.h"
#include "GameplayAI.h"
#include "AnimationStateMachine.h"
#include "GameEffects.h"
#include "ADCombat.h"
#include "ADParticles.h"

using namespace ADResource::ADGameplay;
using namespace ADPhysics;

//This isn't trash
//  This sound should be played on each step when in motion.  Guess there are few ways to do that.
//audioEngine.PlayEvent("event:/Sfx_GolemStep");
//
///End not trash

namespace ADResource
{
	namespace ADGameplay
	{

		class Golem : public Renderable
		{
			int playerElement = 0;
			AnimationStateMachine* anim_controller[4];

		public:
			// Public Methods
			// Constructor and Destructor
			Golem();
			~Golem();


			virtual void Update(float time_delta);
			void ProcessEffects(float _deltaTime);
			void CheckCollision(GameObject* obj);
			virtual void OnCollision(GameObject* other, Manifold& m);
			void Remove();
			void Death();
			void InitializeController();


			// Accessors
			void GetView(XMMATRIX& view);
			XMMATRIX GetColliderInfo();
			virtual StatSheet* GetStatSheet() override;
			int GetCurrentElement();




			// Public Data Members
			// AI
			GameObject* targetMarker = nullptr;

			float commandDistanceTimer = 0;
			ADAI::MinionGroup** minionGroups = nullptr;
			int commandTargetGroup = 0;


			// Physics
			OBB collider = OBB(transform, XMFLOAT3(20, 25, 20));
			PhysicsMaterial mat = PhysicsMaterial(1, 1, 0.5f);

			ADResource::ADGameplay::StatSheet* stats = nullptr;
			struct GolemActions
			{
				Action* punch = nullptr;
				Action* kick = nullptr;
				Action* slam = nullptr;
				Action* special = nullptr;
				Action* nextForm = nullptr;
				Action* prevForm = nullptr;
			} gActions[4];
			Action* consume = nullptr;

			AD_ULONG meshIDs[4];

			BigElementalPuffEmitter* bigPuffs[4] = { nullptr, nullptr, nullptr, nullptr };

		private:
			// Private Methods
			void HandleInput(float delta_time);
			void InitAnims();
			void MoveGolem(XMFLOAT4& forward, float delta_time);
			void PerformSpecial();
			void TowerPunch();
			void GroundSlam();
			void Kick();
			void CastCommandTarget(float delta_time);			
			void SendToCommandTarget(float delta_time);
			void CommandMinions();
			void RecallMinions();
			void ChangeElement(bool nextElement);
			void ChangeMinionGroup(bool nextElement);
			void ConsumeMinion();
			void SummonMinions();
			void FlinchFromFront();
			void FlinchFromBack();
			void FlinchFromLeft();
			void FlinchFromRight();
			void Die();



			// Private Data Members
			// Golem Stats
			bool isAlive = true;
			//int health;



			// Audio Stuff
			bool playingSound = false;
			UINT collectionNoiseID = 0;


			// Collider Translations
			XMMATRIX translateToFront = XMMatrixTranslation(transform.r[3].m128_f32[0], transform.r[3].m128_f32[1], transform.r[3].m128_f32[2] + 3);
			XMMATRIX translateToMiddle = XMMatrixTranslation(transform.r[3].m128_f32[0], transform.r[3].m128_f32[1] + 15, transform.r[3].m128_f32[2]);


			// Gameplay
			const float invulnerablePeriod = 1.6f;
			float invulnerableTimer = 0;
			float responseTimer = 0;


			// Movement
			XMFLOAT3 golemMovementDeltas = XMFLOAT3(0, 0, 0);


			// Animation
			bool isActing = false;
			double currentAnimTime = 0.0;
			double idleTime = 0.0;
			struct GolemAnimations
			{
				std::string born = "";
				std::string command = "";
				std::string death = "";
				std::string eat = "";
				std::string groundSlam = "";
				std::string heavyHitBack = "";
				std::string heavyHitFront = "";
				std::string heavyHitLeft = "";
				std::string heavyHitRight = "";
				std::string idle = "";
				std::string idleLook = "";
				std::string kick = "";
				std::string run = "";
				std::string special = "";
				std::string summonMinions = "";
				std::string towerPunch = "";
			} anims[4];


			// Camera
			XMMATRIX camera;
		};
	}
}