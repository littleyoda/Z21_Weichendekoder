/*
 * CmdSenderBase.cpp
 *
 *  Created on: 13.12.2016
 *      Author: sven
 */

#include "CmdSenderBase.h"

CmdSenderBase::CmdSenderBase() {
}

CmdSenderBase::~CmdSenderBase() {
}

void CmdSenderBase::sendSetTurnout(String id, String status) {
}

void CmdSenderBase::sendSetSensor(uint16_t id, uint8_t status) {
}

void CmdSenderBase::setRequestList(LinkedList<INotify::requestInfo*>* list) {
	requestList = list;
}
