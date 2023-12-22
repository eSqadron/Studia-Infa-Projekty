//
// Created by OEM on 22.12.2020.
//

#include "factory.hpp"
#include <istream>
#include <sstream>

using namespace std;

void Factory::do_deliveries(Time t) {
    for (NodeCollection<Ramp>::iterator i = ramps_.begin(); i != ramps_.end(); i++){
        i->deliver_goods(t);
    }
}

void Factory::do_package_passing(void) {
    for (NodeCollection<Ramp>::iterator i = ramps_.begin(); i != ramps_.end(); i++){
        i->send_package();
    }
    for (NodeCollection<Worker>::iterator i = workers_.begin(); i != workers_.end(); i++){
        i->send_package();
    }
}

void Factory::do_work(Time t) {
    for (NodeCollection<Worker>::iterator i = workers_.begin(); i != workers_.end(); i++){
        i->do_work(t);
    }
}

template<class Node>
void NodeCollection<Node>::sort_nodes(void) {
    unsigned int z = l_.cbegin()->get_id();
    bool sorted = false;
    for(NodeCollection<Node>::const_iterator i = l_.cbegin(); i != l_.cend(); i++) {
        if(i->get_id() < z){
            sorted = true;
            break;
        }
        z = i->get_id();
    }
    if(sorted){l_.sort(comp);}
}

template<typename Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    typename NodeCollection<Node>::iterator w = collection.find_by_id(id);
    for (typename NodeCollection<Ramp>::iterator i = ramps_.begin(); i != ramps_.end(); i++){
        i->receiver_preferences_.remove_receiver( dynamic_cast<IPackageReceiver*>(&*w) );
    }

    for (typename NodeCollection<Worker>::iterator i = workers_.begin(); i != workers_.end(); i++){
        i->receiver_preferences_.remove_receiver( dynamic_cast<IPackageReceiver*>(&*w) );
    }

    collection.remove_by_id(id);

/*    if(collection == ramps_) ramps_.remove_by_id(id);
    else if(collection == workers_) workers_.remove_by_id(id);
    else if(collection == storehouses_) storehouses_.remove_by_id(id);
    else throw "Wrong collection type!";*/
}

void Factory::remove_worker(ElementID id){
    remove_receiver(workers_, id);


/*    NodeCollection<Worker>::iterator w = find_worker_by_id(id);
    for (NodeCollection<Ramp>::iterator i = ramps_.begin(); i != ramps_.end(); i++){
        i->receiver_preferences_.remove_receiver( dynamic_cast<IPackageReceiver*>(&*w) );
    }
    workers_.remove_by_id(id);*/
}

void Factory::remove_storehouse(ElementID id) {
    remove_receiver(storehouses_, id);
/*    NodeCollection<Storehouse>::iterator s = find_storehouse_by_id(id);
    for (NodeCollection<Ramp>::iterator i = ramps_.begin(); i != ramps_.end(); i++){
        i->receiver_preferences_.remove_receiver( dynamic_cast<IPackageReceiver*>(&*s) );
    }
    storehouses_.remove_by_id(id);*/
}

enum class NodeColor { UNVISITED, VISITED, VERIFIED };
bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if(node_colors[sender] == NodeColor::VERIFIED){
        return true;
    }
    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().size() == 0){
        throw std::logic_error("Net is not consistent");
    }

    bool have_more_than_one_receiver = false;
    auto receivers = sender->receiver_preferences_.get_preferences();

    for(auto i = receivers.cbegin(); i != receivers.cend();i++){
        if (i->first->get_receiver_type() == ReceiverType::STOREHOUSE) {
            have_more_than_one_receiver = true;
        }
        else if (i->first->get_receiver_type() == ReceiverType::WORKER){
            IPackageReceiver* receiver_ptr = i->first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if(sendrecv_ptr == sender) break;

            have_more_than_one_receiver = true;

            if(node_colors[sendrecv_ptr] == NodeColor::UNVISITED){
                    has_reachable_storehouse(sendrecv_ptr, node_colors);
            }

        }
    }

    node_colors[sender] = NodeColor::VERIFIED;

    if(have_more_than_one_receiver){
        return true;
    }
    else{
        throw std::logic_error("Net is not consistent");
    }
}

