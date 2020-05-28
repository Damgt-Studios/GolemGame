#pragma once
//#include "GameplayBaseClasses.h"
//#include "ADUserInterface.h"
//#include "AudioManager.h"
//#include "GameObjectClasses.h"
//#include "ADAI.h"

//using namespace ADResource::ADGameplay;
//using namespace ADPhysics;

//namespace ADResource
//{
//	namespace ADGameplay
//	{
//		enum SoundIds
//		{
//			GemCollectSound = 0,
//			DestructableBreakSound,
//			EnemyDeathSound,
//			PlayerChargeSound,
//			PlayerDeathSound,
//			FireBrathSound,
//			GlideSound,
//			HurtSound,
//			JumpSound,
//			LandSound,
//			ObjectHitSound,
//			RunningJumpSound,
//			WalkingSound,
//			WaterSplashSound
//		};

//		class Spyro : public Destructable
//		{
		
	//	public:
	//		Attack testAttack;
	//		ADAI::FlockingGroup* commandGroup;
	//		Spyro();

	//		virtual void Update(float time_delta);
//
			//void Damage(DAMAGE_TYPE d_type);

		//	void Remove();

		//	void CheckCollision(GameObject* obj);

		//	void CheckCollision(AABB& item);

		//	virtual void OnCollision(GameObject* other);
		//	void OnTriggerCharge(GameObject* other);
		//	void OnTriggerFire(GameObject* other);
	//		virtual void OnTrigger(GameObject* other);

	//		void GetView(XMMATRIX& view);
	//		OBB collider = OBB(transform, XMFLOAT3(2, 2, 2));
	//		PhysicsMaterial mat = PhysicsMaterial(1, 1, 0.5f);
	//		bool jumping = false;

		//	void SetAudio(AudioManager* _audioManager);

	//	private:
	//		void HandleInput(float delta_time);

//		private:
	//		int health;
	//		bool playingSound = false;
	//		UINT collectionNoiseID;
//			AudioManager* audioManager;

//			XMFLOAT3 spryo_movement_deltas = XMFLOAT3(0, 0, 0);

	//		OBB chargeCollider = OBB(transform, XMFLOAT3(2, 2, 2));
	//		Collider* chargeCPtr = nullptr;
			OBB fireCollider = OBB(transform, XMFLOAT3(2, 2, 2));
			Collider* fireCPtr = nullptr;

			// Gameplay
			const float invulnerable_peroid = 1.6f;
			float invulnerable_timer = 0;

			float jump_time_up = .4;
			float jump_time_length = 1;
			float jump_count_down = 0;
			float jump_height = 15;
			float og_y_pos = 0;
			float gravity = 50;
			bool gliding = false;
			bool charging = false;
			bool fire = false;


			float floatiness = 0.25f;
			XMMATRIX translatetofront = XMMatrixTranslation(transform.r[3].m128_f32[0], transform.r[3].m128_f32[1], transform.r[3].m128_f32[2] + 3);


			bool buttonup = false;
			XMMATRIX camera;
			// Turning
			float spyro_turn_speed = 5;
			float spyro_move_speed = 30;
		};
	}
}

namespace ADResource
{
	namespace ADGameplay
	{
		
	}
}
=======
//#include "GameplayBaseClasses.h"
//#include "ADUserInterface.h"
//#include "AudioManager.h"
//#include "ADAudio.h"
//#include "GameObjectClasses.h"
//
//using namespace ADResource::ADGameplay;
//using namespace ADPhysics;
//
//namespace ADResource
//{
//	namespace ADGameplay
//	{
//		//enum SoundIds
//		//{
//		//	GemCollectSound = 0,
//		//	DestructableBreakSound,
//		//	EnemyDeathSound,
//		//	PlayerChargeSound,
//		//	PlayerDeathSound,
//		//	FireBrathSound,
//		//	GlideSound,
//		//	HurtSound,
//		//	JumpSound,
//		//	LandSound,
//		//	ObjectHitSound,
//		//	RunningJumpSound,
//		//	WalkingSound,
//		//	WaterSplashSound
//		//};
//
//		class Spyro : public Destructable
//		{
//		
//		public:
//			Attack testAttack;
//			Spyro();
//
//			virtual void Update(float time_delta);
//
//			//void Damage(DAMAGE_TYPE d_type);
//
//			void Remove();
//
//			void CheckCollision(GameObject* obj);
//
//			void CheckCollision(AABB& item);
//
//			virtual void OnCollision(GameObject* other);
//			void OnTriggerCharge(GameObject* other);
//			void OnTriggerFire(GameObject* other);
//			virtual void OnTrigger(GameObject* other);
//
//			void GetView(XMMATRIX& view);
//			OBB collider = OBB(transform, XMFLOAT3(2, 2, 2));
//			PhysicsMaterial mat = PhysicsMaterial(1, 1, 0.5f);
//			bool jumping = false;
//
//			//void SetAudio(AudioManager* _audioManager);
//
//		private:
//			float responseTimer = 0;
//			void HandleInput(float delta_time);
//
//		private:
//			std::vector<ADResource::ADGameplay::Destructable*> fireMinions;
//			std::vector<ADResource::ADGameplay::Destructable*> waterMinions;
//			std::vector<ADResource::ADGameplay::Destructable*> stoneMinions;
//			std::vector<ADResource::ADGameplay::Destructable*> woodMinions;
//
//			int health;
//			bool playingSound = false;
//			UINT collectionNoiseID;
//			//AudioManager* audioManager;
//
//			XMFLOAT3 spryo_movement_deltas = XMFLOAT3(0, 0, 0);
//
//			OBB chargeCollider = OBB(transform, XMFLOAT3(2, 2, 2));
//			Collider* chargeCPtr = nullptr;
//			OBB fireCollider = OBB(transform, XMFLOAT3(2, 2, 2));
//			Collider* fireCPtr = nullptr;
//
//			// Gameplay
//			const float invulnerable_peroid = 1.6f;
//			float invulnerable_timer = 0;
//
//			float jump_time_up = .4;
//			float jump_time_length = 1;
//			float jump_count_down = 0;
//			float jump_height = 15;
//			float og_y_pos = 0;
//			float gravity = 50;
//			bool gliding = false;
//			bool charging = false;
//			bool fire = false;
//
//
//			float floatiness = 0.25f;
//			XMMATRIX translatetofront = XMMatrixTranslation(transform.r[3].m128_f32[0], transform.r[3].m128_f32[1], transform.r[3].m128_f32[2] + 3);
//
//
//			bool buttonup = false;
//			XMMATRIX camera;
//			// Turning
//			float spyro_turn_speed = 5;
//			float spyro_move_speed = 30;
//		};
//	}
//}
//
//namespace ADResource
//{
//	namespace ADGameplay
//	{
//		
//	}
//}
