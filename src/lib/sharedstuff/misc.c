/***************************************************************************
 $RCSfile$
                             -------------------
    cvs         : $Id$
    begin       : Tue Dec 23 2003
    copyright   : (C) 2003 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif


#include <gwenhywfar/debug.h>
#include <gwenhywfar/misc.h>
#include <gwenhywfar/directory.h>
#include <gwenhywfar/buffer.h>

#include <chipcard2/chipcard2.h>


#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>



LC_DRIVER_STATUS LC_DriverStatus_fromString(const char *s) {
  if (strcasecmp(s, "Down")==0)
    return LC_DriverStatusDown;
  else if (strcasecmp(s, "WaitForStart")==0)
    return LC_DriverStatusWaitForStart;
  else if (strcasecmp(s, "Started")==0)
    return LC_DriverStatusStarted;
  else if (strcasecmp(s, "Up")==0)
    return LC_DriverStatusUp;
  else if (strcasecmp(s, "Stopping")==0)
    return LC_DriverStatusStopping;
  else if (strcasecmp(s, "Aborted")==0)
    return LC_DriverStatusAborted;
  else if (strcasecmp(s, "Disabled")==0)
    return LC_DriverStatusDisabled;
  else
    return LC_DriverStatusUnknown;
}



const char *LC_DriverStatus_toString(LC_DRIVER_STATUS dst) {
  switch(dst) {
  case LC_DriverStatusDown:         return "Down";
  case LC_DriverStatusWaitForStart: return "WaitForStart";
  case LC_DriverStatusStarted:      return "Started";
  case LC_DriverStatusUp:           return "Up";
  case LC_DriverStatusStopping:     return "Stopping";
  case LC_DriverStatusAborted:      return "Aborted";
  case LC_DriverStatusDisabled:     return "Disabled";
  default:                          return "Unknown";
  }
}



LC_READER_STATUS LC_ReaderStatus_fromString(const char *s) {
  if (strcasecmp(s, "Down")==0)
    return LC_ReaderStatusDown;
  else if (strcasecmp(s, "WaitForStart")==0)
    return LC_ReaderStatusWaitForStart;
  else if (strcasecmp(s, "WaitForDriver")==0)
    return LC_ReaderStatusWaitForDriver;
  else if (strcasecmp(s, "WaitForReaderUp")==0)
    return LC_ReaderStatusWaitForReaderUp;
  else if (strcasecmp(s, "WaitForReaderDown")==0)
    return LC_ReaderStatusWaitForReaderDown;
  else if (strcasecmp(s, "Up")==0)
    return LC_ReaderStatusUp;
  else if (strcasecmp(s, "Aborted")==0)
    return LC_ReaderStatusAborted;
  else if (strcasecmp(s, "Disabled")==0)
    return LC_ReaderStatusDisabled;
  else
    return LC_ReaderStatusUnknown;
}



const char *LC_ReaderStatus_toString(LC_READER_STATUS rst) {
  switch(rst) {
  case LC_ReaderStatusDown:              return "Down";
  case LC_ReaderStatusWaitForStart:      return "WaitForStart";
  case LC_ReaderStatusWaitForDriver:     return "WaitForDriver";
  case LC_ReaderStatusWaitForReaderUp:   return "WaitForReaderUp";
  case LC_ReaderStatusWaitForReaderDown: return "WaitForReaderDown";
  case LC_ReaderStatusUp:                return "Up";
  case LC_ReaderStatusAborted:           return "Aborted";
  case LC_ReaderStatusDisabled:          return "Disabled";
  default:                               return "Unknown";
  }
}



GWEN_TYPE_UINT32 LC_ReaderFlags_fromDb(GWEN_DB_NODE *db, const char *name) {
  int i;
  const char *p;
  GWEN_TYPE_UINT32 flags=0;

  for (i=0; ; i++) {
    p=GWEN_DB_GetCharValue(db, name, i, 0);
    if (!p)
      break;
    if (strcasecmp(p, "keypad")==0)
      flags|=LC_READER_FLAGS_KEYPAD;
    else if (strcasecmp(p, "display")==0)
      flags|=LC_READER_FLAGS_DISPLAY;
    else if (strcasecmp(p, "noinfo")==0)
      flags|=LC_READER_FLAGS_NOINFO;
    else if (strcasecmp(p, "remote")==0)
      flags|=LC_READER_FLAGS_REMOTE;
    else if (strcasecmp(p, "auto")==0)
      flags|=LC_READER_FLAGS_AUTO;
    else if (strcasecmp(p, "suspended_checks")==0)
      flags|=LC_READER_FLAGS_SUSPENDED_CHECKS;
    else {
      DBG_WARN(0, "Unknown flag \"%s\", ignoring", p);
    }
  } /* for */

  return flags;
}



