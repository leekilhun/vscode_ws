/*
 * def_obj.h
 *
 *  Created on: 2022. 4. 18.
 *      Author: gns2.lee
 */

#ifndef SRC_DEF_OBJ_H_
#define SRC_DEF_OBJ_H_

#define AP_DEF_OBJ_BASE_MOTOR                0x00
#define AP_DEF_OBJ_BASE_CYLINDER             0x10
#define AP_DEF_OBJ_BASE_VACUUM               0X20

#define AP_DEF_OBJ_MOTOR_ID_0                1


#define AP_DEF_OBJ_CYLINDER_ID_PHONE_JIG     0
#define AP_DEF_OBJ_CYLINDER_ID_DRUM_UPDOWN   1
#define AP_DEF_OBJ_CYLINDER_ID_DRUM_Z_UP     2
#define AP_DEF_OBJ_CYLINDER_ID_DRUM_STOP     3
#define AP_DEF_OBJ_CYLINDER_ID_MAX           4
#define GetCylObjId(obj)                     AP_DEF_OBJ_BASE_CYLINDER|obj


#define AP_DEF_OBJ_VACUUM_ID_DRUM_HEAD       0
#define AP_DEF_OBJ_VACUUM_ID_DRUM_TAIL       1
#define AP_DEF_OBJ_VACUUM_ID_PHONE_JIG       2
#define AP_DEF_OBJ_VACUUM_ID_MAX             3
#define GetVacObjId(obj)                     AP_DEF_OBJ_BASE_VACUUM|obj


#endif /* SRC_DEF_OBJ_H_ */
