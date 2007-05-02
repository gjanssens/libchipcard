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


#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "fsclient_p.h"

#include <gwenhywfar/misc.h>
#include <gwenhywfar/debug.h>


GWEN_LIST_FUNCTIONS(LC_FS_CLIENT, LC_FSClient)



LC_FS_CLIENT *LC_FSClient_new(LC_FS *fs, GWEN_TYPE_UINT32 id){
  LC_FS_CLIENT *fcl;

  GWEN_NEW_OBJECT(LC_FS_CLIENT, fcl);
  GWEN_LIST_INIT(LC_FS_CLIENT, fcl);
  fcl->fileSystem=fs;
  fcl->id=id;
  fcl->handles=LC_FSNodeHandle_List_new();

  return fcl;
}



void LC_FSClient_free(LC_FS_CLIENT *fcl){
  if (fcl) {
    GWEN_LIST_FINI(LC_FS_CLIENT, fcl);
    LC_FSNodeHandle_List_free(fcl->handles);
    GWEN_FREE_OBJECT(fcl);
  }
}



LC_FS *LC_FSClient_GetFileSystem(const LC_FS_CLIENT *fcl){
  assert(fcl);
  return fcl->fileSystem;
}



GWEN_TYPE_UINT32 LC_FSClient_GetId(const LC_FS_CLIENT *fcl){
  assert(fcl);
  return fcl->id;
}



LC_FS_NODE_HANDLE_LIST *LC_FSClient_GetHandles(const LC_FS_CLIENT *fcl){
  assert(fcl);
  return fcl->handles;
}



LC_FS_PATH_CTX *LC_FSClient_GetWorkingCtx(const LC_FS_CLIENT *fcl){
  assert(fcl);
  return fcl->workingCtx;
}



void LC_FSClient_SetWorkingCtx(LC_FS_CLIENT *fcl, LC_FS_PATH_CTX *ctx){
  assert(fcl);
  LC_FSPathCtx_free(fcl->workingCtx);
  fcl->workingCtx=ctx;
}



GWEN_TYPE_UINT32 LC_FSClient_GetNextHandleId(LC_FS_CLIENT *fcl){
  assert(fcl);
  return ++(fcl->lastHandleId);
}



void LC_FSClient_AddNodeHandle(LC_FS_CLIENT *fcl, LC_FS_NODE_HANDLE *hdl){
  assert(fcl);
  LC_FSNodeHandle_List_Add(hdl, fcl->handles);
}



LC_FS_NODE_HANDLE *LC_FSClient_FindHandle(LC_FS_CLIENT *fcl,
                                          GWEN_TYPE_UINT32 hid){
  LC_FS_NODE_HANDLE *hdl;

  hdl=LC_FSNodeHandle_List_First(fcl->handles);
  while(hdl) {
    if (LC_FSNodeHandle_GetId(hdl)==hid)
      break;
    hdl=LC_FSNodeHandle_List_Next(hdl);
  } /* while */
  return hdl;
}





