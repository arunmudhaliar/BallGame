/*
 *  ECPlayer1.cpp
 *  waves2
 *
 *  Created by arun on 24/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ECPlayer1.h"
#include "../engine/util/util.h"
#include "../engine/util/gxFile.h"
#include "../Game/ECDefines.h"
#include "ECEnemy.h"
#include "../UI/ECBonus.h"
#include "../UI/ECCollectionSparkle.h"

ECPlayer1::ECPlayer1():
ECActor(0)
{
    m_ePlayerState=PLAYER_MAX_STATE;
    setPlayerCaught(false);
    m_pCommonDataPtr = NULL;
    m_bGodMode=false;
    m_fGodModeElapseTime = 0.0f;
    
    m_iszHeadIconList[0]=m_iszHeadIconList[1]=0;
    m_iHeadIconCntr=0;
    m_fDigElapseTime=0.0f;
    m_fElectrifingElapseTime=0.0f;
    m_pTeaserEnemyPtr=NULL;
    m_fPlayerHurtTime=0.0f;
    m_pPrevWalkedTilePtr=NULL;
    m_nTapKitCollected=0;
}

ECPlayer1::~ECPlayer1()
{
    resetPlayer1();
}

void ECPlayer1::resetPlayer1()
{
    resetActor();
    m_ePlayerState=PLAYER_MAX_STATE;
    setPlayerState((EPLAYERSTATES)(IDLE_EAST+3));
    setDirection(3);

    m_iszHeadIconList[0]=m_iszHeadIconList[1]=0;
    m_iHeadIconCntr=0;
    m_fDigElapseTime=0.0f;
    m_fElectrifingElapseTime=0.0f;
    m_pTeaserEnemyPtr=NULL;
    m_fPlayerHurtTime=0.0f;
    m_pPrevWalkedTilePtr=NULL;
    m_nTapKitCollected=0;
}

void ECPlayer1::init(const char* filename, CTextureManager* texturemanager, ECMap* map, CommonData* commonData)
{
    m_pCommonDataPtr = commonData;
	initActor(ACTOR_VISIBLE_RADIUS*commonData->getPortingMultiplier(), ACTOR_INNER_RADIUS_PERCENTAGE, map);	//base class init function
	
//	loadTexture(texturemanager, filename);    
//	setClipx(0, 0, ITOX(64), ITOX(64));

    setTileOffsetx(ITOX(-60*commonData->getPortingMultiplier()), ITOX(-82*commonData->getPortingMultiplier()));
    
    m_cAnimSprite.load(texturemanager, getResourcePath("charachters/deetleman.txa"), NULL);
    m_cAnimSprite.setFrame(0, 0);
    m_cPitAnimSprite.load(texturemanager, getResourcePath("charachters/pit_animation.txa"), NULL);
    m_cPitAnimSprite.setFrame(0, 0);
//    m_cPlayer1Run.loadTexture(texturemanager, getResourcePath("charachters/player1_run.tx"));
//    m_cPlayer1Run.setClipx(0, 0, ITOX(64), ITOX(64));
//	m_cPlayerIcon.loadTexture(texturemanager, getResourcePath("tiles/ingame_icon.tx"));
    
    //appendHeadIcon(0);
    //appendHeadIcon(1);
    //set the occluder tiles
    appendOccluder(new vector2i(1, -1));
    appendOccluder(new vector2i(1, 0));
    
    gxFile file;
    if(map->getFileDescriptorPtr())
    {
        stFD* fd=map->getFileDescriptorPtr()->getFD(getResourcePath("charachters/ec_player1.anb"));
		file.OpenFileDescriptor(fd->fd, gxFile::FILE_r);
		file.Seek(fd->offset, SEEK_SET);
    }
    else
    {
        file.OpenFile(getResourcePath("charachters/ec_player1.anb"));
    }
    readANBFile(file);
    file.CloseFile();
    
    setPlayerCaught(false);
    setGodMode(false);
    setPlayerState((EPLAYERSTATES)(IDLE_EAST+3));
    setDirection(3);
    
#ifdef ENABLE_OPENFEINT
    m_iOGHurtCounter = 0;
    m_iOGMoneybagCounter = 0;
    m_iOGHoleInNonTreasureAreaCounter = 0;
#endif
}

vector2x ECPlayer1::snapForDigAnimation()
{
    vector2x center(getIamOnTile()->getCenterWorldx());
//    vector2x eastPt(center+getDirectionx(0)*((getMapPtr()->getOneTileDistance().x>>1)-ITOX(ACTOR_PIT_RADIUS>>1)));
//    vector2x northPt(center+getDirectionx(1)*((getMapPtr()->getOneTileDistance().y>>1)-ITOX(ACTOR_PIT_RADIUS>>1)));
//    vector2x westPt(center+getDirectionx(2)*((getMapPtr()->getOneTileDistance().x>>1)-ITOX(ACTOR_PIT_RADIUS>>1)));
//    vector2x southPt(center+getDirectionx(3)*((getMapPtr()->getOneTileDistance().y>>1)-ITOX(ACTOR_PIT_RADIUS>>1)));
    vector2x eastPt(center+getDirectionx(0)*(ITOX(ACTOR_PIT_RADIUS)));
    vector2x northPt(center+getDirectionx(1)*ITOX(ACTOR_PIT_RADIUS));
    vector2x westPt(center+getDirectionx(2)*ITOX(ACTOR_PIT_RADIUS));
    vector2x southPt(center+getDirectionx(3)*ITOX(ACTOR_PIT_RADIUS));
    
#if defined (LOG_DEBUB_VERBOSE)
    DEBUG_PRINT("prevPos = %d, %d, tileID=%d", XTOI(getPosition2x().x), XTOI(getPosition2x().y), getIamOnTile()->getTileID());
    DEBUG_PRINT("center = %d, %d", XTOI(center.x), XTOI(center.y));
    DEBUG_PRINT("eastPt = %d, %d", XTOI(eastPt.x), XTOI(eastPt.y));
    DEBUG_PRINT("northPt = %d, %d", XTOI(northPt.x), XTOI(northPt.y));
    DEBUG_PRINT("westPt = %d, %d", XTOI(westPt.x), XTOI(westPt.y));
    DEBUG_PRINT("southPt = %d, %d", XTOI(southPt.x), XTOI(southPt.y));
#endif
    
    switch (getDirection())
    {
        case 0:
            return westPt; 
            break;
        case 1:
            return southPt; 
            break;
        case 2:
            return eastPt; 
            break;
        case 3:
            return northPt; 
            break;
            
        default:
            break;
    }

    return eastPt;  //control should not reach here
}

void ECPlayer1::update(float dt)
{
    if(isPaused()) return;
    ECActor::update(dt);
    
    if (isGodMode())
    {
        m_fGodModeElapseTime+=dt;
        if (m_fGodModeElapseTime>=GOD_MODE_TIME)
        {
            setGodMode(false);
            m_pCommonDataPtr->stopEventSound(SND_EVENT_INGAME_DEETLEMAN_CAUGHT);
        }
    }
    stAnimCommand* animCommand=getActiveCommand();
    if(animCommand==NULL) return;
    
	UIHud* hud=getMapPtr()->getHUDPtr();
	vector2f force(hud->getDPad()->getForce());
//	vector2f abs_force(ABS(force.x), ABS(force.y));
    
	float force_mag=force.Length();
	float angle=hud->getDPad()->getAngle();
	
//    DEBUG_PRINT("force=%f", force_mag);
    
	vector2x xaxisx(getMapPtr()->getXAxisx());
	vector2x yaxisx(getMapPtr()->getYAxisx());
	
	ECActor* actor=this;
	ECTile* iamontile=actor->getIamOnTile();
	
    vector2x oldPos(actor->getPosition2x());
    vector2x newPos(oldPos);
    bool bPositionUpdated=false;
    

    stAnimState* animState=animCommand->getActiveState();
    int curFrame = animState->getCurrentFrame();//+0.5f;

    m_cAnimSprite.setFrame(m_ePlayerState, curFrame);
    
    bool bOtherActivities = (m_ePlayerState>=JUMP_EAST);
    
   int Multiplier = m_pCommonDataPtr->getPortingMultiplier();
    
    if(!bOtherActivities)
    {
        if(iamontile  && force_mag>DPAD_FORCE_THRESHOLD)
        {
            int speed;
            if ((hud->getDPad()->getState() == UIButtonx::BUTTON_PRESSED)&&force_mag>0.5f)
            {
               //gaurav0111
                //speed=FTOX(PLAYER_RUN*dt);
                speed=FTOX((PLAYER_NORMAL_WALK+(55*force_mag))*dt);
                setDirection((int)angle/90);
                setPlayerState((EPLAYERSTATES)(RUN_EAST+getDirection())); 
            }
            else
            {
                //gaurav0111

               speed=FTOX((PLAYER_NORMAL_WALK+(52.5f*force_mag))*dt);

                setDirection((int)angle/90);
                setPlayerState((EPLAYERSTATES)(WALK_EAST+getDirection())); 
            }
            
            vector2x axis(getDirectionx(getDirection()));            
            newPos=oldPos+axis*(speed*Multiplier);
            bPositionUpdated=true;
        }
        else
        {
            setPlayerState((EPLAYERSTATES)(IDLE_EAST+getDirection()));         
        }
    }
    else
    {
        switch (m_ePlayerState)
        {
            case JUMP_EAST:
            {
                float ratio=(m_cAnimSprite.getCurrentFrame()+1.0f)/(float)m_cAnimSprite.getFrameCount(m_ePlayerState);
                vector2x endPos(m_cJumpStartPos+vector2x(FX_ONE, 0)*FTOX(ACTOR_PIT_JUMP_RADIUS*2.2f*Multiplier));
                vector2x distanceToJump=endPos-m_cJumpStartPos;
                bPositionUpdated=true;
                newPos=m_cJumpStartPos+distanceToJump*FTOX(ratio);
                
#if defined (LOG_DEBUB_VERBOSE)
                DEBUG_PRINT("ratio=%f, dist= %d, %d", ratio, XTOI(distanceToJump.x), XTOI(distanceToJump.y));
#endif

            }
                break;
            case JUMP_NORTH:
            {
                float ratio=(m_cAnimSprite.getCurrentFrame()+1.0f)/(float)m_cAnimSprite.getFrameCount(m_ePlayerState);
                vector2x endPos(m_cJumpStartPos+vector2x(0, -FX_ONE)*FTOX(ACTOR_PIT_JUMP_RADIUS*2.2f*Multiplier));
                vector2x distanceToJump=endPos-m_cJumpStartPos;
                bPositionUpdated=true;
                newPos=m_cJumpStartPos+distanceToJump*FTOX(ratio);
#if defined (LOG_DEBUB_VERBOSE)
                DEBUG_PRINT("ratio=%f, dist= %d, %d", ratio, XTOI(distanceToJump.x), XTOI(distanceToJump.y));
#endif
            }
                break;
            case JUMP_WEST:
            {
                float ratio=(m_cAnimSprite.getCurrentFrame()+1.0f)/(float)m_cAnimSprite.getFrameCount(m_ePlayerState);
                vector2x endPos(m_cJumpStartPos+vector2x(-FX_ONE, 0)*FTOX(ACTOR_PIT_JUMP_RADIUS*2.2f*Multiplier));
                vector2x distanceToJump=endPos-m_cJumpStartPos;
                bPositionUpdated=true;
                newPos=m_cJumpStartPos+distanceToJump*FTOX(ratio);
#if defined (LOG_DEBUB_VERBOSE)
                DEBUG_PRINT("ratio=%f, dist= %d, %d", ratio, XTOI(distanceToJump.x), XTOI(distanceToJump.y));
#endif
            }
                break;
            case JUMP_SOUTH:
            {
                float ratio=(m_cAnimSprite.getCurrentFrame()+1.0f)/(float)m_cAnimSprite.getFrameCount(m_ePlayerState);
                vector2x endPos(m_cJumpStartPos+vector2x(0, FX_ONE)*FTOX(ACTOR_PIT_JUMP_RADIUS*2.2f*Multiplier));
                vector2x distanceToJump=endPos-m_cJumpStartPos;
                bPositionUpdated=true;
                newPos=m_cJumpStartPos+distanceToJump*FTOX(ratio);
#if defined (LOG_DEBUB_VERBOSE)
                DEBUG_PRINT("ratio=%f, dist= %d, %d", ratio, XTOI(distanceToJump.x), XTOI(distanceToJump.y));
#endif
            }
                break;
            case HURT_EAST:
            case HURT_NORTH:
            case HURT_WEST:
            case HURT_SOUTH:
                m_fPlayerHurtTime+=dt;
                break;
            case DIG_EAST:
            case DIG_NORTH:
            case DIG_WEST:
            case DIG_SOUTH:
            {
                m_fDigElapseTime+=dt;
                m_cPitAnimSprite.setFrame(0, (int)m_cAnimSprite.getCurrentFrame());
                //DEBUG_PRINT("pit current frame = %d", (int)m_cAnimSprite.getCurrentFrame());
                bPositionUpdated=true;
                newPos = snapForDigAnimation();
                //DEBUG_PRINT("snappedPos = %d, %d",newPos.x,newPos.y);
            }    
                break;
                
            case DIG_SPARK_EAST:
            case DIG_SPARK_NORTH:
            case DIG_SPARK_WEST:
            case DIG_SPARK_SOUTH:
            {
                bPositionUpdated=true;
                newPos=snapForDigAnimation();
            }
                break;
            case HIT_EAST:
            case HIT_NORTH:
            case HIT_WEST:
            case HIT_SOUTH:                
                break;
            
                
            case ELECTROCUTE_EAST:
            case ELECTROCUTE_NORTH:
            case ELECTROCUTE_WEST:
            case ELECTROCUTE_SOUTH:
            {
                m_fDigElapseTime=0.0f;
                m_fElectrifingElapseTime+=dt;
            }
                break;

            default:
                break;
        }
    }
    

//    bool bCollisionOccuredDuringThisUpdate=false;
    int collision_check_cntr=5;
    //bool bCollision=true;
    while(collision_check_cntr--)
    {
        oldPos=newPos;
        //DEBUG_PRINT("newPos(%f, %f)", newPosf.x, newPosf.y);
        vector2x avgPos;
        int cnt=0;
        
        int row0=iamontile->getTileID()/getMapPtr()->getCol();
        int col0=iamontile->getTileID()%getMapPtr()->getCol();
        int min_row=row0-1;
        int min_col=col0-1;
        int max_row=row0+1;
        int max_col=col0+1;
        
        min_row=(min_row<0)?0:min_row;
        min_col=(min_col<0)?0:min_col;
        max_row=(max_row>=getMapPtr()->getRow())?getMapPtr()->getRow()-1:max_row;
        max_col=(max_col>=getMapPtr()->getCol())?getMapPtr()->getCol()-1:max_col;        
        
        for (int y = min_row; y <= max_row; y++)
        {
            for (int x = min_col; x <= max_col; x++)
            {
                ECTile* tile=getMapPtr()->getTile(y*getMapPtr()->getCol()+x);
                if(tile->isWalkable()) continue;
                CircleCollider* collider=tile->getCircleCollider();
                if(!collider)
                {
                    //circle-rectangle collision
                    vector2x left(tile->getLeftWorld());
                    vector2x right(tile->getRightWorld());
                    vector2x top(tile->getTopWorld());
                    vector2x bottom(tile->getBottomWorld());
                    
                    //check for penitration
                    bool bPenitration=true;
                    if(newPos.x<left.x || newPos.y<top.y || newPos.x>right.x || newPos.y>bottom.y)
                        bPenitration=false;
                    
//                    if(bPenitration)
//                    {
//                        DEBUG_PRINT("=======================PENITRATION OCCURED=================");
//                    }

                    vector2x closestPt[4];
                    
                    closestPt[0]=(util::closestPointOnLine(newPos, left, top));
                    closestPt[1]=(util::closestPointOnLine(newPos, top, right));
                    closestPt[2]=(util::closestPointOnLine(newPos, right, bottom));
                    closestPt[3]=(util::closestPointOnLine(newPos, bottom, left));
                    
                    //for debug
//                    tile->debug_closestPtf[0].set(XTOF(closestPt[0].x), XTOF(closestPt[0].y));
//                    tile->debug_closestPtf[1].set(XTOF(closestPt[1].x), XTOF(closestPt[1].y));
//                    tile->debug_closestPtf[2].set(XTOF(closestPt[2].x), XTOF(closestPt[2].y));
//                    tile->debug_closestPtf[3].set(XTOF(closestPt[3].x), XTOF(closestPt[3].y));
                    //

                    //which one is the closest
                    int closest_length=GX_MAX_INT;
                    int closest_index=-1;
                    
                    for (int l=0; l<4; l++)
                    {
                        vector2x diff(newPos-closestPt[l]);
                        __int64_t length=diff.lengthSquaredx();
                        if(length<closest_length)
                        {
                            closest_length=length;
                            closest_index=l;
                            
                            if(bPenitration)
                            {
                                diff=-diff;
                            }

                            if(closest_length<=m_xActorRadiusSq)
                            {
                                //collision occured
                                diff.normalizex();
                                float val=(ACTOR_COLLISION_RADIUS*m_pCommonDataPtr->getPortingMultiplier())+0.1f;
                                vector2x calc_Pos(closestPt[closest_index] + (diff*val));
                                avgPos+=calc_Pos;
                                cnt++;
                                //DEBUG_PRINT("(%d), closestPt(%f, %f), calc_pos(%f, %f)", l, closestPtf[closest_index].x, closestPtf[closest_index].y, calc_Pos.x, calc_Pos.y);
                            }
                        }
                    }//for
                    //~circle-rectangle collision
                }
                else
                {
                    vector2x center(tile->getCenterWorldx());
                    vector2x cpos(center.x+ITOX(collider->getOffsetX()), center.y+ITOX(collider->getOffsetY()));
                    
                    vector2x diff(newPos-cpos);
                    __int64_t length=diff.lengthSquaredx();
                    int check_val=(ACTOR_COLLISION_RADIUS*m_pCommonDataPtr->getPortingMultiplier())+collider->getRadius();
                    if(length<=ITOX((check_val*check_val)))
                    {
                        diff.normalizex();
                        float val=(ACTOR_COLLISION_RADIUS*m_pCommonDataPtr->getPortingMultiplier())+collider->getRadius()+0.1f;
                        vector2x calc_Pos(cpos + diff*val);
                        avgPos+=calc_Pos;
                        cnt++;
                    }
                }
            }
        }
        //for entire map
        
        if(cnt)
        {
            avgPos.x=DIVX(avgPos.x,ITOX(cnt));
            avgPos.y=DIVX(avgPos.y,ITOX(cnt));
                       
            newPos=avgPos;	//this will cause the actor to come to a halt then move away, so i commented this line
            bPositionUpdated=true;
//            bCollisionOccuredDuringThisUpdate=true;
            //DEBUG_PRINT("collision occured %d, avgPos(%f, %f)", cnt, avgPosf.x, avgPosf.x);
        }
        else
        {
            break;  //break the loop
        }
    }//while

//    if(bCollisionOccuredDuringThisUpdate)
//    {
//        if(m_ePlayerState>=JUMP_EAST && m_ePlayerState<=JUMP_WEST)
//        {
//            vector2x diff=newPos-m_cJumpStartPos;
//            int temp=diff.x;
//            diff.x=-diff.y;
//            diff.y=temp;
//            diff.normalizex();
//            newPos=m_cJumpStartPos-diff*(getMapPtr()->getOneTileDistance().x>>1);
//            setPlayerState((EPLAYERSTATES)(IDLE_EAST+getDirection()));
//        }
//    }
    
    //update hud icons
    if(getIamOnTile()->isTileFlag(MAP_PIT))
    {
        m_pCommonDataPtr->setShowShovelBtnOnHUD(false);
        
        vector2x center(getIamOnTile()->getCenterWorldx());
        vector2x diff(center-getPosition2x());
        if(diff.lengthx()<FTOX(ACTOR_PIT_JUMP_RADIUS*m_pCommonDataPtr->getPortingMultiplier()) && (getPlayerState()<JUMP_EAST || getPlayerState()>JUMP_SOUTH))
        {
            m_cJumpStartPos=getPosition2x();
            setPlayerState((EPLAYERSTATES)(JUMP_EAST+getDirection()));
#if defined (LOG_DEBUB_VERBOSE)
            DEBUG_PRINT("apply jump");
#endif
        }
    }
    //
    
//    //took bomb from bomb pile
//    if(getIamOnTile()->isFlag(XTRA_MAP_BOMBPILE_ON_THIS_TILE))
//    {
//        //reached at bomb pile tile
//        int nBombOnPile=getIamOnTile()->getUserDefined(TILE_USERDEFINED_INDEX_NOOF_BOMBS_ON_PILE);
//        int val=m_pCommonDataPtr->getPlayerData()->getBatteryCollected();
//        m_pCommonDataPtr->getPlayerData()->setBombCollected(val+nBombOnPile);
//        getIamOnTile()->setUserDefined(0, TILE_USERDEFINED_INDEX_NOOF_BOMBS_ON_PILE);
//    }
//    //
    
    if(bPositionUpdated)
    {
        //assign the newpos, row & col
        vector2x onetileDistanceInWorld(getMapPtr()->getOneTileDistance());
        int row=XTOI(newPos.y)/XTOI(onetileDistanceInWorld.y);
        int col=XTOI(newPos.x)/XTOI(onetileDistanceInWorld.x);

        if(row>=0 && col>=0 && newPos.x>=0 && newPos.y>=0 && row<getMapPtr()->getRow() && col<getMapPtr()->getCol())
        {
            ECTile* prevTile=actor->getIamOnTile();
            actor->setPositionx(newPos.x, newPos.y, 0);
            ECTile* tile=getMapPtr()->getTile(row*getMapPtr()->getCol()+col);
            actor->setIamOnTile(tile);
            actor->getIamOnTile()->setUserDefined(getDirection(), TILE_USERDEFINED_INDEX_DIRECTION);
            if(prevTile!=tile)
            {m_pPrevWalkedTilePtr=prevTile;}
        }
    }
}

bool ECPlayer1::circleCollision(ECActor* actor, ECTile* tile, vector2x& newPos, int& row, int& col)
{
    return false;
}

void ECPlayer1::render()
{	
    stAnimCommand* animCommand=getActiveCommand();
    if(animCommand==NULL) return;

    vector2x pos(getPosition2x());
	matrix4x4x* tm=getMapPtr()->getMapTMx();
	vector2x iniso((*tm) * pos);

    m_cAnimSprite.setColorx(FX_ONE, FX_ONE, FX_ONE);
    if(isGodMode())
    {
        float val=m_fGodModeElapseTime/GOD_MODE_TIME;
        m_cAnimSprite.setColorx(FX_ONE, FTOX(val), FTOX(val));
    }
    int Multiplier = m_pCommonDataPtr->getPortingMultiplier();
    //draw the pit animation
    switch (m_ePlayerState)
    {
        case DIG_EAST:
        case DIG_NORTH:
        case DIG_WEST:
        case DIG_SOUTH:
        {
            vector2x pos=getIamOnTile()->getPositionx();
            m_cPitAnimSprite.setPositionx(pos.x, pos.y, 0);
            m_cPitAnimSprite.draw();
            

            
            vector2f fCenter(XTOF(getPositionISOx().x), XTOF(getPositionISOx().y));
            float width=40.0f*Multiplier;
            float height=4.0f*Multiplier;
            float startx=fCenter.x-width*0.5f;
            float starty=fCenter.y-(46.0f*Multiplier)-width*0.5f;        
            
            float val= getDigElapseTime();
            float ratio = (float)val/PLAYER_DIG_TIME;
            if(ratio>=1.0f) ratio =1.0f;
           // printf("\n val =%f   ratio =%f     width =%f",val,ratio,width*ratio);
            glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
            DEBUG_DRAW_FILL_RECTf(startx, starty, (ratio)*width, height);        
            
            DEBUG_DRAW_RECT(startx*65536.0f, starty*65536.0f, width*65536.0f, height*65536.0f);
            if (Multiplier==2)
                DEBUG_DRAW_RECT((startx-1)*65536.0f, (starty-1)*65536.0f, (width+2)*65536.0f, (height+2)*65536.0f);
        }
            break;
            
        case HURT_EAST:
        case HURT_NORTH:
        case HURT_WEST:
        case HURT_SOUTH:
        {
                       //int scale=FX_ONE + (FTOX(offScale)>>1);
            // m_cPickablesSprite.setScale(scale, scale, scale);
            vector2f fCenter(XTOF(getIamOnTile()->getCenterx().x), XTOF(getIamOnTile()->getCenterx().y));
            float width=40.0f*Multiplier;
            float height=4.0f*Multiplier;
            float startx=fCenter.x-width*0.5f;
            float starty=fCenter.y-(55.0f*Multiplier)-width*0.5f;        
            
            float val=PLAYER_RECOVERY_TIME-m_fPlayerHurtTime;
            float ratio = (float)val/PLAYER_RECOVERY_TIME;
            //printf("\nval =%f   ratio =%f     width =%f",val,ratio,width*ratio);
            glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
            DEBUG_DRAW_FILL_RECTf(startx, starty, (ratio)*width, height);        
            
            DEBUG_DRAW_RECT(startx*65536.0f, starty*65536.0f, width*65536.0f, height*65536.0f);
            if (Multiplier==2)
                DEBUG_DRAW_RECT((startx-1)*65536.0f, (starty-1)*65536.0f, (width+2)*65536.0f, (height+2)*65536.0f);
        }
            break;
            
        default:
            break;
    }
    //
    
    
    //render player
	glPushMatrix();
		glTranslatex(FX12TO16(m_fxTileOffset.x), FX12TO16(m_fxTileOffset.y), 0);
        m_cAnimSprite.draw(&iniso);

    //head icons
    int clips[4]={
        (int)ECActionBubble::coin_head0,    //0treasure
        ECActionBubble::exclamation_head0,  //1alert
        ECActionBubble::exclamation_head0,  //2bomb
        ECActionBubble::tapkit_1_head0,     //3tapkit
    };
    int offsets[][2]={
        { 61, -24 },
        { 64, -24 },
        { 61, -24 },
        { 63, -24 },
    };
    int dir=getDirection();
    glTranslatef(offsets[dir][0]*Multiplier, 20*Multiplier, 0);
    for(int x=0;x<m_iHeadIconCntr;x++)
    {
        int val=m_iszHeadIconList[x];
        glTranslatef(0, offsets[dir][1]*Multiplier, 0);
        m_pCommonDataPtr->getIngameIconGUIPtr()->getComponent(clips[val])->draw(&iniso);
//        m_cPlayerIcon.setClipx(ITOX(clips[val][0]), ITOX(clips[val][1]), ITOX(clips[val][2]), ITOX(clips[val][3]));
//        m_cPlayerIcon.draw(&iniso);
    }
    //
    
//    if(getIamOnTile()->isFlag(XTRA_MAP_ACTOR_WILL_HIDDE_UNDER_THIS_TILE))
//    {
//        glTranslatef(0, -60, 0);
//        m_cPlayerIcon.setClipx(ITOX(clips[3][0]), ITOX(clips[3][1]), ITOX(clips[3][2]), ITOX(clips[3][3]));
//        m_cPlayerIcon.draw(&iniso);
//    }
    
    glPopMatrix();
    //
    
    
//    //draw occluder
//    int hover_row=getIamOnTile()->getTileID()/getMapPtr()->getCol();
//    int hover_col=getIamOnTile()->getTileID()%getMapPtr()->getCol();
//    for(int n=0;n<getOccluderCount();n++)
//    {
//        vector2i* actor_occ_offset=getOccluder(n);
//        int occ_r = actor_occ_offset->y + hover_row;
//        int occ_c = actor_occ_offset->x + hover_col;
//        int abs_tile_id = occ_r * getMapPtr()->getCol() + occ_c;
//        if (abs_tile_id >= getMapPtr()->getCol() * getMapPtr()->getRow() || abs_tile_id < 0 || occ_r>=getMapPtr()->getRow() || occ_c>=getMapPtr()->getCol()) continue;
//        
//        //chk if the abs_tile_id is overlapped by any occluder
//        ECTile* abs_tile=getMapPtr()->getTile(abs_tile_id);
//        glColor4f(0, 1, 0, 1);
//        abs_tile->drawDiamond();
//        Occluder* abs_occluder=abs_tile->getOccluderOnLayer(2);
//        if(!abs_occluder) continue;
//        
//        glColor4f(1, 0, 0, 1);
//        abs_tile->drawDiamond();
////        for(int s=0;s<abs_occluder->getCount();s++)
////        {
////            ECTile* sec_tile=m_pszTileList[abs_occluder->getOccluder(s)];
////            ECLayer* sec_layer=sec_tile->getLayerPtr(2);
////            if(!sec_layer) continue;
////            
////            //if(!sec_layer->isFlag(ECLayer::LAYER_RENDERED))
////            if(sec_tile->getTileID()>tile->getTileID())
////            {
////                bRenderActorOnThisTile=false;
////                sec_tile->appendSecondryActorOnThisTile(actor);
////                //DEBUG_PRINT("occlusion %d", sec_tile->getSecondryActorOnThisTileCount());
////                break;
////            }
////        }
//    }
//    //

    
//////#ifdef GL_DEBUG
//    glPushMatrix();
//    glMultMatrixx(tm->getMatrix());
//    drawCirclex(FX12TO16(pos.x), FX12TO16(pos.y));
//    glPopMatrix();
//    getIamOnTile()->drawDiamond();
//    
//    vector2x center(getIamOnTile()->getCenterWorldx());
//    vector2x snapPoints[]={
////        /*(*tm) * */vector2x(center+getDirectionx(0)*((getMapPtr()->getOneTileDistance().x>>1)-ITOX(ACTOR_PIT_RADIUS>>1))),
////        /*(*tm) * */vector2x(center+getDirectionx(1)*((getMapPtr()->getOneTileDistance().y>>1)-ITOX(ACTOR_PIT_RADIUS>>1))),
////        /*(*tm) * */vector2x(center+getDirectionx(2)*((getMapPtr()->getOneTileDistance().x>>1)-ITOX(ACTOR_PIT_RADIUS>>1))),
////        /*(*tm) * */vector2x(center+getDirectionx(3)*((getMapPtr()->getOneTileDistance().y>>1)-ITOX(ACTOR_PIT_RADIUS>>1)))
//        /*(*tm) * */vector2x(center+getDirectionx(0)*ITOX(ACTOR_PIT_RADIUS)),
//        /*(*tm) * */vector2x(center+getDirectionx(1)*ITOX(ACTOR_PIT_RADIUS)),
//        /*(*tm) * */vector2x(center+getDirectionx(2)*ITOX(ACTOR_PIT_RADIUS)),
//        /*(*tm) * */vector2x(center+getDirectionx(3)*ITOX(ACTOR_PIT_RADIUS))
//    };
//
//    for(int x=0;x<4;x++)
//    {   
//        glColor4f(1, 0, 0, 1);
//        glPushMatrix();
//            glMultMatrixx(tm->getMatrix());
//            glTranslatex(FX12TO16(snapPoints[x].x), FX12TO16(snapPoints[x].y), 0);
//            DEBUG_DRAW_LINE(-5*65536, 0, 5*65536, 0);
//            DEBUG_DRAW_LINE(0, -5*65536, 0, 5*65536);
//        glPopMatrix();	
//    }
////
////    
//////#endif
//	glColor4f(1, 1, 0, 1);
//	glPushMatrix();
//		//glMultMatrixx(m);
//		glTranslatex(FX12TO16(iniso.x), FX12TO16(iniso.y), 0);
//		DEBUG_DRAW_LINE(-5*65536, 0, 5*65536, 0);
//		DEBUG_DRAW_LINE(0, -5*65536, 0, 5*65536);
//	glPopMatrix();	
    
