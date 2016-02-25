// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



#include <vector>

#include <WinSock2.h>
#include <windows.h>
#include <iostream>

#include "./netmessage/NetOutgoingMessage.h"
#include "./netmessage/NetIncomingMessage.h"


#include "Message.Base.h"
#include "Message.Pair.h"
#include "Message.Battle.ReliableOrdered.h"
#include "Message.Battle.Unreliable.h"
#include "Message.Battle.UnreliableSequenced.h"
#include "GlobalVariable.h"