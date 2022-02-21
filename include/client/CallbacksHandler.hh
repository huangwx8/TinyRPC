#pragma once

#include <string>
#include <vector>
#include <functional>

#include <runtime/handlemodel/EventHandler.hh>

class CallbackFunction
{
public:
    CallbackFunction();
    ~CallbackFunction();
    
    void Exec(int);
    void Exec(float);
    void Exec(std::string);

    void Register(std::function<void(int)>);
    void Register(std::function<void(float)>);
    void Register(std::function<void(std::string)>);
    
private:
    void Free();

    std::function<void(int)>* IntCallback;
    std::function<void(float)>* FloatCallback;
    std::function<void(std::string)>* StringCallback;
};

class CallbacksHandler: public EventHandler
{
public:
    CallbacksHandler();
    ~CallbacksHandler();
    /**
     * When READ event triggered at a connected fd
     * it should read rpc result and execute corresponding callback function
     */
    virtual void HandleReadEvent(int Fd) override;

    void Register(int, std::function<void(int)>);
    void Register(int, std::function<void(float)>);
    void Register(int, std::function<void(std::string)>);

private:
    std::vector<CallbackFunction> CallidCallbackMapping;
};