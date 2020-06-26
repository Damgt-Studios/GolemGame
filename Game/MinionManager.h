//#pragma once
//#include "pchgame.h"
//
//#include "GameUtilities.h"
////#include "ResourceManager.h"
////#include "GameEffects.h"
////#include "ADAI.h"
//
//
//
//class MinionManager
//{
//	MinionManager() {};
//	~MinionManager() { Shutdown(); };
//	MinionManager(const MinionManager& _rhs) {};
//	MinionManager& operator =(const MinionManager& _rhs) {};
//
//	const UINT MAX_MINIONS = 40;
//
//	//Create a list of empty available golems.
//	ADAI::AIUnit** allUnits = new ADAI::AIUnit*[40];
//
//	std::list<UINT> available;
//
//	//Create a vector for active golems of each type.  
//	std::vector<ADAI::AIUnit*> stoneMinions;
//	std::vector<ADAI::AIUnit*> waterMinions;
//	std::vector<ADAI::AIUnit*>  fireMinions;
//	std::vector<ADAI::AIUnit*>  woodMinions;
//
//	std::vector<std::string> stoneMinionAnimations;
//	std::vector<std::string> waterMinionAnimations;
//	std::vector<std::string> fireMinionAnimations;
//	std::vector<std::string> woodMinionAnimations;
//
//
//public:
//	static MinionManager* Instance()
//	{
//		static MinionManager instance;
//		return &instance;
//	};
//
//	void Initialize(Golem* _golem);
//	void Shutdown();
//
//	ADAI::AIUnit* BirthStoneMinion(ADAI::FlockingGroup* _commandGroup, XMFLOAT3 _position = { 0,0,0 });
//	//void Death(UINT _index);
//};