GWEN_TYPE_UINT32 LC_ReaderFlags_fromXml(GWEN_XMLNODE *node, const char *name){
  const char *p;
  GWEN_TYPE_UINT32 flags=0;
  GWEN_XMLNODE *n;

  n=GWEN_XMLNode_FindFirstTag(node, name, 0, 0);
  while(n) {
    GWEN_XMLNODE *nn;

    nn=GWEN_XMLNode_GetFirstData(n);
    if (nn) {
      p=GWEN_XMLNode_GetData(nn);
      assert(p);

      if (strcasecmp(p, "keypad")==0)
        flags|=LC_READER_FLAGS_KEYPAD;
      else if (strcasecmp(p, "display")==0)
        flags|=LC_READER_FLAGS_DISPLAY;
      else if (strcasecmp(p, "noinfo")==0)
        flags|=LC_READER_FLAGS_NOINFO;
      else if (strcasecmp(p, "remote")==0)
        flags|=LC_READER_FLAGS_REMOTE;
      else if (strcasecmp(p, "auto")==0)
        flags|=LC_READER_FLAGS_AUTO;
      else if (strcasecmp(p, "suspended_checks")==0)
        flags|=LC_READER_FLAGS_SUSPENDED_CHECKS;
      else {
        DBG_WARN(0, "Unknown flag \"%s\", ignoring", p);
      }
    }
    n=GWEN_XMLNode_FindNextTag(n, name, 0, 0);
  } /* while */

  return flags;
}



void LC_ReaderFlags_toDb(GWEN_DB_NODE *db,
                         const char *name,
                         GWEN_TYPE_UINT32 fl) {
  assert(db);
  assert(name);
  GWEN_DB_DeleteVar(db, name);
  if (fl & LC_READER_FLAGS_KEYPAD)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "keypad");
  if (fl & LC_READER_FLAGS_DISPLAY)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "display");
  if (fl & LC_READER_FLAGS_NOINFO)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "noinfo");
  if (fl & LC_READER_FLAGS_REMOTE)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "remote");
  if (fl & LC_READER_FLAGS_AUTO)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "auto");
}



GWEN_TYPE_UINT32 LC_NotifyFlags_fromDb(GWEN_DB_NODE *db, const char *name){
  const char *p;
  GWEN_TYPE_UINT32 flags=0;
  int i;

  for (i=0; ; i++) {
    p=GWEN_DB_GetCharValue(db, name, i, 0);
    if (!p)
      break;

    if (strcasecmp(p, "DriverStart")==0)
      flags|=LC_NOTIFY_FLAGS_DRIVER_START;
    else if (strcasecmp(p, "DriverUp")==0)
      flags|=LC_NOTIFY_FLAGS_DRIVER_UP;
    else if (strcasecmp(p, "DriverDown")==0)
      flags|=LC_NOTIFY_FLAGS_DRIVER_DOWN;
    else if (strcasecmp(p, "DriverError")==0)
      flags|=LC_NOTIFY_FLAGS_DRIVER_ERROR;

    else if (strcasecmp(p, "ReaderStart")==0)
      flags|=LC_NOTIFY_FLAGS_READER_START;
    else if (strcasecmp(p, "ReaderUp")==0)
      flags|=LC_NOTIFY_FLAGS_READER_UP;
    else if (strcasecmp(p, "ReaderDown")==0)
      flags|=LC_NOTIFY_FLAGS_READER_DOWN;
    else if (strcasecmp(p, "ReaderError")==0)
      flags|=LC_NOTIFY_FLAGS_READER_ERROR;

    else if (strcasecmp(p, "ServiceStart")==0)
      flags|=LC_NOTIFY_FLAGS_SERVICE_START;
    else if (strcasecmp(p, "ServiceUp")==0)
      flags|=LC_NOTIFY_FLAGS_SERVICE_UP;
    else if (strcasecmp(p, "ServiceDown")==0)
      flags|=LC_NOTIFY_FLAGS_SERVICE_DOWN;
    else if (strcasecmp(p, "ServiceError")==0)
      flags|=LC_NOTIFY_FLAGS_SERVICE_ERROR;

    else if (strcasecmp(p, "CardInserted")==0)
      flags|=LC_NOTIFY_FLAGS_CARD_INSERTED;
    else if (strcasecmp(p, "CardRemoved")==0)
      flags|=LC_NOTIFY_FLAGS_CARD_REMOVED;

    else if (strcasecmp(p, "ClientUp")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_UP;
    else if (strcasecmp(p, "ClientDown")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_DOWN;
    else if (strcasecmp(p, "ClientStartWait")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_STARTWAIT;
    else if (strcasecmp(p, "ClientStopWait")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_STOPWAIT;
    else if (strcasecmp(p, "ClientTakeCard")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_TAKECARD;
    else if (strcasecmp(p, "ClientGotCard")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_GOTCARD;

    else if (strcasecmp(p, "ClientCmdSend")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_CMDSEND;
    else if (strcasecmp(p, "ClientCmdRecv")==0)
      flags|=LC_NOTIFY_FLAGS_CLIENT_CMDRECV;

    else {
      DBG_WARN(0, "Unknown flag \"%s\", ignoring", p);
    }
  } /* for */

  return flags;
}



void LC_NotifyFlags_toDb(GWEN_DB_NODE *db,
                         const char *name,
                         GWEN_TYPE_UINT32 fl) {
  assert(db);
  assert(name);
  GWEN_DB_DeleteVar(db, name);
  if (fl & LC_NOTIFY_FLAGS_DRIVER_START)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "DriverStart");
  if (fl & LC_NOTIFY_FLAGS_DRIVER_UP)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "DriverUp");
  if (fl & LC_NOTIFY_FLAGS_DRIVER_DOWN)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "DriverDown");
  if (fl & LC_NOTIFY_FLAGS_DRIVER_ERROR)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "DriverError");

  if (fl & LC_NOTIFY_FLAGS_SERVICE_START)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ServiceStart");
  if (fl & LC_NOTIFY_FLAGS_SERVICE_UP)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ServiceUp");
  if (fl & LC_NOTIFY_FLAGS_SERVICE_DOWN)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ServiceDown");
  if (fl & LC_NOTIFY_FLAGS_SERVICE_ERROR)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ServiceError");

  if (fl & LC_NOTIFY_FLAGS_CARD_INSERTED)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "CardInserted");
  if (fl & LC_NOTIFY_FLAGS_CARD_REMOVED)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "CardRemoved");

  if (fl & LC_NOTIFY_FLAGS_CLIENT_UP)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientUp");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_DOWN)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientDown");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_STARTWAIT)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientStartWait");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_STOPWAIT)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientStopWait");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_TAKECARD)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientTakeCard");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_GOTCARD)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientGotCard");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_CMDSEND)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientCmdSend");
  if (fl & LC_NOTIFY_FLAGS_CLIENT_CMDRECV)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT,
                         name, "ClientCmdRecv");
}



