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


#ifndef CHIPCARD_SERVER_DEVICEMANAGER_L_H
#define CHIPCARD_SERVER_DEVICEMANAGER_L_H


#include <gwenhywfar/types.h>
#include <gwenhywfar/ipc.h>
#include <gwenhywfar/inherit.h>
#include <gwenhywfar/stringlist.h>


/**
 * This module expects that the GWEN_PathManager contains information
 * about the path LCDM_DEVICEMANAGER_PM_DRIVER_INFODIR of the destlib
 * LCDM_DEVICEMANAGER_PM_DESTLIB.
 * It also expects the plugin manager for LCDM_DEVICEMANAGER_PLUGIN_DRIVER
 * to be prepared.
 */

typedef struct LCDM_DEVICEMANAGER LCDM_DEVICEMANAGER;

GWEN_INHERIT_FUNCTION_DEFS(LCDM_DEVICEMANAGER)


#include "server_l.h"
#include "lockmanager_l.h"
#include <chipcard/chipcard.h>
#include "common/card.h"
#include "common/devmonitor.h"



LCDM_DEVICEMANAGER *LCDM_DeviceManager_new(LCS_SERVER *server);
void LCDM_DeviceManager_free(LCDM_DEVICEMANAGER *dm);

int LCDM_DeviceManager_Init(LCDM_DEVICEMANAGER *dm, GWEN_DB_NODE *dbConfig);
int LCDM_DeviceManager_Fini(LCDM_DEVICEMANAGER *dm);

void LCDM_DeviceManager_BeginUseCard(LCDM_DEVICEMANAGER *dm, LCCO_CARD *cd);
void LCDM_DeviceManager_EndUseCard(LCDM_DEVICEMANAGER *dm, LCCO_CARD *cd);

void LCDM_DeviceManager_BeginUseReader(LCDM_DEVICEMANAGER *dm,
                                       GWEN_TYPE_UINT32 rid);
void LCDM_DeviceManager_EndUseReader(LCDM_DEVICEMANAGER *dm,
                                     GWEN_TYPE_UINT32 rid);

LC_READER_STATUS LCDM_DeviceManager_GetReaderStatus(LCDM_DEVICEMANAGER *dm,
                                                    GWEN_TYPE_UINT32 rid);

/**
 * This function is used when a client sends a WaitForCard-request.
 */
void LCDM_DeviceManager_BeginUseReaders(LCDM_DEVICEMANAGER *dm);
void LCDM_DeviceManager_EndUseReaders(LCDM_DEVICEMANAGER *dm, int count);


void LCDM_DeviceManager_DriverIpcDown(LCDM_DEVICEMANAGER *dm,
                                      GWEN_TYPE_UINT32 ipcId);

/**
 * This function calls @ref LCS_Server_ReaderChg for every currently known
 * reader.
 */
int LCDM_DeviceManager_ListReaders(LCDM_DEVICEMANAGER *dm);

/**
 * This function calls @ref LCS_Server_DriverChg for every currently known
 * reader.
 */
int LCDM_DeviceManager_ListDrivers(LCDM_DEVICEMANAGER *dm);


/**
 * @return 1 if something could be done, 0 otherwise
 */
int LCDM_DeviceManager_Work(LCDM_DEVICEMANAGER *dm);

int LCDM_DeviceManager_HandleRequest(LCDM_DEVICEMANAGER *dm,
                                     GWEN_TYPE_UINT32 rid,
                                     const char *name,
                                     GWEN_DB_NODE *dbReq);

GWEN_TYPE_UINT32 LCDM_DeviceManager_SendCardCommand(LCDM_DEVICEMANAGER *dm,
                                                    LCCO_CARD *card,
                                                    GWEN_DB_NODE *dbCmd);

const char *LCDM_DeviceManager_GetDriverVar(LCDM_DEVICEMANAGER *dm,
                                            LCCO_CARD *card,
                                            const char *vname);

void LCDM_DeviceManager_DumpState(const LCDM_DEVICEMANAGER *dm);



LCS_LOCKMANAGER*
  LCDM_DeviceManager_GetLockManager(const LCDM_DEVICEMANAGER *dm,
                                    GWEN_TYPE_UINT32 rid,
                                    int slot);

void LCDM_DeviceManager_ClientDown(LCDM_DEVICEMANAGER *dm,
                                   GWEN_TYPE_UINT32 clid);


GWEN_TYPE_UINT32 LCDM_DeviceManager_LockReader(LCDM_DEVICEMANAGER *dm,
                                               GWEN_TYPE_UINT32 rid,
                                               GWEN_TYPE_UINT32 clid,
                                               int maxLockTime,
                                               int maxLockCount);
int LCDM_DeviceManager_CheckLockReaderRequest(LCDM_DEVICEMANAGER *dm,
                                              GWEN_TYPE_UINT32 rid,
                                              GWEN_TYPE_UINT32 rqid);
int LCDM_DeviceManager_RemoveLockReaderRequest(LCDM_DEVICEMANAGER *dm,
                                               GWEN_TYPE_UINT32 rid,
                                               GWEN_TYPE_UINT32 rqid);

int LCDM_DeviceManager_CheckLockReaderAccess(LCDM_DEVICEMANAGER *dm,
                                             GWEN_TYPE_UINT32 rid,
                                             GWEN_TYPE_UINT32 rqid);

int LCDM_DeviceManager_UnlockReader(LCDM_DEVICEMANAGER *dm,
                                    GWEN_TYPE_UINT32 rid,
                                    GWEN_TYPE_UINT32 rqid);

int LCDM_DeviceManager_SuspendReaderCheck(LCDM_DEVICEMANAGER *dm,
                                          GWEN_TYPE_UINT32 rid);

void LCDM_DeviceManager_ResumeReaderCheck(LCDM_DEVICEMANAGER *dm,
                                          GWEN_TYPE_UINT32 rid);

GWEN_TYPE_UINT32
  LCDM_DeviceManager_SendReaderCommand(LCDM_DEVICEMANAGER *dm,
                                       GWEN_TYPE_UINT32 readerId,
                                       GWEN_DB_NODE *dbCmd);

void LCDM_DeviceManager_TriggerHwScan(LCDM_DEVICEMANAGER *dm);


#endif /* CHIPCARD_SERVER_DEVICEMANAGER_L_H */


