#pragma once
#include "GameplayBaseClasses.h"
#include "GameObjectClasses.h"
#include "ADUserInterface.h"
#include "AudioManager.h"
#include "ADAI.h"
#include "AnimationStateMachine.h"

using namespace ADResource::ADGameplay;
using namespace ADPhysics;

//This isn't trash
//  This sound should be played on each step when in motion.  Guess there are few ways to do that.
//audioEngine.PlayEvent("event:/Sfx_GolemStep");
//
///End not trash

namespace
{
	struct GolemAnimations
	{
		std::string command;
		std::string death;
		std::string eat;
		std::string groundSlam;
		std::string heavyHitBack;
		std::string heavyHitFront;
		std::string heavyHitLeft;
		std::string heavyHitRight;
		std::string idle;
		std::string idleLook;
		std::string kick;
		std::string run;
		std::string special;
		std::string summonMinions;
		std::string towerPunch;
	};
}

namespace ADResource
{
	namespace ADGameplay
	{
		enum SoundIds
		{
			GemCollectSound = 0,
			DestructableBreakSound,
			EnemyDeathSound,
			PlayerChargeSound,
			PlayerDeathSound,
			FireBrathSound,
			GlideSound,
			HurtSound,
			JumpSound,
			LandSound,
			ObjectHitSound,
			RunningJumpSound,
			WalkingSound,
			WaterSplashSound
		};

		class Golem : public Renderable
		{
		public:
			// Public Methods
			// Constructor and Destructor
			Golem();
			~Golem();


			// Update Methods
			virtual void Update(float time_delta);
			void ProcessEffects(float _deltaTime);
			void CheckCollision(GameObject* obj);
			virtual void OnCollision(GameObject* other);
			void Remove();


			// Accessors
			void GetView(XMMATRIX& view);
			void GetAnimationController(AnimationStateMachine& controller);
			XMMATRIX GetColliderInfo();
			virtual iStatSheet* GetStatSheet() override;
			int GetCurrentElement();




			// Public Data Members
			// AI
			GameObject* targetMarker;
			ADAI::FlockingGroup** flockingGroups;
			int commandTargetGroup = 0;
			int totalMinionCount = 0;
			int stoneMinionCount = 0;
			int waterMinionCount = 0;
			int fireMinionCount = 0;
			int woodMinionCount = 0;


			// Physics
			OBB collider = OBB(transform, XMFLOAT3(20, 25, 20));
			PhysicsMaterial mat = PhysicsMaterial(1, 1, 0.5f);
			


			// Commented Out Methods
			//void Damage(DAMAGE_TYPE d_type);
			//virtual void OnTrigger(GameObject* other);

		private:
			// Private Methods
			void HandleInput(float delta_time);
			void InitAnims();
			void MoveGolem(XMFLOAT4& forward, float delta_time);
			void PerformSpecial();
			void TowerPunch();
			void GroundSlam();
			void Kick();
			void CommandMinions();
			void RecallMinions();
			void ChangeElement(bool nextElement);
			void ChangeMinionGroup(bool nextElement);



			// Private Data Members
			// Golem Stats
			ADResource::ADGameplay::StatSheet* stats;
			int health;
			int playerElement = 0;


			// Audio Stuff
			bool playingSound = false;
			UINT collectionNoiseID;


			// Collider Translations
			XMMATRIX translateToFront = XMMatrixTranslation(transform.r[3].m128_f32[0], transform.r[3].m128_f32[1], transform.r[3].m128_f32[2] + 3);
			XMMATRIX translateToMiddle = XMMatrixTranslation(transform.r[3].m128_f32[0], transform.r[3].m128_f32[1] + 15, transform.r[3].m128_f32[2]);


			// Gameplay
			const float invulnerablePeriod = 1.6f;
			float invulnerableTimer = 0;
			float responseTimer = 0;
			int specialCoins = 3;


			// Movement
			float golemTurnSpeed = 5;
			float golemMoveSpeed = 500;
			XMFLOAT3 golemMovementDeltas = XMFLOAT3(0, 0, 0);


			// Animation
			bool isActing = false;
			double currentAnimTime = 0.0;
			double idleTime = 0.0;
			AnimationStateMachine* anim_controller;
			GolemAnimations anims[4];


			// Camera
			XMMATRIX camera;
		};
	}
}