bool Factory::is_consistent(void) {
    std::map<const PackageSender*, NodeColor> node_colors;

    for (auto i = ramps_.cbegin(); i != ramps_.cend(); i++) {
        node_colors[dynamic_cast<const PackageSender*>(&(*i))] = NodeColor::UNVISITED;
    }

    for (auto i = workers_.cbegin(); i != workers_.cend(); i++) {
        node_colors[dynamic_cast<const PackageSender*>(&(*i))] = NodeColor::UNVISITED;
    }

    try{ //obtoczyłem całą pętlę wyjątkiem ponieważ chyba tak jest w algorytmie
        for (auto i = ramps_.cbegin(); i != ramps_.cend(); i++) {
            has_reachable_storehouse(dynamic_cast<const PackageSender*>(&(*i)), node_colors);
        }
    }
    catch (std::logic_error&){
        return false;
    }

    return true;
}

/* Przykład pliku IO:
; == LOADING RAMPS ==

LOADING_RAMP id=1 delivery-interval=3
LOADING_RAMP id=2 delivery-interval=2

; == WORKERS ==

WORKER id=1 processing-time=2 queue-type=FIFO
WORKER id=2 processing-time=1 queue-type=LIFO

; == STOREHOUSES ==

STOREHOUSE id=1

; == LINKS ==

LINK src=ramp-1 dest=worker-1

LINK src=ramp-2 dest=worker-1
LINK src=ramp-2 dest=worker-2

LINK src=worker-1 dest=worker-1
LINK src=worker-1 dest=worker-2

LINK src=worker-2 dest=store-1
 */

//FIXME - poprawić auto na autentyczne typy

class FactoryIOerror : public std::logic_error
{
public:
    explicit FactoryIOerror(const std::string&  what_arg ) : std::logic_error(what_arg) {};
};

/**
 * Funkcja zamieniająca łąńcuch "FIFO" lub "LIFO" na odpowiadający typ PackageQueueType
 * @param inString
 * @return
 */
PackageQueueType hashPackageQueueType(std::string inString)  {
    if (inString == "FIFO") return PackageQueueType::FIFO;
    if (inString == "LIFO") return PackageQueueType::LIFO;
    throw "Wrong PackageQueueType!";
}

/**
 * Klasa zawierająca pojedynczą linię z wczytywanego pliku
 * element_type - typ co jest właśnie wczytywane
 * parameters - mapa string->string zawierająca wszystkie key=value
 */
struct ParsedLineData{
    /*struct ParsedLineData
            element_type : ElementType [RAMP|WORKER|STOREHOUSE|LINK]
    parameters : string -> string>  # mapowanie "ID -> wartość"*/

    enum class ElementType {RAMP, WORKER, STOREHOUSE, LINK};
    ElementType element_type;
    std::map<std::string, std::string> parameters;

    /**
     * Funkcja zamnieniająca inString na typ wyliczeniowy ElementType
     * @param inString - wartość którą należy zamienić na odpowiadającą wartość typu ElementType
     * @return ElementType odpowidający podanemu stringowi
     * @raises error jak nie istnieje taka wartość ElementType
     */
    static ParsedLineData::ElementType hashit(std::string const& inString) {
        if (inString == "LOADING_RAMP") return ElementType::RAMP;
        if (inString == "WORKER") return ElementType::WORKER;
        if (inString == "STOREHOUSE") return ElementType::STOREHOUSE;
        if (inString == "LINK") return ElementType::LINK;
        throw FactoryIOerror("Wrong ElementType!");
    }
};


