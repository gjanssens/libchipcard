/* This file is auto-generated from "geldkarte_llog.xml" by the typemaker
   tool of Gwenhywfar. 
   Do not edit this file -- all changes will be lost! */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "geldkarte_llog_p.h"
#include <gwenhywfar/misc.h>
#include <gwenhywfar/db.h>
#include <gwenhywfar/debug.h>
#include <assert.h>
#include <stdlib.h>


GWEN_LIST2_FUNCTIONS(LC_GELDKARTE_LLOG, LC_GeldKarte_LLog)


LC_GELDKARTE_LLOG *LC_GeldKarte_LLog_new() {
  LC_GELDKARTE_LLOG *st;

  GWEN_NEW_OBJECT(LC_GELDKARTE_LLOG, st)
  st->_usage=1;
  return st;
}


void LC_GeldKarte_LLog_free(LC_GELDKARTE_LLOG *st) {
  if (st) {
    assert(st->_usage);
    if (--(st->_usage)==0) {
  if (st->centerId)
    free(st->centerId);
  if (st->terminalId)
    free(st->terminalId);
  if (st->traceId)
    free(st->traceId);
  if (st->time)
    GWEN_Time_free(st->time);
  GWEN_FREE_OBJECT(st);
    }
  }

}


LC_GELDKARTE_LLOG *LC_GeldKarte_LLog_dup(const LC_GELDKARTE_LLOG *d) {
  LC_GELDKARTE_LLOG *st;

  assert(d);
  st=LC_GeldKarte_LLog_new();
  st->status=d->status;
  st->bSeq=d->bSeq;
  st->lSeq=d->lSeq;
  st->value=d->value;
  if (d->centerId)
    st->centerId=strdup(d->centerId);
  if (d->terminalId)
    st->terminalId=strdup(d->terminalId);
  if (d->traceId)
    st->traceId=strdup(d->traceId);
  st->loaded=d->loaded;
  if (d->time)
    st->time=GWEN_Time_dup(d->time);
  return st;
}


int LC_GeldKarte_LLog_toDb(const LC_GELDKARTE_LLOG *st, GWEN_DB_NODE *db) {
  assert(st);
  assert(db);
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "status", st->status))
    return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "bSeq", st->bSeq))
    return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "lSeq", st->lSeq))
    return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "value", st->value))
    return -1;
  if (st->centerId)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "centerId", st->centerId))
      return -1;
  if (st->terminalId)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "terminalId", st->terminalId))
      return -1;
  if (st->traceId)
    if (GWEN_DB_SetCharValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "traceId", st->traceId))
      return -1;
  if (GWEN_DB_SetIntValue(db, GWEN_DB_FLAGS_OVERWRITE_VARS, "loaded", st->loaded))
    return -1;
  if (st->time)
    if (GWEN_Time_toDb(st->time, GWEN_DB_GetGroup(db, GWEN_DB_FLAGS_DEFAULT, "time")))
      return -1;
  return 0;
}


LC_GELDKARTE_LLOG *LC_GeldKarte_LLog_fromDb(GWEN_DB_NODE *db) {
LC_GELDKARTE_LLOG *st;

  assert(db);
  st=LC_GeldKarte_LLog_new();
  LC_GeldKarte_LLog_SetStatus(st, GWEN_DB_GetIntValue(db, "status", 0, 0));
  LC_GeldKarte_LLog_SetBSeq(st, GWEN_DB_GetIntValue(db, "bSeq", 0, 0));
  LC_GeldKarte_LLog_SetLSeq(st, GWEN_DB_GetIntValue(db, "lSeq", 0, 0));
  LC_GeldKarte_LLog_SetValue(st, GWEN_DB_GetIntValue(db, "value", 0, 0));
  LC_GeldKarte_LLog_SetCenterId(st, GWEN_DB_GetCharValue(db, "centerId", 0, 0));
  LC_GeldKarte_LLog_SetTerminalId(st, GWEN_DB_GetCharValue(db, "terminalId", 0, 0));
  LC_GeldKarte_LLog_SetTraceId(st, GWEN_DB_GetCharValue(db, "traceId", 0, 0));
  LC_GeldKarte_LLog_SetLoaded(st, GWEN_DB_GetIntValue(db, "loaded", 0, 0));
  if (1) {
    GWEN_DB_NODE *dbT;

    dbT=GWEN_DB_GetGroup(db, GWEN_PATH_FLAGS_NAMEMUSTEXIST, "time");
    if (dbT)  LC_GeldKarte_LLog_SetTime(st, GWEN_Time_fromDb(dbT));
  }
  st->_modified=0;
  return st;
}


