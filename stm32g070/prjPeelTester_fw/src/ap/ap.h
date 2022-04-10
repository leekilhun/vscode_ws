#pragma once

#include "ap_def.h"

/* engine layer*/
#include "enCyl.h"
#include "enVac.h"
#include "enOP.h"
#include "enFaxis.h"

/* control layer*/
#include "cnAutomanager.h"
#include "cnProcess.h"

/* api layer*/
#include "apiNextion.h"
#include "apiRemote.h"

void apInit(void);
void apMain(void);

