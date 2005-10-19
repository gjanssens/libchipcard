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


#ifndef CHIPCARD_SERVER_DEVMONITOR_H
#define CHIPCARD_SERVER_DEVMONITOR_H

#define LC__DEVICE_FILE "/var/run/chipcard2/dev.state"


typedef struct LC_DEVMONITOR LC_DEVMONITOR;
typedef struct LC_DEVICE LC_DEVICE;

#include <gwenhywfar/idlist.h>
#include <gwenhywfar/misc.h>
#include <gwenhywfar/inherit.h>


typedef enum {
  LC_Device_BusType_Unknown=-1,
  LC_Device_BusType_Any=0,
  LC_Device_BusType_UsbRaw,
  LC_Device_BusType_UsbTty,
  LC_Device_BusType_Pci,
  LC_Device_BusType_Pcmcia,
  LC_Device_BusType_Serial
} LC_DEVICE_BUSTYPE;

LC_DEVICE_BUSTYPE LC_Device_BusType_fromString(const char *s);
const char *LC_Device_BusType_toString(LC_DEVICE_BUSTYPE i);


GWEN_LIST_FUNCTION_DEFS(LC_DEVICE, LC_Device)
GWEN_INHERIT_FUNCTION_DEFS(LC_DEVMONITOR)


typedef int (*LC_DEVMONITOR_READ_DEVS_FN)(LC_DEVMONITOR *um,
                                          LC_DEVICE_LIST *dl);


LC_DEVMONITOR *LC_DevMonitor_new();
void LC_DevMonitor_SetReadDevsFn(LC_DEVMONITOR *um,
                                 LC_DEVMONITOR_READ_DEVS_FN fn);

void LC_DevMonitor_free(LC_DEVMONITOR *um);

int LC_DevMonitor_Scan(LC_DEVMONITOR *um);

LC_DEVICE_LIST *LC_DevMonitor_GetNewDevices(const LC_DEVMONITOR *um);
LC_DEVICE_LIST *LC_DevMonitor_GetLostDevices(const LC_DEVMONITOR *um);
LC_DEVICE_LIST *LC_DevMonitor_GetCurrentDevices(const LC_DEVMONITOR *um);




LC_DEVICE *LC_Device_new(LC_DEVICE_BUSTYPE busType,
                         GWEN_TYPE_UINT32 busId,
                         GWEN_TYPE_UINT32 deviceId,
                         GWEN_TYPE_UINT32 vendorId,
                         GWEN_TYPE_UINT32 productId);
void LC_Device_free(LC_DEVICE *ud);


LC_DEVICE *LC_Device_List_Find(LC_DEVICE_LIST *dl,
                               LC_DEVICE_BUSTYPE busType,
                               GWEN_TYPE_UINT32 busId,
                               GWEN_TYPE_UINT32 deviceId,
                               GWEN_TYPE_UINT32 vendorId,
                               GWEN_TYPE_UINT32 productId);

LC_DEVICE *LC_Device_Get(LC_DEVICE_LIST *dl,
                         LC_DEVICE_BUSTYPE busType,
                         GWEN_TYPE_UINT32 dpos);


GWEN_TYPE_UINT32 LC_Device_GetDevicePos(const LC_DEVICE *ud);
void LC_Device_SetDevicePos(LC_DEVICE *ud, GWEN_TYPE_UINT32 i);
GWEN_TYPE_UINT32 LC_Device_GetBusId(const LC_DEVICE *ud);
GWEN_TYPE_UINT32 LC_Device_GetDeviceId(const LC_DEVICE *ud);
GWEN_TYPE_UINT32 LC_Device_GetVendorId(const LC_DEVICE *ud);
GWEN_TYPE_UINT32 LC_Device_GetProductId(const LC_DEVICE *ud);




#endif /* CHIPCARD_SERVER_DEVMONITOR_H */