//	matrix4x4x* maptm=getMapPtr()->getMapTMx();
//    glColor4f(1, 0, 0, 1);
//    glPushMatrix();
//    glMultMatrixx(maptm->getMatrix());
//    
//    	glDisable(GL_TEXTURE_2D);
//    glEnableClientState(GL_VERTEX_ARRAY);
//	//glEnableClientState(GL_COLOR_ARRAY);
//	
//	glDisable(GL_TEXTURE_2D);
//	glVertexPointer(2, GL_FLOAT, 0, m_fszFOVArray);
//	//glColorPointer(4, GL_UNSIGNED_BYTE, 0, m_pvszFOVclrLst);
//	//glPushMatrix();
//	//glTranslatex(x, y, 0);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//	//glPopMatrix();
//	
//	//glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_VERTEX_ARRAY);
//
//    glPopMatrix();

}

bool ECPlayer1::setPlayerState(EPLAYERSTATES ePlayerState)
{
	if(m_ePlayerState == ePlayerState)
		return false;
	return(setNextCommandID(ePlayerState));
}

void ECPlayer1::callback_transitToNextState(int stateIndex)
{
    switch (m_ePlayerState)
    {
        case DIG_EAST:
        case DIG_NORTH:
        case DIG_WEST:
        case DIG_SOUTH:
         //   m_fDigElapseTime=0.0f;  //will be called only for the first state
            break;
    }
}

