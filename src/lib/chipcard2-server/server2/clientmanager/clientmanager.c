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


#include "clientmanager_p.h"
#include "fullserver_l.h"
#include <gwenhywfar/debug.h>
#include <gwenhywfar/misc.h>


#include <stdlib.h>
#include <assert.h>
#include <string.h>


LCCL_CLIENTMANAGER *LCCL_ClientManager_new(LCS_SERVER *server) {
  LCCL_CLIENTMANAGER *clm;

  assert(server);
  GWEN_NEW_OBJECT(LCCL_CLIENTMANAGER, clm);
  clm->server=server;
  clm->ipcManager=LCS_Server_GetIpcManager(server);
  clm->clients=LCCL_Client_List_new();

  return clm;
}



void LCCL_ClientManager_free(LCCL_CLIENTMANAGER *clm) {
  if (clm) {
    LCCL_Client_List_free(clm->clients);
    GWEN_FREE_OBJECT(clm);
  }
}



int LCCL_ClientManager_Init(LCCL_CLIENTMANAGER *clm, GWEN_DB_NODE *dbConfig) {
  GWEN_DB_NODE *dbT;

  DBG_INFO(0, "Initializing client manager");
  assert(clm);

  clm->maxClientLockTime=LCCL_CLIENTMANAGER_DEF_MAX_CLIENT_LOCKTIME;
  clm->maxClientLocks=LCCL_CLIENTMANAGER_DEF_MAX_CLIENT_LOCKS;
  clm->takeCardExpireTimeout=LCCL_CLIENTMANAGER_DEF_TAKE_CARD_EXPIRE_TIMEOUT;
  clm->commandTimeout=LCCL_CLIENTMANAGER_DEF_COMMAND_TIMEOUT;

  dbT=GWEN_DB_GetGroup(dbConfig, GWEN_PATH_FLAGS_NAMEMUSTEXIST,
                       "ClientManager");
  if (dbT) {
    /* read some values */
#define LCCL_CLM_INIT_VAL(s) \
  clm->s=GWEN_DB_GetIntValue(dbT, __STRING(s), 0, clm->s);
    LCCL_CLM_INIT_VAL(maxClientLockTime)
    LCCL_CLM_INIT_VAL(maxClientLocks)
    LCCL_CLM_INIT_VAL(takeCardExpireTimeout)
    LCCL_CLM_INIT_VAL(commandTimeout)
#undef LCDM_DM_INIT_VAL
  }
  return 0;
}



int LCCL_ClientManager_Fini(LCCL_CLIENTMANAGER *clm, GWEN_DB_NODE *db) {
  return 0;
}



int LCCL_ClientManager_Work(LCCL_CLIENTMANAGER *clm) {
  return 0;
}



int LCCL_ClientManager_HandleRequest(LCCL_CLIENTMANAGER *clm,
                                     GWEN_TYPE_UINT32 rid,
                                     const char *name,
                                     GWEN_DB_NODE *dbReq) {
  int rv;

  assert(clm);
  assert(name);

  if (strcasecmp(name, "ClientReady")==0) {
    rv=LCCL_ClientManager_HandleClientReady(clm, rid, name, dbReq);
  }
  else if (strcasecmp(name, "SetNotify")==0) {
    rv=LCCL_ClientManager_HandleSetNotify(clm, rid, name, dbReq);
  }
  else if (strcasecmp(name, "StartWait")==0) {
    rv=LCCL_ClientManager_HandleStartWait(clm, rid, name, dbReq);
  }
  else if (strcasecmp(name, "StopWait")==0) {
    rv=LCCL_ClientManager_HandleStopWait(clm, rid, name, dbReq);
  }
  else if (strcasecmp(name, "TakeCard")==0) {
    rv=LCCL_ClientManager_HandleTakeCard(clm, rid, name, dbReq);
  }
  else if (strcasecmp(name, "ReleaseCard")==0) {
    rv=LCCL_ClientManager_HandleReleaseCard(clm, rid, name, dbReq);
  }
  else if (strcasecmp(name, "CommandCard")==0) {
    rv=LCCL_ClientManager_HandleExecApdu(clm, rid, name, dbReq);
  }
  else if (strcasecmp(name, "execCommand")==0) {
    rv=LCCL_ClientManager_HandleExecCommand(clm, rid, name, dbReq);
  }
  else if (strcasecmp(name, "CardReset")==0) {
    rv=LCCL_ClientManager_HandleCardReset(clm, rid, name, dbReq);
  }
  else if (strcasecmp(name, "SelectCard")==0) {
    rv=LCCL_ClientManager_HandleSelectCard(clm, rid, name, dbReq);
  }
  /* Insert more handlers here */
  else {
    DBG_INFO(0, "Command \"%s\" not handled by client manager",
             name);
    rv=1; /* not handled */
  }

  return rv;
}



void LCCL_ClientManager_DriverChg(LCCL_CLIENTMANAGER *clm,
                                  GWEN_TYPE_UINT32 did,
                                  const char *driverType,
                                  const char *driverName,
                                  const char *libraryFile,
                                  LC_DRIVER_STATUS newSt,
                                  const char *reason) {
  LCCL_ClientManager_SendDriverNotification(clm,
                                            /* if there currently is a
                                             * setNotify request in work
                                             * then listingClient would be
                                             * set, otherwise it is 0 which
                                             * means to send the notification
                                             * to all interested clients */
                                            clm->listingClient,
                                            did,
                                            driverType,
                                            driverName,
                                            libraryFile,
                                            newSt,
                                            reason);
}



