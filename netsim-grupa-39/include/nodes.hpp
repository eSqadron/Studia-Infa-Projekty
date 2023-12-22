//
// Created by OEM on 22.12.2020.
//

#ifndef NET_SIMULATION_NODES_HPP
#define NET_SIMULATION_NODES_HPP

#include <memory>
#include <map>
#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"


enum class ReceiverType{WORKER, STOREHOUSE};

class IPackageReceiver{
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id(void) const = 0;
    virtual ReceiverType get_receiver_type(void) const = 0;

    using const_iterator = IPackageStockpile::const_iterator;

    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
};

class Storehouse : public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))) : id_(id), d_(std::move(d)) {};
    //Storehouse(Storehouse& other) : id_(other.id_), d_(std::move(other.d_)) {};
    //Storehouse(Storehouse other) : id_(other.id_), d_(std::move(other.d_)) {};

    void receive_package(Package&& p) override { d_->push(std::move(p)); };

    ElementID get_id(void) const override { return id_;};

    ReceiverType get_receiver_type(void) const override {return ReceiverType::STOREHOUSE;};
    std::shared_ptr<IPackageStockpile> get_stock() const {return d_;};

    const_iterator cbegin() const override {return d_->cbegin(); };
    const_iterator cend() const override {return d_->cend(); };

    const_iterator begin() const override {return d_-> cbegin(); };
    const_iterator end() const override {return d_->cend(); };
private:
    ElementID id_;
    std::shared_ptr<IPackageStockpile> d_;

};

class ReceiverPreferences{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator): pg_(pg) {};

    const_iterator begin() const {return pref_.cbegin();};
    const_iterator cbegin() const {return pref_.cbegin();};

    const_iterator end() const {return pref_.cend();};
    const_iterator cend() const {return pref_.cend();};

    /**
     * W tym momencie funkcja daje każdemu dodanemu r takie samo prawdopodobieństwo, rozkłąd jest tylko inny.
     * @param r
     */
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);

    void recalculate(void);

    IPackageReceiver* choose_receiver(void); //W choose_receiver zakładam że wszystko się sumuje do 1, pomimo że tak nie jest
    const preferences_t& get_preferences(void) const {return pref_;};
private:
    preferences_t pref_;
    ProbabilityGenerator pg_;

};

class PackageSender{
public:
    //PackageSender() : receiver_preferences_() {};
    PackageSender(ReceiverPreferences receiver_preferences = ReceiverPreferences(probability_generator))  : receiver_preferences_(receiver_preferences) {};
    //PackageSender(PackageSender&) = default;
    PackageSender(PackageSender&&) = default;

    void send_package(void);

    const std::optional<Package>& get_sending_buffer(void) const { return buffer_; };

    ReceiverPreferences receiver_preferences_;
protected:
    //żółty przewrócony kwadrat na UML oznacza składową potected
    void push_package(Package&& p) { buffer_ = std::move(p); };
private:
    std::optional<Package> buffer_;
};

class Ramp: public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di) : ID_(id), di_(di) {};
    //Ramp(Ramp& r) = default;
    void deliver_goods(Time t) {if (!((t-1)%di_)) push_package(Package());};

    TimeOffset get_delivery_interval(void) const {return di_;};
    ElementID get_id(void) const {return ID_;};
private:
    ElementID ID_;
    TimeOffset di_;
};

class Worker : public IPackageReceiver, public PackageSender{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : ID_(id), pd_(pd), q_(std::move(q)) {};
    //Worker(Worker& w) : IPackageReceiver(w), PackageSender(w), ID_(w.ID_), pd_(w.pd_), q_(std::move(w.q_)), t_(w.t_), empty_(w.empty_), processed_package_(w.processed_package_) {};
    void do_work(Time t);
    TimeOffset get_processing_duration(void) const {return pd_;};
    Time get_package_processing_start_time(void) const {return t_;};
    void receive_package(Package&& p) override { q_->push(std::move(p)); };
    ElementID get_id(void) const override { return ID_;};
    const std::optional<Package>& get_processing_buffer(void) const {return WIP_buffer_;};
    ReceiverType get_receiver_type(void) const override {return ReceiverType::WORKER;};

    const IPackageQueue* get_queue(void) const {return &*q_; }

    const_iterator cbegin() const override {return q_->cbegin(); };
    const_iterator cend() const override {return q_->cend(); };

    const_iterator begin() const override {return q_-> cbegin(); };
    const_iterator end() const override {return q_->cend(); };

private:
    const ElementID ID_;
    const TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
    Time t_;
    std::optional<Package> WIP_buffer_;

};



#endif //NET_SIMULATION_NODES_HPP