/**
 * Funkcja rozkładająca linię linę na typ ParsedLineData. Jako pierwszy element "wyjmuje" typ typ elementu i zapisuje do
 * ParsedLineData::element_type i wszystkie wartość key=value zapisuje w mapie string -> string ParsedLineData::parameters
 * @param line
 * @return
 */
ParsedLineData parse_line(std::string line) {
/*  parse_line(line : string)
    rozłóż linię na ID określający typ elementu oraz tokeny "klucz=wartość"
    każdy token rozłóż na parę (klucz, wartość)
    na podstawie ID oraz par (klucz, wartość) zwróć odpowiedni obiekt typu ParsedLineData*/

    std::istringstream token_stream(line);

    std::vector<std::string> tokens;
    std::string token;

    //Dzielę otrzymaną linijkę na poszczególne słowa
    while (std::getline(token_stream, token, ' ')) {
        tokens.push_back(token);
    }


    if (tokens[0] != ";") {
        ParsedLineData data{};

        //Pierwszy element zamieniam na typ wyliczeniowy ElementType i ustawiam jako typ elementu daty
        data.element_type = ParsedLineData::hashit(tokens[0]);

        //dla wszystkiego co nie jest pierwszym słowem, dzielę słowo na dwa słowa przed i po "=" i mapuję [przed] = po
        for(auto i = tokens.cbegin() + 1; i!= tokens.cend(); ++i) {
            size_t found = i->find("=");

            if (found == std::string::npos) throw FactoryIOerror("no \"=\" in one of tokens!");

            data.parameters[i->substr(0, found)] = i->substr(found+1, size(*i) - found+1);


        }

        return data;
    }

    throw FactoryIOerror("parse_line called on non-parseable line!");
}






