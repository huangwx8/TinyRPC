#pragma once

/**
 * 事件处理抽象类
 */
class EventHandler 
{
public:
    enum EventType
    {
        READ_EVENT = 0x01,
        WRITE_EVENT = 0x02,
        CLOSE_EVENT = 0x04,
        MAX_EVENT = 0xff,
    };

    // Event id 优先返回 CLOSE, 其次WRITE, 最后READ 
    static inline int EventTypeToEventId(EventType Type)
    {
        static int LookUpTable[8] = {-1, 0, 1, 1, 2, 2, 2, 2};
        int low3bits = Type & 7;
        return LookUpTable[low3bits];
    }

public:
    EventHandler() = default;
    virtual ~EventHandler() = default;

    virtual void HandleEvent(int Fd, EventType Type);
    virtual void HandleReadEvent(int Fd) {};
    virtual void HandleWriteEvent(int Fd) {};
    virtual void HandleCloseEvent(int Fd) {};
};