void ECPlayer1::callback_transitToPrevState(int stateIndex)
{
}

void ECPlayer1::callback_commandCompleted(int commandId)
{    
#if defined (LOG_DEBUB_VERBOSE)
    DEBUG_PRINT("deetleman command completed");
#endif
    switch (m_ePlayerState)
    {
        case HIT_EAST:
        case HIT_NORTH:
        case HIT_WEST:
        case HIT_SOUTH:
        {
            if(isPlayerCaught() || isGodMode())
            {
                setPlayerState((EPLAYERSTATES)(IDLE_EAST+getDirection()));
                break;
            }
            
            bool bPlayHurt=false;
            stLinkNode<ECActor*>* node= m_pMapPtr->getActorList()->getHead();
            while(node)
            {
                ECActor* actor=node->getData();
                if(actor==this)
                {
                    node=node->getNext();
                    continue;
                }
                
                //chk if the enemy is near the attack radius
                ECEnemyBase* enemybase=(ECEnemyBase*)actor;
                vector2x diff(enemybase->getPosition2x()-getPosition2x());
                if(enemybase->getEnemyState()!=ECEnemyBase::STATE_HURT && diff.lengthx()<ITOX(PLAYER_ATTACK_RADIUS))
                {
                    //
                    int fov=60;
                    int angle=(getDirection())*90;
                    angle%=360;
                    int min=angle-fov;
                    int max=angle+fov;
                    max%=360;
                    if(min<0)min=min+360;
                    
                    float x1=XTOF(m_xVisibleRadius)*cos(DEG2RAD((float)(360-min)));
                    float y1=XTOF(m_xVisibleRadius)*sin(DEG2RAD((float)(360-min)));
                    float x2=XTOF(m_xVisibleRadius)*cos(DEG2RAD((float)(360-max)));
                    float y2=XTOF(m_xVisibleRadius)*sin(DEG2RAD((float)(360-max)));
                    
                    //vector2x actorPos(getPosition2x());    
                    float mid_pointX=x1+(x2-x1)*0.5f;
                    float mid_pointY=y1+(y2-y1)*0.5f;
                    
//                    m_fszFOVArray[0]=XTOF(getPosition2x().x);
//                    m_fszFOVArray[1]=XTOF(getPosition2x().y);
//
//                    m_fszFOVArray[2]=m_fszFOVArray[0]+x1;
//                    m_fszFOVArray[3]=m_fszFOVArray[1]+y1;
//
//                    m_fszFOVArray[4]=m_fszFOVArray[0]+x2;
//                    m_fszFOVArray[5]=m_fszFOVArray[1]+y2;

                    
                    vector2x fovdirection;
                    fovdirection.setx(FTOX(mid_pointX), FTOX(mid_pointY));
                    fovdirection.normalizex();
                    //
                    if(isPointInsideCone(enemybase->getPosition2x(), getPosition2x(), fovdirection, ITOX((fov>>1))))
                    {
                        enemybase->forceActiveCmdToCompletetion();
                        enemybase->setEnemyState(ECEnemyBase::STATE_HURT);                        
#if defined (LOG_DEBUB_VERBOSE)
                        DEBUG_PRINT("i hit one amp.....");
#endif
                        
#ifdef ENABLE_OPENFEINT                    
                        m_iOGHurtCounter++;
                        if(m_iOGHurtCounter == 3)
                        {
                            openFeintWrapper::UnlockAchievement(ACHIEVEMENT_ID_MR_VENGEANCE);
                        }
#endif
                        //////
                        //int dir[]={2,3,0,1};
                        
                        //
                        vector2x playerPos=getPosition2x();
                        vector2x enemyPos=enemybase->getPosition2x();
                        vector2x diffVec=playerPos-enemyPos;
                        diffVec.normalizex();
                        
                        switch(getDirection())
                        {
                            case 0:    //E/W
                            case 2:
                            {
                                vector2x refVec=getDirectionx(1);
                                __int64_t angle = (refVec.dotx(diffVec));
                                float anglef = XTOF(angle);
                                anglef = CLAMP(anglef, -1, 1);           
                                anglef = acos(anglef);
                                anglef = (diffVec.x<0)?360.0f-anglef:anglef;
                                
                                //                     System.out.println("amp e/w angle "+angle);
                                if(anglef>180)
                                {
                                    enemybase->setDirection(2);
                                    //m_iDirectionID=2;
                                }
                                else
                                {
                                    enemybase->setDirection(0);
                                    //m_iDirectionID=0;
                                }
                            }
                                break;
                                
                            case 1:    //N/S
                            case 3:
                            {
                                vector2x refVec=getDirectionx(0);
                                __int64_t angle = (refVec.dotx(diffVec));
                                float anglef = XTOF(angle);
                                anglef = CLAMP(anglef, -1, 1);           
                                anglef = acos(anglef);
                                anglef = (diffVec.y<0)?360.0f-angle:angle;
                                
                                //                    System.out.println("diff "+(diffVec.x/4096.0f) +" "+(diffVec.y/4096.0f)) ;
                                //                     System.out.println("amp n/s angle "+angle);
                                
                                if(anglef>180)
                                {
                                    enemybase->setDirection(1);
                                    //m_iDirectionID=1;
                                }
                                else
                                {
                                    enemybase->setDirection(3);
                                    //m_iDirectionID=3;
                                }
                            }
                                break;
                        }
                        //
                        //m_pPlayer1.setPlayerAnimState((EPLAYERSTATES.ELECTROCUTE_EAST+m_iDirectionID));
                        /////////////
//                        int dir[]={2, 3, 0, 1};
//                        enemybase->setDirection(dir[getDirection()]);
                        bPlayHurt=true;
                    }
                }
                node=node->getNext();
            }

            if(bPlayHurt)
                m_pCommonDataPtr->playEventSound(SND_EVENT_INGAME_HIT, getPosition2x());
            else
                m_pCommonDataPtr->playEventSound(SND_EVENT_INGAME_WHIP, getPosition2x());
            
            setPlayerState((EPLAYERSTATES)(IDLE_EAST+getDirection()));
        }
            break;
            
        case DIG_SPARK_EAST:
        case DIG_SPARK_NORTH:
        case DIG_SPARK_WEST:
        case DIG_SPARK_SOUTH:
        {
            m_pCommonDataPtr->playEventSound(SND_EVENT_INGAME_DIG_ON_ROCK, getPosition2x());
            setPlayerState((EPLAYERSTATES)(IDLE_EAST+getDirection()));
        }
            break;
    }
}