LC_SERVICE_STATUS LC_ServiceStatus_fromString(const char *s) {
  if (strcasecmp(s, "Down")==0)
    return LC_ServiceStatusDown;
  else if (strcasecmp(s, "WaitForStart")==0)
    return LC_ServiceStatusWaitForStart;
  else if (strcasecmp(s, "Started")==0)
    return LC_ServiceStatusStarted;
  else if (strcasecmp(s, "Up")==0)
    return LC_ServiceStatusUp;
  else if (strcasecmp(s, "Stopping")==0)
    return LC_ServiceStatusStopping;
  else if (strcasecmp(s, "Aborted")==0)
    return LC_ServiceStatusAborted;
  else if (strcasecmp(s, "Disabled")==0)
    return LC_ServiceStatusDisabled;
  else
    return LC_ServiceStatusUnknown;
}



const char *LC_ServiceStatus_toString(LC_SERVICE_STATUS st) {
  switch(st) {
  case LC_ServiceStatusDown:         return "Down";
  case LC_ServiceStatusWaitForStart: return "WaitForStart";
  case LC_ServiceStatusStarted:      return "Started";
  case LC_ServiceStatusUp:           return "Up";
  case LC_ServiceStatusStopping:     return "Stopping";
  case LC_ServiceStatusAborted:      return "Aborted";
  case LC_ServiceStatusDisabled:     return "Disabled";
  default:                           return "Unknown";
  }
}



GWEN_TYPE_UINT32 LC_ServiceFlags_fromDb(GWEN_DB_NODE *db, const char *name){
  const char *p;
  int i;
  GWEN_TYPE_UINT32 flags=0;

  for (i=0; ; i++) {
    p=GWEN_DB_GetCharValue(db, name, i, 0);
    if (!p)
      break;
    if (strcasecmp(p, "autoload")==0)
      flags|=LC_SERVICE_FLAGS_AUTOLOAD;
    else if (strcasecmp(p, "client")==0)
      flags|=LC_SERVICE_FLAGS_CLIENT;
    else if (strcasecmp(p, "silent")==0)
      flags|=LC_SERVICE_FLAGS_SILENT;
    else {
      DBG_WARN(0, "Unknown service flag \"%s\"", p);
    }
  }

  return flags;
}



void LC_ServiceFlags_toDb(GWEN_DB_NODE *db,
                          const char *name,
                          GWEN_TYPE_UINT32 flags) {
  GWEN_DB_DeleteVar(db, name);
  if (flags & LC_SERVICE_FLAGS_AUTOLOAD)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT, name, "autoload");
  if (flags & LC_SERVICE_FLAGS_CLIENT)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT, name, "client");
  if (flags & LC_SERVICE_FLAGS_SILENT)
    GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_DEFAULT, name, "silent");
}












