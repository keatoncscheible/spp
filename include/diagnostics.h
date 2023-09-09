/******************************************************************************
 * Filename:    diagnostics.h
 * Copyright (c) 2023 Keaton Scheible
 *****************************************************************************/

#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

/***********************************************
Defines
***********************************************/
#define DIAGNOSTICS_FOLDER "diagnostics"
#define DIAGNOSTICS_FILE DIAGNOSTICS_FOLDER "/diagnostics_out.txt"

/***********************************************
Function Prototypes
***********************************************/
void* DiagnosticsTask(void* arg);

#endif  // DIAGNOSTICS_H
