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

#include "cardmgr_p.h"
#include "../card_l.h"
#include "msgengine_l.h"

#include <gwenhywfar/debug.h>
#include <gwenhywfar/inherit.h>
#include <gwenhywfar/misc.h>
#include <gwenhywfar/text.h>
#include <gwenhywfar/directory.h>

#include <chipcard2-server/chipcard2.h>


#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>



LC_CARDMGR *LC_CardMgr_new(const GWEN_STRINGLIST *paths){
  LC_CARDMGR *mgr;

  GWEN_NEW_OBJECT(LC_CARDMGR, mgr);
  mgr->contexts=LC_CardContext_List_new();
  mgr->paths=GWEN_StringList_dup(paths);
  mgr->cardFiles=GWEN_StringList_new();
  mgr->appFiles=GWEN_StringList_new();
  mgr->loadedCards=GWEN_StringList_new();
  mgr->loadedApps=GWEN_StringList_new();
  mgr->xmlCards=GWEN_XMLNode_new(GWEN_XMLNodeTypeTag, "cards");
  mgr->xmlApps=GWEN_XMLNode_new(GWEN_XMLNodeTypeTag, "apps");
  LC_CardMgr_SampleFiles(mgr, paths);
  mgr->msgEngine=LC_MsgEngine_new();

  mgr->usage=1;
  return mgr;
}



void LC_CardMgr_free(LC_CARDMGR *mgr){
  if (mgr) {
    assert(mgr->usage);
    if (--(mgr->usage)==0) {
      LC_CardContext_List_free(mgr->contexts);
      GWEN_MsgEngine_free(mgr->msgEngine);
      GWEN_XMLNode_free(mgr->xmlApps);
      GWEN_XMLNode_free(mgr->xmlCards);
      GWEN_StringList_free(mgr->loadedApps);
      GWEN_StringList_free(mgr->loadedCards);
      GWEN_StringList_free(mgr->appFiles);
      GWEN_StringList_free(mgr->cardFiles);
      GWEN_StringList_free(mgr->paths);
      GWEN_FREE_OBJECT(mgr);
    }
  }
}



void LC_CardMgr_Attach(LC_CARDMGR *mgr){
  assert(mgr);
  mgr->usage++;
}



GWEN_MSGENGINE *LC_CardMgr_GetMsgEngine(const LC_CARDMGR *mgr){
  assert(mgr);
  return mgr->msgEngine;
}



void LC_CardMgr__SampleFiles(LC_CARDMGR *mgr,
                             const char *where) {
  GWEN_BUFFER *buf;
  GWEN_DIRECTORYDATA *d;
  unsigned int dpos;

  buf=GWEN_Buffer_new(0, 256, 0, 1);

  d=GWEN_Directory_new();
  GWEN_Buffer_AppendString(buf, where);
  GWEN_Buffer_AppendByte(buf, '/');
  GWEN_Buffer_AppendString(buf, "cards");
  DBG_DEBUG(0, "Sampling files in \"%s\"",
            GWEN_Buffer_GetStart(buf));
  dpos=GWEN_Buffer_GetPos(buf);
  if (!GWEN_Directory_Open(d, GWEN_Buffer_GetStart(buf))) {
    char buffer[256];
    unsigned int i;

    while (!GWEN_Directory_Read(d, buffer, sizeof(buffer))){
      struct stat st;

      GWEN_Buffer_Crop(buf, 0, dpos);
      GWEN_Buffer_SetPos(buf, dpos);
      GWEN_Buffer_AppendByte(buf, '/');
      GWEN_Buffer_AppendString(buf, buffer);
      DBG_INFO(0, "Checking file \"%s\"",
               GWEN_Buffer_GetStart(buf));

      if (stat(GWEN_Buffer_GetStart(buf), &st)) {
        DBG_ERROR(0, "stat(%s): %s",
                  GWEN_Buffer_GetStart(buf),
                  strerror(errno));
      }
      else {
        if (!S_ISDIR(st.st_mode)) {
          i=strlen(buffer);
          if (i>3) {
            if (strcasecmp(buffer+i-4, ".xml")==0) {
	      DBG_INFO(0, "Adding card file \"%s\"",
		       GWEN_Buffer_GetStart(buf));
	      GWEN_StringList_AppendString(mgr->cardFiles,
					   GWEN_Buffer_GetStart(buf),
                                           0, 1);
            } /* if name ends in ".xml" */
          } /* if name longer than 3 chars */
        } /* if it is not a folder */
      } /* if stat succeeded */
    } /* while */
    GWEN_Directory_Close(d);
  } /* if open succeeded */
  else {
    DBG_INFO(0, "Could not open dir \"%s\"",
             GWEN_Buffer_GetStart(buf));
  }
  GWEN_Directory_free(d);
  GWEN_Buffer_free(buf);
}



