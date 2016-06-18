/*
 *  ECPlayer1.h
 *  waves2
 *
 *  Created by arun on 24/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ECPLAYER1_H
#define ECPLAYER1_H

#include "../engine/core/TextureManager.h"
#include "../Map/ECActor.h"


#define DPAD_FORCE_THRESHOLD    0.03f

class ECPlayer1 : public ECActor
{
public:
    
    enum EPLAYERSTATES
    {
        IDLE_EAST=0,
        IDLE_NORTH,
        IDLE_WEST,
        IDLE_SOUTH,
        WALK_EAST,
        WALK_NORTH,
        WALK_WEST,
        WALK_SOUTH,
        RUN_EAST,
        RUN_NORTH,
        RUN_WEST,
        RUN_SOUTH,
        
        JUMP_EAST,
        JUMP_NORTH,
        JUMP_WEST,
        JUMP_SOUTH,
        
        HURT_EAST,
        HURT_NORTH,
        HURT_WEST,
        HURT_SOUTH,
        
        DIG_EAST,
        DIG_NORTH,
        DIG_WEST,
        DIG_SOUTH,

        HIT_EAST,
        HIT_NORTH,
        HIT_WEST,
        HIT_SOUTH,
        
        DIG_SPARK_EAST,
        DIG_SPARK_NORTH,
        DIG_SPARK_WEST,
        DIG_SPARK_SOUTH,
        
        ELECTROCUTE_EAST,
        ELECTROCUTE_NORTH,
        ELECTROCUTE_WEST,
        ELECTROCUTE_SOUTH,

        PLAYER_MAX_STATE
    };
    
	ECPlayer1();
	~ECPlayer1();
	
	void init(const char* filename, CTextureManager* texturemanager, ECMap* map, CommonData* commonData);
	
	void update(float dt);
	void render();
    
    void resetPlayer1();
    
    bool circleCollision(ECActor* actor, ECTile* tile, vector2x& newPos, int& row, int& col);    
    bool setPlayerState(EPLAYERSTATES ePlayerState);
    EPLAYERSTATES getPlayerState()  {   return m_ePlayerState;  }
    
    //stAnimationPackage - callbacks
    void callback_transitToNextState(int stateIndex);
	void callback_transitToPrevState(int stateIndex);
	void callback_commandCompleted(int commandId);
	void callback_loopCompleted(int stateIndex);
	void callback_holdReached();
	void callback_commandChangedTo(int commandId);
    
    bool isPlayerCaught()               {   return m_bPlayerCaught;     }
    void setPlayerCaught(bool  status)
    {   
        m_bPlayerCaught = status;   
        if(!status) 
        {
#if defined (LOG_DEBUB_VERBOSE)
           DEBUG_PRINT("setCaught to false");
#endif           
        }          
        else
        {
#if defined (LOG_DEBUB_VERBOSE)
            DEBUG_PRINT("setCaught to true");
#endif
        }            
    }
    bool isActionBubbleActive()
    {
        return m_bBubbleActive;
    }
    void setActionBubbleActive(bool status)
    {
        m_bBubbleActive = status;
    }
    
    bool isGodMode()                    {   return m_bGodMode;            }
    void setGodMode(bool flag)          
    {  
        m_bGodMode = flag; 
        m_fGodModeElapseTime = 0.0f;        
    }
    
    void appendHeadIcon(int val)
    {
        if(m_iHeadIconCntr>=2) return;
        m_iszHeadIconList[m_iHeadIconCntr++]=val;
    }
    
    void clearHeadIcons()
    {
        m_iHeadIconCntr=0;
    }
    
    vector2x snapForDigAnimation();
    
    float getDigElapseTime()    { return m_fDigElapseTime;}
    void setDigElapseTime(float val)    { m_fDigElapseTime =val;}
    void setTeaserEnemyPtr(ECActor* enemy)  {   m_pTeaserEnemyPtr=enemy;    }
    ECActor* getTeaserEnemyPtr()    {   return m_pTeaserEnemyPtr;   }
    
    ECTile* getPrevWalkedTile() {   return m_pPrevWalkedTilePtr;    }
    
    void setNoOfTapkitCollected(int nTapkit)   {   m_nTapKitCollected=nTapkit; }
    int getNoOfTapkitCollected()    {   return m_nTapKitCollected;  }
    
private:   
#ifdef ENABLE_OPENFEINT
    int m_iOGHurtCounter;//to unlock achievement 'Mr. Vengence'
    int m_iOGMoneybagCounter;//for 'TREASURE_COLLECTOR' openfeint achievement unlock
    int m_iOGHoleInNonTreasureAreaCounter;//to unlock achievement 'EXCAVATOR'
#endif
    
    EPLAYERSTATES m_ePlayerState;
//    Sprite2Dx m_cPlayerIcon;
    bool m_bPlayerCaught;
    bool m_bBubbleActive;
    CommonData* m_pCommonDataPtr;   //must not delete this pointer
    bool m_bGodMode;
    float m_fGodModeElapseTime;
    AnimSprite m_cAnimSprite;
    AnimSprite m_cPitAnimSprite;
    int m_iszHeadIconList[2];   //1=alert, 0=tressure, 2=bomb, 3=tapkit
    int m_iHeadIconCntr;        //range 0 to 2
    
    float m_fDigElapseTime;
    float m_fElectrifingElapseTime;
    ECActor* m_pTeaserEnemyPtr;
    vector2x m_cJumpStartPos;
    float m_fPlayerHurtTime;
    ECTile* m_pPrevWalkedTilePtr;
    int m_nTapKitCollected;
    
//    float m_fszFOVArray[3*2];
};

//#include "ECMainGame.h"
#endif