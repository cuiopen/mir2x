/*
 * =====================================================================================
 *
 *       Filename: monster.cpp
 *        Created: 08/31/2015 08:26:57 PM
 *  Last Modified: 06/14/2016 14:32:40
 *
 *    Description: 
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */
#include "monster.hpp"
#include <SDL2/SDL.h>

// static monster global info map
std::unordered_map<uint32_t, MonsterGInfo> Monster::s_MonsterGInfoMap;

Monster::Monster(uint32_t nMonsterID, uint32_t nUID, uint32_t nAddTime)
    : Creature(nUID, nAddTime)
    , m_MonsterID(nMonsterID)
    , m_LookIDN(0)
{}

Monster::~Monster()
{}

void Monster::Update()
{
    if(SDL_GetTicks() < m_UpdateTime + m_FrameDelay){ return; }

    // ok now it's time to update
    m_UpdateTime = SDL_GetTicks();
    uint32_t nFrameCount = FrameCount();
    if(nFrameCount){
        m_Frame = (m_Frame + 1) % nFrameCount;
    }
}

void Monster::Draw(int nViewX, int nViewY)
{
    // 0. check the validness of graphical resource
    //    please check it or all you get will be LookID = 0
    if(!ValidG()){ return; }

    // 1. ok draw it
    uint32_t nBaseKey = (LookID() << 12) + (m_State << 8) + (m_Direction << 5);
    uint32_t nKey0 = 0X00000000 + nBaseKey + m_Frame; // body
    uint32_t nKey1 = 0X01000000 + nBaseKey + m_Frame; // shadow

    int nDX0 = 0;
    int nDY0 = 0;
    int nDX1 = 0;
    int nDY1 = 0;

    extern PNGTexOffDBN *g_PNGTexOffDBN;
    auto pFrame0 = g_PNGTexOffDBN->Retrieve(nKey0, &nDX0, &nDY0);
    auto pFrame1 = g_PNGTexOffDBN->Retrieve(nKey1, &nDX1, &nDY1);

    extern SDLDevice *g_SDLDevice;
    g_SDLDevice->DrawTexture(pFrame1, m_X + nDX1 - nViewX, m_Y + nDY1 - nViewY);
    g_SDLDevice->DrawTexture(pFrame0, m_X + nDX0 - nViewX, m_Y + nDY0 - nViewY);
}