void LC_CardMgr_SampleFiles(LC_CARDMGR *mgr,
                            const GWEN_STRINGLIST *sl) {
  GWEN_STRINGLISTENTRY *se;
  GWEN_BUFFER *buf;

  DBG_DEBUG(0, "Sampling card and app files");
  buf=GWEN_Buffer_new(0, 256, 0, 1);
  se=GWEN_StringList_FirstEntry(sl);
  while(se) {
    /* sample card files */
    LC_CardMgr__SampleFiles(mgr, GWEN_StringListEntry_Data(se));
    se=GWEN_StringListEntry_Next(se);
  } /* while */
}



int LC_CardMgr_MergeXMLDefs(LC_CARDMGR *mgr,
                            GWEN_XMLNODE *destNode,
                            GWEN_XMLNODE *node) {
  GWEN_XMLNODE *nsrc, *ndst;

  assert(mgr);
  assert(node);

  nsrc=GWEN_XMLNode_GetChild(node);
  while(nsrc) {
    /* merge 1st level */
    if (GWEN_XMLNode_GetType(nsrc)==GWEN_XMLNodeTypeTag) {
      ndst=GWEN_XMLNode_FindFirstTag(destNode,
                                     GWEN_XMLNode_GetData(nsrc),
                                     "name",
                                     GWEN_XMLNode_GetProperty(nsrc,
                                                              "name",
                                                              ""));
      if (ndst) {
        GWEN_XMLNODE *nsrc2, *ndst2;

        /* merge 2nd level */
        DBG_DEBUG(0, "Merging tags from \"%s\" into \"%s\"",
                   GWEN_XMLNode_GetData(nsrc),
                   GWEN_XMLNode_GetData(ndst));
        nsrc2=GWEN_XMLNode_GetChild(nsrc);
        while(nsrc2) {
          if (GWEN_XMLNode_GetType(nsrc2)==GWEN_XMLNodeTypeTag) {
            ndst2=GWEN_XMLNode_FindNode(ndst,
                                        GWEN_XMLNodeTypeTag,
                                        GWEN_XMLNode_GetData(nsrc2));
            if (ndst2) {
              GWEN_XMLNODE *n;

              DBG_DEBUG(0,
                         "Level2: Merging tags from "
                         "\"%s\" into \"%s\"",
                         GWEN_XMLNode_GetData(nsrc2),
                         GWEN_XMLNode_GetData(ndst2));
              /* node found, copy branch */
              n=GWEN_XMLNode_GetChild(nsrc2);
              while (n) {
                GWEN_XMLNODE *newNode;

                DBG_DEBUG(0, "Adding node \"%s\"",
                           GWEN_XMLNode_GetData(n));
                newNode=GWEN_XMLNode_dup(n);
                GWEN_XMLNode_AddChild(ndst2, newNode);
                n=GWEN_XMLNode_Next(n);
              } /* while n */
            }
            else {
              GWEN_XMLNODE *newNode;

              DBG_DEBUG(0, "Adding branch \"%s\"",
                         GWEN_XMLNode_GetData(nsrc2));
              newNode=GWEN_XMLNode_dup(nsrc2);
              GWEN_XMLNode_AddChild(ndst, newNode);
            }
          } /* if TAG */
          nsrc2=GWEN_XMLNode_Next(nsrc2);
        } /* while there are 2nd level source tags */
      }
      else {
	GWEN_XMLNODE *newNode;

        DBG_DEBUG(0, "Adding branch \"%s\"",
                  GWEN_XMLNode_GetData(nsrc));
        newNode=GWEN_XMLNode_dup(nsrc);
        GWEN_XMLNode_AddChild(destNode, newNode);
      }
    } /* if TAG */
    nsrc=GWEN_XMLNode_Next(nsrc);
  } /* while */

  return 0;
}



