#include "stdio.h"

#include <client/CallbacksHandler.hh>

void CallbacksHandler::HandleReadEvent(int Fd)
{
    printf("CallbacksHandler::HandleReadEvent Fd = [%d]\n", Fd);
}
