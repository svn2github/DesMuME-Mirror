// LzOutWindow.cpp

#include "StdAfx.h"

#include "LzOutWindow.h"

void CLzOutWindow::Init(bool solid)
{
  if (!solid)
    COutBuffer::Init();
  #ifdef _NO_EXCEPTIONS
  ErrorCode = S_OK;
  #endif
}
