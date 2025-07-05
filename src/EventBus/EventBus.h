#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <list>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Event.h"
#include "../Log/Logger.h"

class IEventCallback {
public:
    virtual ~IEventCallback() = default;
    
    void Execute(Event& e) {
        Call(e);
    }
private:
    virtual void Call(Event& e) = 0;
};

template<typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
typedef void (TOwner::*CallbackFunction)(TEvent&);

public:
    EventCallback(TOwner* owner, CallbackFunction callback) : owner(owner), callbackFunction(callback) {}
    virtual ~EventCallback() override = default;

private:
    TOwner* owner;
    CallbackFunction callbackFunction;

    void Call(Event& e) override {
        std::invoke(callbackFunction, owner, static_cast<TEvent&>(e)); // 注意这里是转换引用而不是对象
    }
};


typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;
class EventBus {
public:
    std::unordered_map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

    EventBus() {
        Logger::Log("EventBus Constructed");
    }

    ~EventBus() {
        Logger::Log("EventBus Destroyed");
    }

    void Reset() {
            subscribers.clear();
        }

    template<typename TEvent, typename TOwner>
    void Subscribe(TOwner* owner, void (TOwner::*callbackFunction)(TEvent&)) {
        if (subscribers.find(typeid(TEvent)) == subscribers.end()) {
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }

        subscribers[typeid(TEvent)]->push_back(std::make_unique<EventCallback<TOwner, TEvent>>(owner, callbackFunction));
        Logger::Log("Subscribed to event type: " + std::string(typeid(TEvent).name()));
    }

    template<typename TEvent, typename... TArgs>
    void EmitEvent(TArgs... args) {
        if (subscribers.find(typeid(TEvent)) == subscribers.end()) {
            Logger::Err("No handlers found for event type: " + std::string(typeid(TEvent).name()));
            return;
        }

        HandlerList* handlers = subscribers[typeid(TEvent)].get();
        TEvent e(args...);
        for (auto& handler : *handlers) {
            handler->Execute(e);
        }
    }
};

#endif