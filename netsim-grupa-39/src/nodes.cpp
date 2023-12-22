//
// Created by OEM on 22.12.2020.
//

#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver* r)  {
    pref_[r] = 1.0;
    //pref_[r] = 1.0/(pref_.size()+1);
    recalculate();
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    pref_.erase(r);
    recalculate();
}

void ReceiverPreferences::recalculate(void) {
    double t = 1.0/pref_.size();
    //const double t = std::accumulate(pref_.begin(), pref_.end(),0.0, [&] (const double& y, const std::pair<IPackageReceiver*, double>& x){ return x.second + y;} );

    for (auto i = pref_.begin(); i != pref_.end(); i++) {
        i->second = t;
        //i->second = i->second / t;
        //t+=t;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver(void) {
    double prob = pg_();
    preferences_t::iterator i = pref_.begin();
    double p = 0;
    while (p < prob) {
        p = p + i->second;
        ++i;
    }

    return (--i)->first;
}


void Worker::do_work(Time t) {
    if (!WIP_buffer_.has_value()){
        t_ = t;
        if(!q_->empty()){
            WIP_buffer_ = std::move(q_->pop());
        }

    }

    if(WIP_buffer_.has_value() and t == t_ + pd_ - 1){
        push_package(std::move(*WIP_buffer_));
        WIP_buffer_ = std::optional<Package>(std::nullopt);
    }
}

void PackageSender::send_package(void)  {
    if(buffer_.has_value()){
        receiver_preferences_.choose_receiver()->receive_package(std::move(*buffer_));
        buffer_ = std::optional<Package>(std::nullopt);
    }
}
