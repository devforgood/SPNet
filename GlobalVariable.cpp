#include "StdAfx.h"
#include "GlobalVariable.h"
#include "Room.h"

GlobalVariable GlobalVariable::inst;

GlobalVariable::GlobalVariable()
{
	room_mgr_ = new RoomMgr();
}

GlobalVariable::~GlobalVariable()
{
	delete room_mgr_;
}