Factory load_factory_structure(std::istream& is) {
/*  load_factory_structure(is: std::istream&)
    utwórz (pusty) obiekt typu Factory

    dla każdej linii w pliku
    jeśli linia pusta lub rozpoczyna się od znaku komentarza - przejdź do kolejnej linii
        dokonaj parsowania linii
    w zależności od typu elementu - wykorzystaj pary (klucz, wartość) do poprawnego:
        * zainicjalizowania obiektu właściwego typu węzła i dodania go do obiektu fabryki, albo
        * utworzenia połączenia między zadanymi węzłami sieci

    zwróć wypełniony obiekt fabryki*/

    Factory f{};

    std::string line;

    bool links_creating = false;

    while (std::getline(is, line)) {
        if((line != "") && (line[0] != ';')){
            ParsedLineData l = parse_line(line);

            if(size(l.parameters) < 1 ) throw FactoryIOerror("Error in data type, not enough arguments");


            //nieudolna próba przeniesienia przypisywania id poza switcha by uniknąć powtórzeń
            ElementID id = 0;
            if((l.parameters.find("id") == l.parameters.cend()) && (l.element_type != ParsedLineData::ElementType::LINK)) throw FactoryIOerror("no element id!");
            else if(l.parameters.find("id") != l.parameters.cend()) id = std::stoi(l.parameters["id"]);

            switch (l.element_type) {
                case ParsedLineData::ElementType::RAMP:{
                    //LOADING_RAMP id=1 delivery-interval=3
                    if(size(l.parameters) !=2 ) throw FactoryIOerror("Error in data type, wrong number of LOADING_RAMP arguments");

                    if(links_creating) throw FactoryIOerror("Liks defined before nodes!");

                    if(l.parameters.find("delivery-interval") == l.parameters.cend()) throw FactoryIOerror("no ramp dt!");
                    TimeOffset dt = std::stoi(l.parameters["delivery-interval"]);

                    f.add_ramp(Ramp(id, dt));
                    break;
                }
                case ParsedLineData::ElementType::WORKER:{
                    //WORKER id=1 processing-time=2 queue-type=FIFO
                    if(size(l.parameters) !=3 ) throw FactoryIOerror("Error in data type, wrong number of WORKER arguments");

                    if(links_creating) throw FactoryIOerror("Liks defined before nodes!");

                    if(l.parameters.find("processing-time") == l.parameters.cend()) throw FactoryIOerror("no worker processing time!");
                    Time pt = std::stoi(l.parameters["processing-time"]);

                    if(l.parameters.find("queue-type") == l.parameters.cend()) throw FactoryIOerror("no worker queue type!");
                    PackageQueueType qt = static_cast<PackageQueueType>(hashPackageQueueType(l.parameters["queue-type"]));

                    f.add_worker(Worker(id, pt, std::make_unique<PackageQueue>(PackageQueue(qt))));
                    break;
                }
                case ParsedLineData::ElementType::STOREHOUSE:{
                    //STOREHOUSE id=1
                    if(size(l.parameters) !=1 ) throw FactoryIOerror("Error in data type, wrong number of Storehouse arguments");

                    if(links_creating) throw FactoryIOerror("Liks defined before nodes!");

                    f.add_storehouse(Storehouse(id));
                    break;
                }
                case ParsedLineData::ElementType::LINK:{
                    //LINK src=ramp-1 dest=worker-1

                    if(size(l.parameters) !=2 ) throw FactoryIOerror("Error in data type, wrong number of link arguments");

                    links_creating = true;

                    //SOURCE

                    if(l.parameters.find("src") == l.parameters.cend()) throw FactoryIOerror("no link src!");
                    std::string ps = l.parameters["src"];

                    size_t found = ps.find("-");

                    if(found == std::string::npos) throw FactoryIOerror("Wrong source qualifier!");

                    auto src_type = ps.substr(0, found);

                    //To ma sens w ogóle? - Nie
                    //std::transform(src_type.begin(), src_type.end(), src_type.begin(), [](unsigned char c) -> unsigned char { return std::toupper(c); } );

                    auto src_id = std::stoi(ps.substr(found+1, size(ps) - found+1));

                    //DESTINATION

                    if(l.parameters.find("dest") == l.parameters.cend()) throw FactoryIOerror("no link dest!");
                    ps = l.parameters["dest"];

                    found = ps.find("-");

                    if(found == std::string::npos) throw FactoryIOerror("Wrong dest qualifier!");

                    auto des_type = ps.substr(0, found);
                    auto des_id = std::stoi(ps.substr(found+1, size(ps) - found+1));

                    // ADDING CONNECTION
                    if(src_type == "ramp"){
                        if(des_type == "worker"){
                            f.find_ramp_by_id(src_id)->receiver_preferences_.add_receiver(dynamic_cast<IPackageReceiver *>(&*f.find_worker_by_id(des_id)));
                        }
                        else if(des_type == "store"){
                            f.find_ramp_by_id(src_id)->receiver_preferences_.add_receiver(dynamic_cast<IPackageReceiver *>(&*f.find_storehouse_by_id(des_id)));
                        }
                    }
                    else if(src_type == "worker"){
                        if(des_type == "worker"){
                            f.find_worker_by_id(src_id)->receiver_preferences_.add_receiver(dynamic_cast<IPackageReceiver *>(&*f.find_worker_by_id(des_id)));
                        }
                        else if(des_type == "store"){
                            f.find_worker_by_id(src_id)->receiver_preferences_.add_receiver(dynamic_cast<IPackageReceiver *>(&*f.find_storehouse_by_id(des_id)));
                        }
                    }
                    break;
                }
                default:
                    throw FactoryIOerror("Wrong ElementType! (hashit passed, something went terribly wrong)");
            }
        }
    }

    return f;

}