int LC_CardMgr_LoadCard(LC_CARDMGR *mgr, const char *name) {
  GWEN_STRINGLISTENTRY *se;
  unsigned int j;
  unsigned int filesLoaded;

  assert(mgr);
  assert(name);
  if (GWEN_StringList_HasString(mgr->loadedCards, name)) {
    DBG_DEBUG(0, "Card type \"%s\" already loaded", name);
    return 0;
  }

  DBG_INFO(0, "Loading card type \"%s\"", name);

  filesLoaded=0;
  j=strlen(name);
  se=GWEN_StringList_FirstEntry(mgr->cardFiles);
  while(se) {
    unsigned int i;

    i=strlen(GWEN_StringListEntry_Data(se));
    if (i>j+3) {
      if (strncasecmp(GWEN_StringListEntry_Data(se)+i-4-j,
                      name, j)==0) {
        GWEN_XMLNODE *n;

        /* name matches */
        n=GWEN_XMLNode_new(GWEN_XMLNodeTypeTag, "card");
        if (GWEN_XML_ReadFile(n,
                              GWEN_StringListEntry_Data(se),
                              GWEN_XML_FLAGS_DEFAULT)) {
          DBG_ERROR(0, "Could not read XML file \"%s\"",
                    GWEN_StringListEntry_Data(se));
        }
        else {
          GWEN_XMLNODE *nn;

          nn=GWEN_XMLNode_FindNode(n, GWEN_XMLNodeTypeTag, "cards");
          if (!nn) {
            DBG_WARN(0, "File \"%s\" does not contain <cards>",
                     GWEN_StringListEntry_Data(se));
          }
          else {
            if (LC_CardMgr_MergeXMLDefs(mgr, mgr->xmlCards, nn)) {
              DBG_ERROR(0, "Could not merge file \"%s\"",
                        GWEN_StringListEntry_Data(se));
            }
            else {
              GWEN_StringList_AppendString(mgr->loadedCards,
                                           name,
                                           0, 1);
              GWEN_MsgEngine_SetDefinitions(mgr->msgEngine,
                                            mgr->xmlCards,
                                            0); /* dont take over */
              filesLoaded++;
            }
          }
        }
        GWEN_XMLNode_free(n);
      } /* if name matches */
    }
    se=GWEN_StringListEntry_Next(se);
  } /* while */

  if (!filesLoaded) {
    DBG_ERROR(0, "No files loaded");
    return -1;
  }
  return 0;
}



int LC_CardMgr_LoadApp(LC_CARDMGR *mgr, const char *name) {
  GWEN_STRINGLISTENTRY *se;
  unsigned int j;
  unsigned int filesLoaded;

  assert(mgr);
  assert(name);
  if (GWEN_StringList_HasString(mgr->loadedApps, name)) {
    DBG_DEBUG(0, "App type \"%s\" already loaded", name);
    return 0;
  }

  filesLoaded=0;
  j=strlen(name);
  se=GWEN_StringList_FirstEntry(mgr->appFiles);
  while(se) {
    unsigned int i;

    i=strlen(GWEN_StringListEntry_Data(se));
    if (i>j+3) {
      if (strncasecmp(GWEN_StringListEntry_Data(se)+i-4-j,
                      name, j)==0) {
        GWEN_XMLNODE *n;

        /* name matches */
        n=GWEN_XMLNode_new(GWEN_XMLNodeTypeTag, "app");
        if (GWEN_XML_ReadFile(n,
                              GWEN_StringListEntry_Data(se),
                              GWEN_XML_FLAGS_DEFAULT)) {
          DBG_ERROR(0, "Could not read XML file \"%s\"",
                    GWEN_StringListEntry_Data(se));
        }
        else {
          GWEN_XMLNODE *nn;

          nn=GWEN_XMLNode_FindNode(n, GWEN_XMLNodeTypeTag, "apps");
          if (!nn) {
            DBG_WARN(0, "File \"%s\" does not contain <apps>",
                     GWEN_StringListEntry_Data(se));
          }
          else {
            if (LC_CardMgr_MergeXMLDefs(mgr, mgr->xmlApps, nn)) {
              DBG_ERROR(0, "Could not merge file \"%s\"",
                        GWEN_StringListEntry_Data(se));
            }
            else {
              GWEN_StringList_AppendString(mgr->loadedApps,
                                           name,
                                           0, 1);
              filesLoaded++;
            }
          }
        }
        GWEN_XMLNode_free(n);
      } /* if name matches */
    }
    se=GWEN_StringListEntry_Next(se);
  } /* while */

  if (!filesLoaded) {
    DBG_ERROR(0, "No files loaded");
    return -1;
  }
  return 0;
}



