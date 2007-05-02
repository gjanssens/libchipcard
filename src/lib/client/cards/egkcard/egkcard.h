/***************************************************************************
 $RCSfile$
                             -------------------
    cvs         : $Id: ddvcard.h 233 2006-09-11 15:18:03Z martin $
    begin       : Mon Mar 01 2004
    copyright   : (C) 2004 by Martin Preuss
    email       : martin@libchipcard.de

 ***************************************************************************
 *          Please see toplevel file COPYING for license details           *
 ***************************************************************************/


#ifndef CHIPCARD_CARD_EGKCARD_H
#define CHIPCARD_CARD_EGKCARD_H

#include <chipcard/client/card.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup chipcardc_cards_egk eGK Cards
 * @ingroup chipcardc_cards
 *
 * <p>
 *  eGK cards are new German medical cards ("Elektronische Gesundheitskarte").
 * </p>
 *
 */
/*@{*/

/** @name Extending Basic Card Object
 *
 */
/*@{*/
/**
 * Extends a basic card type thus making functions of this group available.
 * This stores some DDV-related data with the given card object.
 * This function internally calls @ref LC_ProcessorCard_ExtendCard.
 */
CHIPCARD_API int LC_EgkCard_ExtendCard(LC_CARD *card);

/**
 * Unextend a card object which has previously been extended using
 * @ref LC_EgkCard_ExtendCard. This functions releases all DDV-related
 * ressources.
 * This function internally calls @ref LC_ProcessorCard_UnextendCard.
 */
CHIPCARD_API int LC_EgkCard_UnextendCard(LC_CARD *card);

/**
 * This function is called from within LC_Card_Open for cards which have been
 * extended using @ref LC_EgkCard_ExtendCard. However, if a card extended
 * after opening you can call this function here to let the card do some
 * necessary work before other functions of this group can be used.
 */
CHIPCARD_API LC_CLIENT_RESULT LC_EgkCard_Reopen(LC_CARD *card);
/*@}*/

/** @name Pin Verification
 *
 */
/*@{*/
/**
 * Verify the given pin.
 */
CHIPCARD_API LC_CLIENT_RESULT LC_EgkCard_VerifyPin(LC_CARD *card,
                                                   const char *pin);
/**
 * Secure pin verification. This can be used if the card's reader flags
 * indicate that the reader has a keypad.
 */
CHIPCARD_API LC_CLIENT_RESULT LC_EgkCard_SecureVerifyPin(LC_CARD *card);
/*@}*/


CHIPCARD_API LC_CLIENT_RESULT LC_EgkCard_ReadPd(LC_CARD *card,
						GWEN_BUFFER *buf);

CHIPCARD_API LC_CLIENT_RESULT LC_EgkCard_ReadVd(LC_CARD *card,
						GWEN_BUFFER *buf);

/*@}*/ /* defgroup */


#ifdef __cplusplus
}
#endif


#endif /* CHIPCARD_CARD_EGKCARD_H */

