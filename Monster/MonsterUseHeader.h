#include "../../stdafx.h"
#include "../Map.h"
#include "../Status.h"
#include <time.h>
#include "../Character/CharacterParant.h"
#include "../Item/ItemList.h"
#include "../../Framework/UIController.h"
#include "../SkillManager.h"
#include "../DamageUI.h"
#include "../Character/Npc.h"
#include "HitParticle.h"

#define PCHARACTER (*m_ppCharacter)
#define CHARACTER (*m_ppCharacter)->GetCharacter()