/*void save_factory_structure(Factory& factory, std::ostream& os) {
 // przelecieć pętlą po wszystkicha nodeach fabryki:
 // pracownikach: factory.worker_cbegin() - factory.worker_cend()
 // magazynach: factory.storehouse_cbegin() - factory.storehouse_cend()
 // wyciągnąć z każdej rampy id i di (i->get_id() i i->get_delivery_interval(), gdzie i to iterator)
 // tak samo id, processing_time i queue_type z workera. queue_type trzeba na stringa zamienić, ODWROTNIE niż robi to funkcja hashPackageQueueType z pliku storage types
 // ze strehouses tylko id
 //
 // później idą połączenia
 // trzeba je wyjąć z workerów i ramp, tak samo jak powyżej w pętli. i to będzie src. czyli bierzesz id i jeżeli to jest rampa to piszesz src=ramp-<id> jeżeli worker to src=worker-<id>
 // ale dla każdego workera i rampy trzeba przeiterować po i->receiver_preferences_.get_preferences() żeby jeszcze uzyskać ich dest (czyli pętla w pętli)
 // j -> first to obiekt do kórego ma wysłać pakiet, nasze dest (gdzie j to iterator pętli wewnętrznej, tej iterującej po i->receiver_preferences_.get_preferences())
 // bierz typtakiego poprzez .get_receiver_type() i id poprzez.get_id() reciver_typę jest typu enum ReceiverType więc też ifem trzeba to zamienić na stringa
 // zapisujesz do os << "tekst" tak żeby wyglądało jak w przykładzie pliku we/wy na górze albo na stronie Kłeczka

}*/

void save_factory_structure(Factory& factory, std::ostream& os) {

    os << "; == LOADING RAMPS =="<< endl << endl;

    for (auto r = factory.ramp_cbegin(); r != factory.ramp_cend(); r++) {

        os << "LOADING_RAMP id=" << r->get_id() << " delivery-interval=" << r->get_delivery_interval()<< endl;;
    }

    os << "; == WORKERS =="<< endl << endl;

    for (auto w = factory.worker_cbegin(); w != factory.worker_cend(); w++) {
        if (w->get_queue()->get_queue_type() == PackageQueueType::FIFO) {
            os << "WORKER id=" << w->get_id() << " processing-time=" << w->get_processing_duration() << " queue-type=FIFO"<< endl;;
        }
        if (w->get_queue()->get_queue_type() == PackageQueueType::LIFO) {
            os << "WORKER id=" << w->get_id() << " processing-time=" << w->get_processing_duration() << " queue-type=LIFO"<< endl;;
        }
    }

    os << "; == STOREHOUSES =="<< endl << endl;

    for (auto s = factory.storehouse_cbegin(); s != factory.storehouse_cend(); s++) {

        os << "STOREHOUSE id=" << s->get_id() << endl; ;
    }

    os << "; == LINKS =="<< endl << endl;;

    for (auto l_1 =  factory.ramp_cbegin(); l_1 != factory.ramp_cend(); l_1++) {
        for (auto d_1 = l_1->receiver_preferences_.get_preferences().cbegin(); d_1 != l_1->receiver_preferences_.get_preferences().cend(); d_1++) {
            std::string des = "";
            if (d_1->first->get_receiver_type() == ReceiverType::WORKER){
                des = "worker";
            }
            if (d_1->first->get_receiver_type() == ReceiverType::STOREHOUSE){
                des = "store";
            }
            os << "LINK src=ramp-" << l_1->get_id() << " dest=" << des << "-" << d_1->first->get_id() << endl;
        }

        os << endl;
    }

    for (auto l_2 = factory.worker_cbegin(); l_2 != factory.worker_cend(); l_2++) {
        for (auto d_2 = l_2->receiver_preferences_.get_preferences().cbegin(); d_2 != l_2->receiver_preferences_.get_preferences().cend(); d_2++) {
            std::string des = "";
            if (d_2->first->get_receiver_type() == ReceiverType::WORKER){
                des = "worker";
            }
            if (d_2->first->get_receiver_type() == ReceiverType::STOREHOUSE){
                des = "store";
            }
            os << "LINK src=worker-" << l_2->get_id() << " dest=" << des << "-" << d_2->first->get_id()<< endl;
        }

        os << endl;
    }

}