#pragma once

#include <runtime/handlemodel/EventHandler.hh>

class CallbacksHandler: public EventHandler
{
    /**
     * When READ event triggered at a connected fd
     * it should read rpc result and execute corresponding callback function
     */
    virtual void HandleReadEvent(int Fd) override;
};