int LC_GeldKarte_LLog_GetStatus(const LC_GELDKARTE_LLOG *st) {
  assert(st);
  return st->status;
}


void LC_GeldKarte_LLog_SetStatus(LC_GELDKARTE_LLOG *st, int d) {
  assert(st);
  st->status=d;
  st->_modified=1;
}


int LC_GeldKarte_LLog_GetBSeq(const LC_GELDKARTE_LLOG *st) {
  assert(st);
  return st->bSeq;
}


void LC_GeldKarte_LLog_SetBSeq(LC_GELDKARTE_LLOG *st, int d) {
  assert(st);
  st->bSeq=d;
  st->_modified=1;
}


int LC_GeldKarte_LLog_GetLSeq(const LC_GELDKARTE_LLOG *st) {
  assert(st);
  return st->lSeq;
}


void LC_GeldKarte_LLog_SetLSeq(LC_GELDKARTE_LLOG *st, int d) {
  assert(st);
  st->lSeq=d;
  st->_modified=1;
}


int LC_GeldKarte_LLog_GetValue(const LC_GELDKARTE_LLOG *st) {
  assert(st);
  return st->value;
}


void LC_GeldKarte_LLog_SetValue(LC_GELDKARTE_LLOG *st, int d) {
  assert(st);
  st->value=d;
  st->_modified=1;
}


const char *LC_GeldKarte_LLog_GetCenterId(const LC_GELDKARTE_LLOG *st) {
  assert(st);
  return st->centerId;
}


void LC_GeldKarte_LLog_SetCenterId(LC_GELDKARTE_LLOG *st, const char *d) {
  assert(st);
  if (st->centerId)
    free(st->centerId);
  if (d)
    st->centerId=strdup(d);
  else
    st->centerId=0;
  st->_modified=1;
}


const char *LC_GeldKarte_LLog_GetTerminalId(const LC_GELDKARTE_LLOG *st) {
  assert(st);
  return st->terminalId;
}


void LC_GeldKarte_LLog_SetTerminalId(LC_GELDKARTE_LLOG *st, const char *d) {
  assert(st);
  if (st->terminalId)
    free(st->terminalId);
  if (d)
    st->terminalId=strdup(d);
  else
    st->terminalId=0;
  st->_modified=1;
}


const char *LC_GeldKarte_LLog_GetTraceId(const LC_GELDKARTE_LLOG *st) {
  assert(st);
  return st->traceId;
}


void LC_GeldKarte_LLog_SetTraceId(LC_GELDKARTE_LLOG *st, const char *d) {
  assert(st);
  if (st->traceId)
    free(st->traceId);
  if (d)
    st->traceId=strdup(d);
  else
    st->traceId=0;
  st->_modified=1;
}


int LC_GeldKarte_LLog_GetLoaded(const LC_GELDKARTE_LLOG *st) {
  assert(st);
  return st->loaded;
}


void LC_GeldKarte_LLog_SetLoaded(LC_GELDKARTE_LLOG *st, int d) {
  assert(st);
  st->loaded=d;
  st->_modified=1;
}


const GWEN_TIME *LC_GeldKarte_LLog_GetTime(const LC_GELDKARTE_LLOG *st) {
  assert(st);
  return st->time;
}


void LC_GeldKarte_LLog_SetTime(LC_GELDKARTE_LLOG *st, const GWEN_TIME *d) {
  assert(st);
  if (st->time)
    GWEN_Time_free(st->time);
  if (d)
    st->time=GWEN_Time_dup(d);
  else
    st->time=0;
  st->_modified=1;
}


int LC_GeldKarte_LLog_IsModified(const LC_GELDKARTE_LLOG *st) {
  assert(st);
  return st->_modified;
}


void LC_GeldKarte_LLog_SetModified(LC_GELDKARTE_LLOG *st, int i) {
  assert(st);
  st->_modified=i;
}


void LC_GeldKarte_LLog_Attach(LC_GELDKARTE_LLOG *st) {
  assert(st);
  st->_usage++;
}
LC_GELDKARTE_LLOG *LC_GeldKarte_LLog_List2__freeAll_cb(LC_GELDKARTE_LLOG *st, void *user_data) {
  LC_GeldKarte_LLog_free(st);
return 0;
}


void LC_GeldKarte_LLog_List2_freeAll(LC_GELDKARTE_LLOG_LIST2 *stl) {
  if (stl) {
    LC_GeldKarte_LLog_List2_ForEach(stl, LC_GeldKarte_LLog_List2__freeAll_cb, 0);
    LC_GeldKarte_LLog_List2_free(stl); 
  }
}





