#pragma once
/*
 * ap_evt.h
 *
 * 작성자 : 이길훈
 * 22.04.05
 *  콜백, 메세지 이벤트 객체
 */


/****************************************************
 *	고민 중
 ****************************************************/


typedef void (*msg_cb)(void* obj, void* w_parm, void* l_param); // 콜백 타입 정의