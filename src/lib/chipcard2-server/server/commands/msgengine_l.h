/***************************************************************************
 $RCSfile$
                             -------------------
    cvs         : $Id$
    begin       : Mon Mar 01 2004
    copyright   : (C) 2004 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/


#ifndef CHIPCARD_CLIENT_MSGENGINE_L_H
#define CHIPCARD_CLIENT_MSGENGINE_L_H

#include <gwenhywfar/msgengine.h>

/**
 * @file msgengine_l.h
 *
 * This message engine implements a few new types:
 * <ul>
 *   <li>byte</li>
 *   <li>word (bigEndian="1")</li>
 *   <li>dword (bigEndian="1")</li>
 *   <li>bytes (size="-1") </li>
 *   <li>tlv (tlvType="BER"||"SIMPLE") </li>
 * </ul>
 */


GWEN_MSGENGINE *LC_MsgEngine_new();




#endif /* CHIPCARD_CLIENT_MSGENGINE_L_H */


