//
// Created by OEM on 18.12.2020.
//
#ifndef NET_SIMULATION_STORAGE_TYPES_HPP
#define NET_SIMULATION_STORAGE_TYPES_HPP

#include <set>
#include <list>
#include "types.hpp"
#include "package.hpp"


enum class PackageQueueType{FIFO, LIFO};



class IPackageStockpile{
public:

    using const_iterator = std::list<Package>::const_iterator;
    //using iterator = std::list<Package>::iterator;

    virtual void push(Package&& x) = 0;

    //virtual iterator begin() = 0;
    virtual const_iterator begin() const = 0;
    virtual const const_iterator cbegin() const = 0;

    //virtual iterator end() = 0;
    virtual const_iterator end() const = 0;
    virtual const const_iterator cend() const = 0;

    virtual size_t size() const = 0;

    virtual bool empty() = 0;

    virtual ~IPackageStockpile() = default;

private:

};

class IPackageQueue : public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;

};


class PackageQueue : public IPackageQueue{
public:

    //using const_iterator = std::list<Package>::const_iterator;

    PackageQueue(PackageQueueType queue_type) : queue_type_(queue_type) {};

    //virtual iterator begin() override {return queue_.begin();}
    virtual const_iterator begin() const override {return queue_.cbegin();}
    virtual const const_iterator cbegin() const override {return queue_.cbegin();}

    //virtual iterator end() override  {return queue_.end();}
    virtual const_iterator end() const override  {return queue_.cend();};
    virtual const const_iterator cend() const override {return queue_.cend();};

    size_t size() const override {return queue_.size();}

    bool empty() override  {return size() == 0;}

    void push(Package&& x) override {
        queue_.emplace_back(
                std::move(x));}

    Package pop() override;

    PackageQueueType get_queue_type() const override {return queue_type_;}

    //~PackageQueue() = default;

private:
    PackageQueueType queue_type_;
    std::list<Package> queue_;
};

#endif //NET_SIMULATION_STORAGE_TYPES_HPP
