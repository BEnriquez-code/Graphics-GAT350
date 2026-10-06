#pragma once
#include "Framework/Object.h"
#include "Singleton.h"
#include "StringUtils.h"

#include <iostream>
#include <map>
#include <memory>


#define FACTORY_REGISTER(classname)                                 \
    class Register##classname                                       \
    {                                                               \
    public:                                                         \
        Register##classname()                                       \
        {                                                           \
            nu::Factory::Instance().Register<classname>(#classname);\
        }                                                           \
    };                                                              \
    static Register##classname registerInstance;

namespace nu {
    class ICreator {
    public:
        virtual ~ICreator() = default;
        virtual std::unique_ptr<Object> Create() = 0;
    };

    template<typename T>
        requires std::derived_from<T, Object>
    class Creator : public ICreator {

    public:
        std::unique_ptr<Object> Create() override { return std::make_unique<T>(); }
    };

    template<typename T>
        requires std::derived_from<T, Object>
    class PrototypeCreator : public ICreator {

    public:
        PrototypeCreator(std::unique_ptr<Object> prototype) :
            m_prototype{ std::move(prototype) } {}

        std::unique_ptr<Object> Create() override { 
            return m_prototype->Clone();
        }
    private:
        std::unique_ptr<Object> m_prototype;
    };

    class Factory : public Singleton<Factory> {
    public:
        template<typename T>
            requires std::derived_from<T, Object>
        void Register(const std::string& name);

        template<typename T>
            requires std::derived_from<T, Object>
        void RegisterPrototype(const std::string& name, std::unique_ptr<T> prototype);

        template<typename T = class Object>
            requires std::derived_from<T, Object>
        std::unique_ptr<T> Create(const std::string& name);

    private:
        std::map<std::string, std::unique_ptr<ICreator>> m_registry;
    };

    template<typename T>
        requires std::derived_from<T, Object>
    inline void Factory::Register(const std::string& name) {


        if (m_registry.contains(name)) {
            std::cerr << "Object already registered: " << name << std::endl;
            return;
        }
        m_registry[name] = std::make_unique<Creator<T>>();
        std::cout << "success: registered object " << name << std::endl;
    }

    template<typename T>
        requires std::derived_from<T, Object>
    inline void Factory::RegisterPrototype(const std::string& name, std::unique_ptr<T> prototype){

        if (m_registry.contains(name)) {
            std::cerr << "Object already registered prototype: " << name << std::endl;
            return;
        }
        m_registry[std::move(name)] = std::make_unique<PrototypeCreator<T>>(std::move(prototype));
        std::cout << "success: registered prototype " << name << std::endl;
    }


    template<typename T>
        requires std::derived_from<T, Object>
    inline std::unique_ptr<T> Factory::Create(const std::string& name){

        auto iter = m_registry.find(name);
        if (iter == m_registry.end()) {
            std::cerr << "Object not registered: " << name << std::endl;
            return nullptr;
        }

        //create unique pointer to object
        auto object = iter->second->Create();

        //check if object is derived from T
        if (T* derived = dynamic_cast<T*>(object.get())) {
            //release unique pointer ownership
            object.release();
            std::cout << "success: created object " << name << std::endl;

            //create new unique pointer wiht derived pointer
            return std::unique_ptr<T>(derived);
        }
        std::cerr << "Object " << name << " failed to dynamiclly cast" << std::endl;
        return nullptr;
    }

}