void LCCL_ClientManager_ReaderChg(LCCL_CLIENTMANAGER *clm,
                                  GWEN_TYPE_UINT32 did,
                                  GWEN_TYPE_UINT32 rid,
                                  const char *readerType,
                                  const char *readerName,
                                  LC_READER_STATUS newSt,
                                  const char *reason) {
  LCCL_ClientManager_SendReaderNotification(clm,
                                            /* if there currently is a
                                             * setNotify request in work
                                             * then listingClient would be
                                             * set, otherwise it is 0 which
                                             * means to send the notification
                                             * to all interested clients */
                                            clm->listingClient,
                                            did, rid,
                                            readerType,
                                            readerName,
                                            newSt,
                                            reason);
}



void LCCL_ClientManager_NewCard(LCCL_CLIENTMANAGER *clm, LCCO_CARD *card){
  if (LCCO_Card_GetStatus(card)==LC_CardStatusInserted) {
    LCCL_CLIENT *cl;

    DBG_WARN(0, "Advertising card \"%08x\" to all interested clients",
             LCCO_Card_GetCardId(card));
    cl=LCCL_Client_List_First(clm->clients);
    while(cl) {
      if (LCCL_Client_GetWaitRequestCount(cl))
        LCCL_ClientManager_SendCardAvailable(clm, cl, card);
      cl=LCCL_Client_List_Next(cl);
    } /* while */
  }
  else {
    DBG_WARN(0, "Card \"%08x\" not inserted, will not handle it",
             LCCO_Card_GetCardId(card));
  }
}



void LCCL_ClientManager_CardRemoved(LCCL_CLIENTMANAGER *clm,
                                    GWEN_TYPE_UINT32 rid,
                                    int slotNum,
                                    GWEN_TYPE_UINT32 cardNum) {
  // TODO
}



void LCCL_ClientManager_ClientDown(LCCL_CLIENTMANAGER *clm,
                                   GWEN_TYPE_UINT32 clid) {
  LCCL_CLIENT *cl;
  LCCM_CARDMANAGER *cm;
  LCCO_CARD *card;

  cl=LCCL_Client_List_First(clm->clients);
  while(cl) {
    if (LCCL_Client_GetClientId(cl)==clid)
      break;
    cl=LCCL_Client_List_Next(cl);
  } /* while */
  if (!cl) {
    DBG_ERROR(0, "Client \"%08x\" not found", clid);
    return;
  }

  /* unregister from every card in the card manager,
   * unlock and reset all locked cards */
  cm=LCS_FullServer_GetCardManager(clm->server);
  assert(cm);

  card=LCCM_CardManager_GetFirstCard(cm);
  while(card) {
    if (!LCCM_CardManager_CheckAccess(cm, card, LCCL_Client_GetClientId(cl))){
      GWEN_TYPE_UINT32 rid;
      int rv;

      /* we own this card, so unlock it */
      rid=LCCL_ClientManager_SendResetCard(clm, card);
      if (rid) {
        /* immediately remove this request, we don't expect an answer */
        if (GWEN_IPCManager_RemoveRequest(clm->ipcManager, rid, 1)) {
          DBG_ERROR(0, "Could not remove request");
          abort();
        }
      }
      else {
        DBG_WARN(0, "Could not reset card \"%08x\"",
                 LCCO_Card_GetCardId(card));
      }
      rv=LCCM_CardManager_UnlockCard(cm, card, LCCL_Client_GetClientId(cl));
      if (rv) {
        DBG_WARN(0, "Could not unlock card \"%08x\"",
                 LCCO_Card_GetCardId(card));
      }
      else {
        DBG_INFO(0, "Card \"%08x\" unlocked from client",
                 LCCO_Card_GetCardId(card));
      }
    }
    card=LCCM_CardManager_GetNextCard(cm, card);
  }

  LCCM_CardManager_ClientDown(cm, LCCL_Client_GetClientId(cl));

  /* unregister from usage of readers if necessary */
  if (LCCL_Client_GetWaitRequestCount(cl)) {
    LCS_Server_EndUseReaders(clm->server, 1);
    LCCL_Client_ResetRequestCount(cl);
  }

  /* finally destroy client */
  DBG_NOTICE(0, "Removing client %08x [%s/%s]",
             LCCL_Client_GetClientId(cl),
             LCCL_Client_GetApplicationName(cl),
             LCCL_Client_GetUserName(cl));
  LCCL_Client_List_Del(cl);
  LCCL_Client_free(cl);
}



void LCCL_ClientManager_ServiceChg(LCCL_CLIENTMANAGER *clm,
                                   GWEN_TYPE_UINT32 sid,
                                   const char *serviceType,
                                   const char *serviceName,
                                   LC_SERVICE_STATUS newSt,
                                   const char *reason) {
  LCCL_ClientManager_SendServiceNotification(clm,
                                             /* if there currently is a
                                              * setNotify request in work
                                              * then listingClient would be
                                              * set, otherwise it is 0 which
                                              * means to send the notification
                                              * to all interested clients */
                                             clm->listingClient,
                                             sid,
                                             serviceType,
                                             serviceName,
                                             newSt,
                                             reason);
}

















