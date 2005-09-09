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


#ifndef CHIPCARD_CLIENT_CARD_P_H
#define CHIPCARD_CLIENT_CARD_P_H


#include "card_l.h"
#include <gwenhywfar/inherit.h>


struct LC_CARD {
  GWEN_LIST_ELEMENT(LC_CARD)
  GWEN_INHERIT_ELEMENT(LC_CARD)
  LC_CLIENT *client;
  GWEN_TYPE_UINT32 cardId;
  char *readerType;
  char *driverType;
  GWEN_TYPE_UINT32 readerFlags;
  char *cardType;
  GWEN_TYPE_UINT32 serverId;
  GWEN_BUFFER *atr;

  GWEN_STRINGLIST *cardTypes;
  char *selectedApp;

  LC_CARDCONTEXT *context;

  LC_CARD_OPEN_FN openFn;
  LC_CARD_CLOSE_FN closeFn;

  LC_CARD_GETPINSTATUS_FN getPinStatusFn;
  LC_CARD_GETINITIALPIN_FN getInitialPinFn;

  LC_CARD_ISOREADBINARY_FN readBinaryFn;
  LC_CARD_ISOWRITEBINARY_FN writeBinaryFn;
  LC_CARD_ISOUPDATEBINARY_FN updateBinaryFn;
  LC_CARD_ISOERASEBINARY_FN eraseBinaryFn;
  LC_CARD_ISOREADRECORD_FN readRecordFn;
  LC_CARD_ISOWRITERECORD_FN writeRecordFn;
  LC_CARD_ISOAPPENDRECORD_FN appendRecordFn;
  LC_CARD_ISOUPDATERECORD_FN updateRecordFn;
  LC_CARD_ISOVERIFYPIN_FN verifyPinFn;
  LC_CARD_ISOMODIFYPIN_FN modifyPinFn;

  LC_CARD_ISOPERFORMVERIFICATION_FN performVerificationFn;
  LC_CARD_ISOPERFORMMODIFICATION_FN performModificationFn;

  LC_CARD_ISOMANAGESE_FN manageSeFn;
  LC_CARD_ISOSIGN_FN signFn;
  LC_CARD_ISOVERIFY_FN verifyFn;
  LC_CARD_ISOENCIPHER_FN encipherFn;
  LC_CARD_ISODECIPHER_FN decipherFn;

  int lastSW1;
  int lastSW2;
  char *lastResult;
  char *lastText;

  GWEN_DB_NODE *driverVars;
};

LC_CLIENT_RESULT LC_Card__Open(LC_CARD *card);
LC_CLIENT_RESULT LC_Card__Close(LC_CARD *card);


LC_CLIENT_RESULT LC_Card__IsoReadBinary(LC_CARD *card,
                                        GWEN_TYPE_UINT32 flags,
                                        int offset,
                                        int size,
                                        GWEN_BUFFER *buf);
LC_CLIENT_RESULT LC_Card__IsoUpdateBinary(LC_CARD *card,
                                          GWEN_TYPE_UINT32 flags,
                                          int offset,
                                          const char *ptr,
                                          unsigned int size);
LC_CLIENT_RESULT LC_Card__IsoWriteBinary(LC_CARD *card,
                                         GWEN_TYPE_UINT32 flags,
                                         int offset,
                                         const char *ptr,
                                         unsigned int size);
LC_CLIENT_RESULT LC_Card__IsoEraseBinary(LC_CARD *card,
                                         GWEN_TYPE_UINT32 flags,
                                         int offset,
                                         unsigned int size);
LC_CLIENT_RESULT LC_Card__IsoReadRecord(LC_CARD *card,
                                        GWEN_TYPE_UINT32 flags,
                                        int recNum,
                                        GWEN_BUFFER *buf);
LC_CLIENT_RESULT LC_Card__IsoWriteRecord(LC_CARD *card,
                                         GWEN_TYPE_UINT32 flags,
                                         int recNum,
                                         const char *ptr,
                                         unsigned int size);
LC_CLIENT_RESULT LC_Card__IsoUpdateRecord(LC_CARD *card,
                                          GWEN_TYPE_UINT32 flags,
                                          int recNum,
                                          const char *ptr,
                                          unsigned int size);
LC_CLIENT_RESULT LC_Card__IsoAppendRecord(LC_CARD *card,
                                          GWEN_TYPE_UINT32 flags,
                                          const char *ptr,
                                          unsigned int size);
LC_CLIENT_RESULT LC_Card__IsoVerifyPin(LC_CARD *card,
                                       GWEN_TYPE_UINT32 flags,
                                       const LC_PININFO *pi,
                                       const char *ptr,
                                       unsigned int size,
                                       int *triesLeft);

LC_CLIENT_RESULT LC_Card__IsoPerformVerification(LC_CARD *card,
                                                 GWEN_TYPE_UINT32 flags,
                                                 const LC_PININFO *pi,
                                                 int *triesLeft);

LC_CLIENT_RESULT LC_Card__IsoModifyPin(LC_CARD *card,
                                       GWEN_TYPE_UINT32 flags,
                                       const LC_PININFO *pi,
                                       const char *oldptr,
                                       unsigned int oldsize,
                                       const char *newptr,
                                       unsigned int newsize,
                                       int *triesLeft);

LC_CLIENT_RESULT LC_Card__IsoPerformModification(LC_CARD *card,
                                                 GWEN_TYPE_UINT32 flags,
                                                 const LC_PININFO *pi,
                                                 int *triesLeft);


LC_CLIENT_RESULT LC_Card__IsoManageSe(LC_CARD *card,
                                      int tmpl, int kids, int kidp, int ar);
LC_CLIENT_RESULT LC_Card__IsoEncipher(LC_CARD *card,
                                      const char *ptr,
                                      unsigned int size,
                                      GWEN_BUFFER *codeBuf);
LC_CLIENT_RESULT LC_Card__IsoDecipher(LC_CARD *card,
                                      const char *ptr,
                                      unsigned int size,
                                      GWEN_BUFFER *plainBuf);


GWEN_XMLNODE *LC_Card__GetEfInfo(const LC_CARD *card);
GWEN_XMLNODE *LC_Card__GetDfInfo(const LC_CARD *card);
GWEN_XMLNODE *LC_Card__GetAppInfo(const LC_CARD *card);











#endif /* CHIPCARD_CLIENT_CARD_P_H */


