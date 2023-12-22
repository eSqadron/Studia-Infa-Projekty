//
// Created by OEM on 18.12.2020.
//

#include <memory>
#include "storage_types.hpp"

Package PackageQueue::pop(){
    if (queue_type_ == PackageQueueType::LIFO){
        auto el = std::move(queue_.back());
        queue_.pop_back();
        return el;
    }
    else if (queue_type_ == PackageQueueType::FIFO){
        Package el = std::move(queue_.front());
        queue_.pop_front();
        return el;
    }
    else{
        throw "Bad queue type!";
    }
}

