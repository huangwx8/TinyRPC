#include <runtime/handlemodel/EventHandler.hh>


void EventHandler::HandleEvent(int Fd, EventType Type)
{
    switch (Type)
    {
    case CLOSE_EVENT:
        HandleCloseEvent(Fd);
        break;
    case READ_EVENT:
        HandleReadEvent(Fd);
        break;
    case WRITE_EVENT:
        HandleWriteEvent(Fd);
        break;
    default:
        break;
    }
}
