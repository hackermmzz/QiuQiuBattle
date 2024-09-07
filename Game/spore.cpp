#include "spore.h"

Spore::Spore(const vec2 &curp,const vec2&dir,GLuint appearanceType)
{
    appearance=appearanceType;
    CurPos=curp;
    TarPos=CurPos+dir*SporeShootDistance;
    if(CheckIsOverEdge(CurPos))CurPos=GetValidPos(CurPos,dir,SporeShootDistance);
    if(CheckIsOverEdge(TarPos))TarPos=GetValidPos(CurPos,dir,SporeShootDistance);
    state=SPORE_SHOOT;
    shootTime=0;
}

void Spore::Shoot()
{
    if(state==SPORE_SHOOT){
        shootTime+=RenderTime;
        shootTime=min(shootTime,SporeShootTime);
        CurPos=CurPos.mix(TarPos,shootTime*1.0/SporeShootTime);
        if(shootTime==SporeShootTime)
            state=SPORE_STOP;
    }
}

Spore::~Spore()
{

}