void ECPlayer1::callback_loopCompleted(int stateIndex)
{
    vector2i actualWndSz(m_pCommonDataPtr->getIngameHUDPtr()->getExtendedGUI()->getActualGUISize()); 
    switch (m_ePlayerState)
    {
        case RUN_EAST:
        case RUN_NORTH:
        case RUN_WEST:
        case RUN_SOUTH:
        {
#if defined (ENABLE_EVENTSOUND)
            if(!m_pCommonDataPtr->getEventSound(SND_EVENT_INGAME_DEETLEMAN_RUN)->isPlaying())
                m_pCommonDataPtr->playEventSound(SND_EVENT_INGAME_DEETLEMAN_RUN, getPosition2x());
#endif
        }
            break;
            
        case DIG_EAST:
        case DIG_NORTH:
        case DIG_WEST:
        case DIG_SOUTH:
        {
            if(m_fDigElapseTime>PLAYER_DIG_TIME)   //4 seconds
            {
                m_fDigElapseTime=0.0f;
                if(!forceActiveHoldOrLoopCmdToNextState())  //will return false for the last state
                {
//                    //chk if this pit contains bomb or treasure
//                    if(getIamOnTile()->isTileFlag(MAP_BOMB))
//                    {
//                        int val=Scene::getCommonData()->getPlayerData()->getBombCollected();
//                        Scene::getCommonData()->getPlayerData()->setBombCollected(val+1);
//                        getIamOnTile()->resetTileFlag(MAP_BOMB);
//                        ComponentBase* btn_bomb=Scene::getCommonData()->getIngameHUDPtr()->getExtendedGUI()->getComponent(UIHud::bomb_hud0);
//                        btn_bomb->show();
//                        btn_bomb->enable();
//                    
//                        //show bomb hud sparkle animation
//                         ECCollectionSparkle* collectionSparkle= new ECCollectionSparkle(ECCollectionSparkle::HIDDEN_BOMB, getIamOnTile(), getIamOnTile()->getCenterx());
//                        m_pCommonDataPtr->getEntityManager()->appendEntity(collectionSparkle);                        
//                        ////
//                    }
                    
                    if(getIamOnTile()->isTileFlag(MAP_TREASURE))
                    {
                        int retinaDisplayAdjustX;
                        if (m_pCommonDataPtr->getRetinaDisplay())
                        {
                            retinaDisplayAdjustX = 32;
                        }
                        else
                        {
                            retinaDisplayAdjustX = 0;
                        }
                        int deltaY=0;
                        if (m_pCommonDataPtr->isDontScaleGUI())
                        {
                            deltaY=((m_pCommonDataPtr->getIngameHUDPtr()->getExtendedGUI()->getActualGUISize().y-m_pCommonDataPtr->getScreenHeight())>>1);
                        }
                        matrix4x4x scale_matrix;
                        scale_matrix.setScale(FTOX(getMapPtr()->getMapScale()), FTOX(getMapPtr()->getMapScale()), FX_ONE);
                        matrix4x4x temp_matrix(scale_matrix * (*getMapPtr()->getMapTMx()));
                        vector2x iniso(temp_matrix * getIamOnTile()->getCenterWorldx());
                        vector2x startpos(iniso.x-FTOX(getMapPtr()->getMapOffsetX()), iniso.y-FTOX(getMapPtr()->getMapOffsetY()));
                                                
                        vector2x a = vector2x(XTOI(startpos.x),XTOI(startpos.y));
                        vector2x pos = ((Sprite2Dx*)m_pCommonDataPtr->getIngameHUDPtr()->getExtendedGUI()->getComponent(UIHud::coile0))->getPosition2x();
                        vector2x b = vector2x((actualWndSz.x>>1)+XTOI(pos.x)-retinaDisplayAdjustX,(actualWndSz.y>>1)+XTOI(pos.y)-deltaY);
                        
                        //
  
                        b = (*m_pCommonDataPtr->getIngameHUDPtr()->getExtendedGUI()) % b;
                        
                        ECBonus* bonus = new ECBonus();
                        bonus->init(5,ECCoins::COINS, a, b);
                        Scene::getCommonData()->getEntityManager()->appendEntity(bonus);
                        
                        //show treasure sparkle animation
                        ECCollectionSparkle* collectionSparkle= new ECCollectionSparkle(ECCollectionSparkle::HIDDEN_TREASURE, getIamOnTile(), getIamOnTile()->getCenterx());
                        m_pCommonDataPtr->getEntityManager()->appendEntity(collectionSparkle);
                        //
                        
                        Scene::getCommonData()->getPlayerData()->incrementPlayerData(CPlayerData::CASH_DATA, 50);
                        getIamOnTile()->resetTileFlag(MAP_TREASURE);
                        m_pCommonDataPtr->playEventSound(SND_EVENT_INGAME_TREASURE_FOUND, getPosition2x());
#ifdef ENABLE_OPENFEINT
                        m_iOGMoneybagCounter++;
                        if(m_iOGMoneybagCounter == LEVEL1_TOTAL_MONEY_BAGS)
                        {
                            openFeintWrapper::UnlockAchievement(ACHIEVEMENT_ID_TREASURE_COLLECTOR);
                        }
#endif
                    }
                    else
                    {
#ifdef ENABLE_OPENFEINT
                        //to check whether hole is in non treasure area
                        // to unlock achievement 'EXCAVATOR'
                        m_iOGHoleInNonTreasureAreaCounter++;
                        if(m_iOGHoleInNonTreasureAreaCounter >= 2)
                        {
                            openFeintWrapper::UnlockAchievement(ACHIEVEMENT_ID_EXCAVATOR);
                        }
#endif
                    }
                    
                    //show shovel regeneration
                    m_pCommonDataPtr->getIngameHUDPtr()->setShovelGenerated(false);
                    m_pCommonDataPtr->getIngameHUDPtr()->setShovelRegenerationScale(0.0f);
                    //
                    
                    setPlayerState((EPLAYERSTATES)(IDLE_EAST+getDirection()));
                    getIamOnTile()->setTileFlag(MAP_PIT);
                    getIamOnTile()->setUserDefined(PIT_AUTOFILL_TIME, TILE_USERDEFINED_INDEX_PITAUTOFILL);   //5000 millisec
                    m_pCommonDataPtr->stopEventSound(SND_EVENT_INGAME_DEETLEMAN_DIG);
                }
            }
        }
            break;
            
        case ELECTROCUTE_EAST:
        case ELECTROCUTE_NORTH:
        case ELECTROCUTE_WEST:
        case ELECTROCUTE_SOUTH:
        {
            if(m_fElectrifingElapseTime>PLAYER_ELECTRIFYING_TIME)
            {
                setPlayerCaught(true);
                forceActiveHoldOrLoopCmdToNextState();
                setPlayerState((EPLAYERSTATES)(IDLE_EAST+getDirection()));
                m_fElectrifingElapseTime=0.0f;
                
                if(m_pTeaserEnemyPtr)m_pTeaserEnemyPtr->forceActiveCmdToCompletetion();
                m_pTeaserEnemyPtr=NULL;
            }
        }
            break;
            
        case HURT_EAST:
        case HURT_NORTH:
        case HURT_WEST:
        case HURT_SOUTH:
        {
            if (m_fPlayerHurtTime>PLAYER_RECOVERY_TIME) 
            {
                m_fPlayerHurtTime=0;
                forceActiveCmdToCompletetion();
                setPlayerState((EPLAYERSTATES)(IDLE_EAST+getDirection()));
                m_pCommonDataPtr->stopEventSound(SND_EVENT_INGAME_DEETLEMAN_DISORIENT);
            }
        }
    }
}

void ECPlayer1::callback_holdReached()
{
    switch (m_ePlayerState)
    {
        case JUMP_EAST:
        case JUMP_NORTH:
        case JUMP_WEST:
        case JUMP_SOUTH:
        {
            forceActiveHoldOrLoopCmdToNextState();
            
            UIHud* hud=getMapPtr()->getHUDPtr();
            vector2f force(hud->getDPad()->getForce());
            float force_mag=force.Length();

            if(force_mag>DPAD_FORCE_THRESHOLD)
            {
                setPlayerState((EPLAYERSTATES)(WALK_EAST+getDirection()));
            }
            else
            {
                setPlayerState((EPLAYERSTATES)(IDLE_EAST+getDirection()));
            }
        }
            break;
    }
}

void ECPlayer1::callback_commandChangedTo(int commandId)
{
    m_ePlayerState = (EPLAYERSTATES)commandId;
    
    switch (m_ePlayerState)
    {
        case HURT_EAST:
        case HURT_NORTH:
        case HURT_WEST:
        case HURT_SOUTH:
            m_fPlayerHurtTime=0.0f;
            break;
    }
}