int LC_CardMgr_SelectCard(LC_CARDMGR *mgr,
                          LC_CARD *card,
                          const char *cardName) {
  GWEN_XMLNODE *cardNode;
  LC_CARDCONTEXT *ctx;

  assert(cardName);

  if (LC_CardMgr_LoadCard(mgr, cardName)) {
    DBG_ERROR(0, "Card type \"%s\" not available", cardName);
    return LC_CardMgr_ResultCmdError;
  }
  cardNode=GWEN_XMLNode_FindFirstTag(mgr->xmlCards,
                                     "card",
                                     "name",
                                     cardName);
  assert(cardNode);
  DBG_NOTICE(0, "Card type \"%s\" selected", cardName);

  /* create context */
  ctx=LC_CardContext_new(mgr);
  LC_CardContext_SetCardNode(ctx, cardNode);

  /* set context */
  LC_Card_SetContext(card, ctx);
  return 0;
}



LC_CARDMGR_RESULT LC_CardMgr_HandleCommand(LC_CARDMGR *mgr,
                                           LC_CARD *card,
                                           GWEN_TYPE_UINT32 rid,
                                           GWEN_DB_NODE *dbReq,
                                           GWEN_DB_NODE *dbRsp){
  LC_CARDCONTEXT *ctx;

  assert(mgr);
  assert(card);
  ctx=LC_Card_GetContext(card);

  if (!ctx) {
    DBG_ERROR(0, "No card/app selected");
    GWEN_DB_GroupRename(dbRsp, "error");
    GWEN_DB_SetIntValue(dbRsp, GWEN_DB_FLAGS_OVERWRITE_VARS,
                        "code", LC_ERROR_INVALID);
    GWEN_DB_SetCharValue(dbRsp, GWEN_DB_FLAGS_OVERWRITE_VARS,
                         "text", "No card/app selected");
    return LC_CardMgr_ResultCmdError;
  }

  return LC_CardContext_BuildCmd(ctx,
                                 card,
                                 rid,
                                 dbReq,
                                 dbRsp);
}



LC_CARDMGR_RESULT LC_CardMgr_CheckResponse(LC_CARDMGR *mgr,
                                           LC_REQUEST *rq,
                                           GWEN_DB_NODE *dbDriverRsp,
                                           GWEN_DB_NODE *dbRsp){
  LC_CARDCONTEXT *ctx;
  LC_CARD *card;

  assert(mgr);
  assert(rq);
  card=LC_Request_GetCard(rq);
  assert(card);
  ctx=LC_Card_GetContext(card);

  if (!ctx) {
    DBG_ERROR(0, "No card/app selected");
    GWEN_DB_GroupRename(dbRsp, "error");
    GWEN_DB_SetIntValue(dbRsp, GWEN_DB_FLAGS_OVERWRITE_VARS,
                        "code", LC_ERROR_INVALID);
    GWEN_DB_SetCharValue(dbRsp, GWEN_DB_FLAGS_OVERWRITE_VARS,
                         "text", "No card/app selected");
    return LC_CardMgr_ResultCmdError;
  }

  return LC_CardContext_CheckCmd(ctx,
                                 rq,
                                 dbDriverRsp,
                                 dbRsp);
}



GWEN_XMLNODE *LC_CardMgr_FindCardNode(LC_CARDMGR *mgr,
                                      const char *cardName){

  if (LC_CardMgr_LoadCard(mgr, cardName)) {
    DBG_ERROR(0, "Card type \"%s\" not available", cardName);
    return 0;
  }
  return GWEN_XMLNode_FindFirstTag(mgr->xmlCards,
                                   "card",
                                   "name",
                                   cardName);
}



GWEN_XMLNODE *LC_CardMgr_FindAppNode(LC_CARDMGR *mgr,
                                     const char *appName){
  if (LC_CardMgr_LoadApp(mgr, appName)) {
    DBG_ERROR(0, "Card application \"%s\" not available", appName);
    return 0;
  }
  return GWEN_XMLNode_FindFirstTag(mgr->xmlApps,
                                   "app",
                                   "name",
                                   appName);
}






















