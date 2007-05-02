/* This file is auto-generated from "lockrequest.xml" by the typemaker
   tool of Gwenhywfar. 
   Do not edit this file -- all changes will be lost! */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "lockrequest_p.h"
#include <gwenhywfar/misc.h>
#include <gwenhywfar/db.h>
#include <gwenhywfar/debug.h>
#include <assert.h>
#include <stdlib.h>
#include <strings.h>



GWEN_LIST_FUNCTIONS(LCS_LOCKREQUEST, LCS_LockRequest)


LCS_LOCKREQUEST *LCS_LockRequest_new() {
  LCS_LOCKREQUEST *st;

  GWEN_NEW_OBJECT(LCS_LOCKREQUEST, st)
  st->_usage=1;
  GWEN_LIST_INIT(LCS_LOCKREQUEST, st)
  return st;
}


void LCS_LockRequest_free(LCS_LOCKREQUEST *st) {
  if (st) {
    assert(st->_usage);
    if (--(st->_usage)==0) {
  if (st->lockTime)
    GWEN_Time_free(st->lockTime);
  if (st->lockUntil)
    GWEN_Time_free(st->lockUntil);
  GWEN_LIST_FINI(LCS_LOCKREQUEST, st)
  GWEN_FREE_OBJECT(st);
    }
  }

}


LCS_LOCKREQUEST *LCS_LockRequest_dup(const LCS_LOCKREQUEST *d) {
  LCS_LOCKREQUEST *st;

  assert(d);
  st=LCS_LockRequest_new();
  st->requestId=d->requestId;
  st->clientId=d->clientId;
  if (d->lockTime)
    st->lockTime=GWEN_Time_dup(d->lockTime);
  st->duration=d->duration;
  if (d->lockUntil)
    st->lockUntil=GWEN_Time_dup(d->lockUntil);
  return st;
}


int LCS_LockRequest_toDb(const LCS_LOCKREQUEST *st, GWEN_DB_NODE *db) {
  assert(st);
  assert(db);
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "requestId", st->requestId))
    return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "clientId", st->clientId))
    return -1;
  if (st->lockTime)
    if (GWEN_Time_toDb(st->lockTime, GWEN_DB_GetGroup(db, GWEN_DB_FLAGS_DEFAULT, "lockTime")))
      return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "duration", st->duration))
    return -1;
  if (st->lockUntil)
    if (GWEN_Time_toDb(st->lockUntil, GWEN_DB_GetGroup(db, GWEN_DB_FLAGS_DEFAULT, "lockUntil")))
      return -1;
  return 0;
}


LCS_LOCKREQUEST *LCS_LockRequest_fromDb(GWEN_DB_NODE *db) {
LCS_LOCKREQUEST *st;

  assert(db);
  st=LCS_LockRequest_new();
  LCS_LockRequest_SetRequestId(st, GWEN_DB_GetIntValue(db, "requestId", 0, 0));
  LCS_LockRequest_SetClientId(st, GWEN_DB_GetIntValue(db, "clientId", 0, 0));
  if (1) {
    GWEN_DB_NODE *dbT;

    dbT=GWEN_DB_GetGroup(db, GWEN_PATH_FLAGS_NAMEMUSTEXIST, "lockTime");
    if (dbT) st->lockTime=GWEN_Time_fromDb(dbT);
  }
  LCS_LockRequest_SetDuration(st, GWEN_DB_GetIntValue(db, "duration", 0, 0));
  if (1) {
    GWEN_DB_NODE *dbT;

    dbT=GWEN_DB_GetGroup(db, GWEN_PATH_FLAGS_NAMEMUSTEXIST, "lockUntil");
    if (dbT) st->lockUntil=GWEN_Time_fromDb(dbT);
  }
  st->_modified=0;
  return st;
}


GWEN_TYPE_UINT32 LCS_LockRequest_GetRequestId(const LCS_LOCKREQUEST *st) {
  assert(st);
  return st->requestId;
}


void LCS_LockRequest_SetRequestId(LCS_LOCKREQUEST *st, GWEN_TYPE_UINT32 d) {
  assert(st);
  st->requestId=d;
  st->_modified=1;
}




GWEN_TYPE_UINT32 LCS_LockRequest_GetClientId(const LCS_LOCKREQUEST *st) {
  assert(st);
  return st->clientId;
}


void LCS_LockRequest_SetClientId(LCS_LOCKREQUEST *st, GWEN_TYPE_UINT32 d) {
  assert(st);
  st->clientId=d;
  st->_modified=1;
}




const GWEN_TIME *LCS_LockRequest_GetLockTime(const LCS_LOCKREQUEST *st) {
  assert(st);
  return st->lockTime;
}


void LCS_LockRequest_SetLockTime(LCS_LOCKREQUEST *st, const GWEN_TIME *d) {
  assert(st);
  if (st->lockTime)
    GWEN_Time_free(st->lockTime);
  if (d)
    st->lockTime=GWEN_Time_dup(d);
  else
    st->lockTime=0;
  st->_modified=1;
}




int LCS_LockRequest_GetDuration(const LCS_LOCKREQUEST *st) {
  assert(st);
  return st->duration;
}


void LCS_LockRequest_SetDuration(LCS_LOCKREQUEST *st, int d) {
  assert(st);
  st->duration=d;
  st->_modified=1;
}



const GWEN_TIME *LCS_LockRequest_GetLockUntil(const LCS_LOCKREQUEST *st) {
  assert(st);
  return st->lockUntil;
}



void LCS_LockRequest_SetLockUntil(LCS_LOCKREQUEST *st, const GWEN_TIME *d) {
  assert(st);
  if (st->lockUntil)
    GWEN_Time_free(st->lockUntil);
  if (d)
    st->lockUntil=GWEN_Time_dup(d);
  else
    st->lockUntil=0;
  st->_modified=1;
}




int LCS_LockRequest_IsModified(const LCS_LOCKREQUEST *st) {
  assert(st);
  return st->_modified;
}


void LCS_LockRequest_SetModified(LCS_LOCKREQUEST *st, int i) {
  assert(st);
  st->_modified=i;
}


void LCS_LockRequest_Attach(LCS_LOCKREQUEST *st) {
  assert(st);
  st->_usage++;
}
LCS_LOCKREQUEST_LIST *LCS_LockRequest_List_dup(const LCS_LOCKREQUEST_LIST *stl) {
  if (stl) {
    LCS_LOCKREQUEST_LIST *nl;
    LCS_LOCKREQUEST *e;

    nl=LCS_LockRequest_List_new();
    e=LCS_LockRequest_List_First(stl);
    while(e) {
      LCS_LOCKREQUEST *ne;

      ne=LCS_LockRequest_dup(e);
      assert(ne);
      LCS_LockRequest_List_Add(ne, nl);
      e=LCS_LockRequest_List_Next(e);
    } /* while (e) */
    return nl;
  }
  else
    return 0;
}


