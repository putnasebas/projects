#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>

//identification = region + ID || city + address
// region && city - A != a
//owners - A == a
struct CLand {
    CLand() = default;
    CLand(const std::string &region, size_t id, const std::string &city, const std::string &address,const std::string &owner)
        : m_region(region),m_ID(id),m_city(city),m_address(address), m_owner(owner) {
    }
    void printByRegionAndId(std::ostream  &os) const;
    void printByCityAndAddress(std::ostream &os) const;
    bool operator == (const CLand &cmp) const { return  m_owner == cmp.m_owner; }
    friend std::ostream &operator << (std::ostream &os, const CLand &data);

    std::string m_region;
    size_t  m_ID;
    std::string m_city;
    std::string m_address;
    std::string m_owner;
    int m_count;
};

bool compareCityAddress(const CLand &cmp1, const CLand &cmp2);
bool compareRegions(const CLand &cmp1, const CLand &cmp2);

class CIterator {
    public:
        CIterator(const std::vector<CLand> &data)
            : data(data) {}
        bool atEnd() const;
        void next() {position++;}
        std::string city() const { return data[position].m_city;}
        std::string addr() const { return data[position].m_address;}
        std::string region() const { return data[position].m_region;}
        unsigned id() const { return data[position].m_ID;}
        std::string owner() const {return data[position].m_owner;}

    private:
        const std::vector<CLand>& data;
        size_t position = 0;
};


class CLandRegister {
    public:
        CLandRegister() = default;
        /**
         * @params new land
         * @returns true if adding was successful, otherwise false(for example duplicate with city && address || region and id)
         * */
        bool add(const std::string &city,
                 const std::string &addr,
                 const std::string &region,
                 unsigned int id);
        /**
         * @params city and address
         * @returns true if land is present otherwise false
         * */
        bool del(const std::string &city,
                 const std::string &addr);
        /**
         * @params region + ID
         * @returns true if land is present otherwise false
         * */
        bool del(const std::string &region,
                 unsigned int id);
        /**
         * @params city + address <=> search in first vector
         * @returns true if found + change owner variable, otherwise false and owner remains same
         * */
        bool getOwner(const std::string &city,
                      const std::string &addr,
                      std::string &owner) const;
        /**
        * @params city + address <=> search in second vector
        * @returns true if found + change owner variable, otherwise false and owner remains same
        * */
        bool getOwner(const std::string &region,
                      unsigned int id,
                      std::string &owner) const;
        /**
         * @params city + address <=> search in first vector
         * @returns true if found + land receives new owner, false if land does not exist or is already occupied
         * */
        bool newOwner(const std::string &city,
                      const std::string &addr,
                      const std::string &owner);
        /**
         * @params city + address <=> search in second vector
         * @returns true if found + land receives new owner, false if land does not exist or is already occupied
         * */
        bool newOwner(const std::string &region,
                      unsigned int id,
                      const std::string &owner);
        /**
         * @returns amount of parcels owned by owner, 0 if none
         * */
        size_t count(const std::string &owner) const;
        /**
         * Sorted primary by city and secondary by address
         * */
        CIterator listByAddr() const;
        /**
         * Sorted by the time of time received, if none returns end() immediately
         * */
        CIterator listByOwner(const std::string &owner) const;

        friend std::ostream &operator << (std::ostream &os, const CLandRegister &data);

    private:
        bool findOwner(const std::string &owner,size_t &index) const;
        void insertNewPerson(CLand &newOwner);

        std::vector<std::vector<CLand>> m_dataByOwner;
        std::vector<CLand> m_dummy;
        std::vector<CLand> m_dataByCityAndAddress;
        std::vector<CLand> m_dataRegionAndID;
};

bool CIterator::atEnd() const {
    if(position == data.size() || data.empty())
        return true;
    return false;
}

bool compareCityAddress(const CLand &cmp1, const CLand &cmp2) {
    if(cmp1.m_city != cmp2.m_city)
        return cmp1.m_city < cmp2.m_city;
    return  cmp1.m_address < cmp2.m_address;
}

bool compareRegions(const CLand &cmp1, const CLand &cmp2) {
    if(cmp1.m_region != cmp2.m_region)
        return cmp1.m_region < cmp2.m_region;
    return cmp1.m_ID < cmp2.m_ID;
}

bool CLandRegister::add(const std::string &city, const std::string &addr, const std::string &region, unsigned int id) {
    CLand input(region,id,city,addr,"");
    //first emplace
    if(m_dataByCityAndAddress.empty()) {
        m_dataByCityAndAddress.emplace_back(input);
        m_dataRegionAndID.emplace_back(input);
        m_dataByOwner.emplace_back();
        m_dataByOwner[0].emplace_back(input);
        return true;
    }
    //Check for duplicate + insert
    auto inRegion = std::lower_bound(m_dataRegionAndID.begin(),m_dataRegionAndID.end(),input ,compareRegions);
    auto inCity = std::lower_bound(m_dataByCityAndAddress.begin(),m_dataByCityAndAddress.end(),input,
                                   compareCityAddress);
    auto index = inRegion - m_dataRegionAndID.begin();
    auto index1 = inCity - m_dataByCityAndAddress.begin();
    //duplicate found
    if( inRegion != m_dataRegionAndID.end() &&
        (m_dataRegionAndID[index].m_region == input.m_region && m_dataRegionAndID[index].m_ID == input.m_ID))
        return false;
    if( inCity != m_dataByCityAndAddress.end() &&
        (m_dataByCityAndAddress[index1].m_city == input.m_city && m_dataByCityAndAddress[index1].m_address == input.m_address))
        return false;
    //Input is valid
    if(inRegion == m_dataRegionAndID.end()) m_dataRegionAndID.emplace_back(input);
    else m_dataRegionAndID.insert(m_dataRegionAndID.begin()+index,input);
    if(inCity == m_dataByCityAndAddress.end()) m_dataByCityAndAddress.emplace_back(input);
    else m_dataByCityAndAddress.insert(m_dataByCityAndAddress.begin()+index1,input);
    m_dataByOwner[0].emplace_back(input);
    return true;
}

bool CLandRegister::del(const std::string &city, const std::string &addr) {
    CLand t("",0,city, addr,"");
    auto search = std::lower_bound(m_dataByCityAndAddress.begin(),m_dataByCityAndAddress.end(),t, compareCityAddress);
    if(search == m_dataByCityAndAddress.end())
        return false;
    auto index = search - m_dataByCityAndAddress.begin();
    //value found
    if(m_dataByCityAndAddress[index].m_city == t.m_city && m_dataByCityAndAddress[index].m_address == t.m_address) {
        t.m_ID = m_dataByCityAndAddress[index].m_ID;
        t.m_region = m_dataByCityAndAddress[index].m_region;
        t.m_owner = m_dataByCityAndAddress[index].m_owner;
        m_dataByCityAndAddress.erase(m_dataByCityAndAddress.begin()+index);
        //delete from second vector
        auto search1 = std::lower_bound(m_dataRegionAndID.begin(),m_dataRegionAndID.end(),t, compareRegions) - m_dataRegionAndID.begin();
        m_dataRegionAndID.erase(m_dataRegionAndID.begin()+search1);
        //delete from third vector
        auto findPlace = std::lower_bound(m_dataByOwner.begin(),m_dataByOwner.end(),t,
                                          [](auto &cmp1,const CLand &cmp2){ return strcasecmp(cmp1[0].m_owner.c_str(),cmp2.m_owner.c_str()) < 0; }) - m_dataByOwner.begin();
        size_t found = 0;
        for(size_t i = 0; i < m_dataByOwner[findPlace].size();++i) {
            if(strcasecmp(m_dataByOwner[findPlace][i].m_owner.c_str(),t.m_owner.c_str()) == 0
               && ((m_dataByOwner[findPlace][i].m_city == t.m_city && m_dataByOwner[findPlace][i].m_address == t.m_address)
                   || (m_dataByOwner[findPlace][i].m_region == t.m_region && m_dataByOwner[findPlace][i].m_ID == t.m_ID)))
                found = i;
        } m_dataByOwner[findPlace].erase(m_dataByOwner[findPlace].begin()+found);
        return true;
    } return false;
}

bool CLandRegister::del(const std::string &region, unsigned int id) {
    CLand t(region,id,"","","");
    auto search = std::lower_bound(m_dataRegionAndID.begin(),m_dataRegionAndID.end(),t, compareRegions);
    if(search == m_dataRegionAndID.end())
        return false;
    size_t index = search - m_dataRegionAndID.begin();
    //value found
    if(m_dataRegionAndID[index].m_region == t.m_region && m_dataRegionAndID[index].m_ID == t.m_ID) {
       t.m_owner = m_dataRegionAndID[index].m_owner;
       t.m_address = m_dataRegionAndID[index].m_address;
       t.m_city = m_dataRegionAndID[index].m_city;
       m_dataRegionAndID.erase(m_dataRegionAndID.begin()+index);
       //delete from second vector
       auto search1 = std::lower_bound(m_dataByCityAndAddress.begin(),m_dataByCityAndAddress.end(),t,
                                       compareCityAddress) - m_dataByCityAndAddress.begin();
       m_dataByCityAndAddress.erase(m_dataByCityAndAddress.begin()+search1);
       //delete from third vector
        auto findPlace = std::lower_bound(m_dataByOwner.begin(),m_dataByOwner.end(),t,
                                          [](auto &cmp1,const CLand &cmp2){ return strcasecmp(cmp1[0].m_owner.c_str(),cmp2.m_owner.c_str()) < 0; }) - m_dataByOwner.begin();
        size_t found = 0;
        for(size_t i = 0; i < m_dataByOwner[findPlace].size();++i) {
            if(strcasecmp(m_dataByOwner[findPlace][i].m_owner.c_str(),t.m_owner.c_str()) == 0
               && ((m_dataByOwner[findPlace][i].m_city == t.m_city && m_dataByOwner[findPlace][i].m_address == t.m_address)
                   || (m_dataByOwner[findPlace][i].m_region == t.m_region && m_dataByOwner[findPlace][i].m_ID == t.m_ID)))
                found = i;
        } m_dataByOwner[findPlace].erase(m_dataByOwner[findPlace].begin()+found);
        return true;
    } return false;
}

CIterator CLandRegister::listByAddr() const {
    return {(m_dataByCityAndAddress)};
}

bool CLandRegister::getOwner(const std::string &city, const std::string &addr, std::string &owner) const {
    CLand t("",0,city,addr,"");
    auto search = std::lower_bound(m_dataByCityAndAddress.begin(),m_dataByCityAndAddress.end(),t, compareCityAddress);
    if(search == m_dataByCityAndAddress.end())
        return false;
    size_t index = search - m_dataByCityAndAddress.begin();
    if(m_dataByCityAndAddress[index].m_city == t.m_city && m_dataByCityAndAddress[index].m_address == t.m_address) {
        owner = m_dataByCityAndAddress[index].m_owner;
        return true;
    } return false;
}

bool CLandRegister::getOwner(const std::string &region, unsigned int id, std::string &owner) const {
    CLand t(region,id,"","","");
    auto search = std::lower_bound(m_dataRegionAndID.begin(),m_dataRegionAndID.end(),t, compareRegions);
    if(search == m_dataRegionAndID.end())
        return false;
    size_t index = search - m_dataRegionAndID.begin();
    if(m_dataRegionAndID[index].m_region == t.m_region && m_dataRegionAndID[index].m_ID == t.m_ID) {
        owner = m_dataRegionAndID[index].m_owner;
        return true;
    } return false;
}

bool CLandRegister::findOwner(const std::string &owner,size_t &index) const {
    CLand person("",0,"","",owner);
    auto search = std::lower_bound(m_dataByOwner.begin(),m_dataByOwner.end(),person,
                                  [](auto &cmp1,const CLand &cmp2){ return strcasecmp(cmp1[0].m_owner.c_str(),cmp2.m_owner.c_str()) < 0; });
    if(search == m_dataByOwner.end())
        return false;
    index = search - m_dataByOwner.begin();
    if(strcasecmp(m_dataByOwner[index][0].m_owner.c_str(),owner.c_str()) == 0)
        return true;
    return false;
}

void CLandRegister::insertNewPerson(CLand &newOwner) {
    auto item = std::lower_bound(m_dataByOwner.begin(),m_dataByOwner.end(),newOwner,
                                 [](const auto &cmp1, const auto &cmp2)
                                 { return strcasecmp( cmp1[0].m_owner.c_str(), cmp2.m_owner.c_str()) < 0;});
    if(item == m_dataByOwner.end()) {
        m_dataByOwner.emplace_back();
        m_dataByOwner[m_dataByOwner.size()-1].emplace_back(newOwner);
        return;
    } auto index = item - m_dataByOwner.begin();
    m_dataByOwner.insert(m_dataByOwner.begin()+index,{newOwner});
}

bool CLandRegister::newOwner(const std::string &city, const std::string &addr, const std::string &owner) {
    CLand t("",0, city,addr,owner);
    auto search = std::lower_bound(m_dataByCityAndAddress.begin(),m_dataByCityAndAddress.end(),t, compareCityAddress);
    if(search == m_dataByCityAndAddress.end())
        return false;
    size_t index = search - m_dataByCityAndAddress.begin();
    //Land successfully found
    if(m_dataByCityAndAddress[index].m_city == city && m_dataByCityAndAddress[index].m_address == addr) {
        if(strcasecmp( m_dataByCityAndAddress[index].m_owner.c_str(),owner.c_str()) == 0)
            return false;
        std::string oldOwner = m_dataByCityAndAddress[index].m_owner;
        m_dataByCityAndAddress[index].m_owner = owner;
        t.m_ID = m_dataByCityAndAddress[index].m_ID;
        t.m_region = m_dataByCityAndAddress[index].m_region;
        size_t search1 = std::lower_bound(m_dataRegionAndID.begin(),m_dataRegionAndID.end(),t, compareRegions)
                - m_dataRegionAndID.begin();
        m_dataRegionAndID[search1].m_owner = owner;
        //replace owner
        t.m_owner = oldOwner;
        auto findPlace = std::lower_bound(m_dataByOwner.begin(),m_dataByOwner.end(),t,
                                       [](auto &cmp1,const CLand &cmp2){ return strcasecmp(cmp1[0].m_owner.c_str(),cmp2.m_owner.c_str()) < 0; }) - m_dataByOwner.begin();
        size_t found = 0;
        for(size_t i = 0; i < m_dataByOwner[findPlace].size();++i) {
            if(strcasecmp(m_dataByOwner[findPlace][i].m_owner.c_str(),t.m_owner.c_str()) == 0
            && ((m_dataByOwner[findPlace][i].m_city == t.m_city && m_dataByOwner[findPlace][i].m_address == t.m_address)
                || (m_dataByOwner[findPlace][i].m_region == t.m_region && m_dataByOwner[findPlace][i].m_ID == t.m_ID)))
                found = i;
        } m_dataByOwner[findPlace].erase(m_dataByOwner[findPlace].begin()+found);
        size_t insertTo = 0;
        t.m_owner = owner;
        if(findOwner(owner,insertTo)) {
            m_dataByOwner[insertTo].emplace_back(t);
        } else this->insertNewPerson(t);
        return true;
    } return false;
}

bool CLandRegister::newOwner(const std::string &region, unsigned int id, const std::string &owner) {
    CLand t(region,id,"","",owner);
    auto search = std::lower_bound(m_dataRegionAndID.begin(),m_dataRegionAndID.end(),t, compareRegions);
    if(search == m_dataRegionAndID.end())
        return false;
    size_t index = search - m_dataRegionAndID.begin();
    //Land successfully found
    if(m_dataRegionAndID[index].m_region == region && m_dataRegionAndID[index].m_ID == id) {
        if(strcasecmp( m_dataRegionAndID[index].m_owner.c_str(),owner.c_str()) == 0)
            return false;
        std::string oldOwner = m_dataRegionAndID[index].m_owner;
        m_dataRegionAndID[index].m_owner = owner;
        t.m_city = m_dataRegionAndID[index].m_city;
        t.m_address = m_dataRegionAndID[index].m_address;
        size_t search1 = std::lower_bound(m_dataByCityAndAddress.begin(),m_dataByCityAndAddress.end(),t,
                                          compareCityAddress) - m_dataByCityAndAddress.begin();
        m_dataByCityAndAddress[search1].m_owner = owner;
        //replace owner
        t.m_owner = oldOwner;
        auto findPlace = std::lower_bound(m_dataByOwner.begin(),m_dataByOwner.end(),t,
                                          [](const auto &cmp1,const auto &cmp2)
                                          { return strcasecmp(cmp1[0].m_owner.c_str(),cmp2.m_owner.c_str()) < 0; }) - m_dataByOwner.begin();
        size_t found = 0;
        for(size_t i = 0; i < m_dataByOwner[findPlace].size();++i) {
            if(strcasecmp(m_dataByOwner[findPlace][i].m_owner.c_str(),t.m_owner.c_str()) == 0
               && ((m_dataByOwner[findPlace][i].m_city == t.m_city && m_dataByOwner[findPlace][i].m_address == t.m_address)
                   || (m_dataByOwner[findPlace][i].m_region == t.m_region && m_dataByOwner[findPlace][i].m_ID == t.m_ID)))
                found = i;
        } m_dataByOwner[findPlace].erase(m_dataByOwner[findPlace].begin()+found);
        size_t insertTo = 0;
        t.m_owner = owner;
        if(findOwner(owner,insertTo)) {
            m_dataByOwner[insertTo].emplace_back(t);
        } else this->insertNewPerson(t);
        return true;
    } return false;
}

size_t CLandRegister::count(const std::string &owner) const {
    size_t index = 0;
    if(findOwner(owner,index))
        return m_dataByOwner[index].size();
    return 0;
}

CIterator CLandRegister::listByOwner(const std::string &owner) const {
    size_t index = 0;
    if(findOwner(owner,index)) {
        return {{m_dataByOwner[index]}};
    } return {m_dummy};
}

void CLand::printByRegionAndId(std::ostream  &os) const {
    os << "{" << m_region << "," << m_ID << "|"
       << m_owner << "," << m_city << "," << m_address<< "}";
}

void CLand::printByCityAndAddress(std::ostream &os) const {
    os << "{" << m_city << "," << m_address << "|"
       << m_owner << "," << m_ID << "," << m_region<< "}";
}

std::ostream &operator<<(std::ostream &os, const CLand &data) {
    os << "{" << data.m_owner << "," << data.m_city << "," << data.m_address << "," << data.m_ID << "," << data.m_region << "}";
    return os;
}

std::ostream &operator << (std::ostream &os, const CLandRegister &data) {
    os << "Sorted by City/Address\n";
    for(const auto &it: data.m_dataByCityAndAddress) {
        it.printByCityAndAddress(os);
        os << std::endl;
    } os << std::endl;
    os << "Sorted by ID/Region\n";
    for(const auto &it: data.m_dataRegionAndID) {
        it.printByRegionAndId(os);
        os << std::endl;
    } return os;
}

static void test0() {
    CLandRegister x;
    std::string owner;

    assert (x.add("Prague", "Thakurova", "Dejvice", 12345));
    assert (x.add("Prague", "Evropska", "Vokovice", 12345));
    assert (x.add("Prague", "Technicka", "Dejvice", 9873));
    assert (x.add("Plzen", "Evropska", "Plzen mesto", 78901));
    assert (x.add("Liberec", "Evropska", "Librec", 4552));
    CIterator i0 = x.listByAddr();
    assert (!i0.atEnd()
            && i0.city() == "Liberec"
            && i0.addr() == "Evropska"
            && i0.region() == "Librec"
            && i0.id() == 4552
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Plzen"
            && i0.addr() == "Evropska"
            && i0.region() == "Plzen mesto"
            && i0.id() == 78901
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Evropska"
            && i0.region() == "Vokovice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Technicka"
            && i0.region() == "Dejvice"
            && i0.id() == 9873
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Thakurova"
            && i0.region() == "Dejvice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (i0.atEnd());
    assert (x.count("") == 5);
    CIterator i1 = x.listByOwner("");
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Thakurova"
            && i1.region() == "Dejvice"
            && i1.id() == 12345
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Evropska"
            && i1.region() == "Vokovice"
            && i1.id() == 12345
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Technicka"
            && i1.region() == "Dejvice"
            && i1.id() == 9873
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Plzen"
            && i1.addr() == "Evropska"
            && i1.region() == "Plzen mesto"
            && i1.id() == 78901
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Liberec"
            && i1.addr() == "Evropska"
            && i1.region() == "Librec"
            && i1.id() == 4552
            && i1.owner() == "");
    i1.next();
    assert (i1.atEnd());

    assert (x.count("CVUT") == 0);
    CIterator i2 = x.listByOwner("CVUT");
    assert (i2.atEnd());

    assert (x.newOwner("Prague", "Thakurova", "CVUT"));
    assert (x.newOwner("Dejvice", 9873, "CVUT"));
    assert (x.newOwner("Plzen", "Evropska", "Anton Hrabis"));
    assert (x.newOwner("Librec", 4552, "Cvut"));
    assert (x.getOwner("Prague", "Thakurova", owner) && owner == "CVUT");
    assert (x.getOwner("Dejvice", 12345, owner) && owner == "CVUT");
    assert (x.getOwner("Prague", "Evropska", owner) && owner == "");
    assert (x.getOwner("Vokovice", 12345, owner) && owner == "");
    assert (x.getOwner("Prague", "Technicka", owner) && owner == "CVUT");
    assert (x.getOwner("Dejvice", 9873, owner) && owner == "CVUT");
    assert (x.getOwner("Plzen", "Evropska", owner) && owner == "Anton Hrabis");
    assert (x.getOwner("Plzen mesto", 78901, owner) && owner == "Anton Hrabis");
    assert (x.getOwner("Liberec", "Evropska", owner) && owner == "Cvut");
    assert (x.getOwner("Librec", 4552, owner) && owner == "Cvut");
    CIterator i3 = x.listByAddr();
    assert (!i3.atEnd()
            && i3.city() == "Liberec"
            && i3.addr() == "Evropska"
            && i3.region() == "Librec"
            && i3.id() == 4552
            && i3.owner() == "Cvut");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Plzen"
            && i3.addr() == "Evropska"
            && i3.region() == "Plzen mesto"
            && i3.id() == 78901
            && i3.owner() == "Anton Hrabis");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Evropska"
            && i3.region() == "Vokovice"
            && i3.id() == 12345
            && i3.owner() == "");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Technicka"
            && i3.region() == "Dejvice"
            && i3.id() == 9873
            && i3.owner() == "CVUT");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Thakurova"
            && i3.region() == "Dejvice"
            && i3.id() == 12345
            && i3.owner() == "CVUT");
    i3.next();
    assert (i3.atEnd());
    assert (x.count("cvut") == 3);
    CIterator i4 = x.listByOwner("cVuT");
    assert (!i4.atEnd()
            && i4.city() == "Prague"
            && i4.addr() == "Thakurova"
            && i4.region() == "Dejvice"
            && i4.id() == 12345
            && i4.owner() == "CVUT");
    i4.next();
    assert (!i4.atEnd()
            && i4.city() == "Prague"
            && i4.addr() == "Technicka"
            && i4.region() == "Dejvice"
            && i4.id() == 9873
            && i4.owner() == "CVUT");
    i4.next();
    assert (!i4.atEnd()
            && i4.city() == "Liberec"
            && i4.addr() == "Evropska"
            && i4.region() == "Librec"
            && i4.id() == 4552
            && i4.owner() == "Cvut");
    i4.next();
    assert (i4.atEnd());
    assert (x.newOwner("Plzen mesto", 78901, "CVut"));
    assert (x.count("CVUT") == 4);
    CIterator i5 = x.listByOwner("CVUT");
    assert (!i5.atEnd()
            && i5.city() == "Prague"
            && i5.addr() == "Thakurova"
            && i5.region() == "Dejvice"
            && i5.id() == 12345
            && i5.owner() == "CVUT");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Prague"
            && i5.addr() == "Technicka"
            && i5.region() == "Dejvice"
            && i5.id() == 9873
            && i5.owner() == "CVUT");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Liberec"
            && i5.addr() == "Evropska"
            && i5.region() == "Librec"
            && i5.id() == 4552
            && i5.owner() == "Cvut");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Plzen"
            && i5.addr() == "Evropska"
            && i5.region() == "Plzen mesto"
            && i5.id() == 78901
            && i5.owner() == "CVut");
    i5.next();
    assert (i5.atEnd());



    assert (x.del("Liberec", "Evropska"));
    assert (x.del("Plzen mesto", 78901));
    assert (x.count("cvut") == 2);
    CIterator i6 = x.listByOwner("cVuT");
    assert (!i6.atEnd()
            && i6.city() == "Prague"
            && i6.addr() == "Thakurova"
            && i6.region() == "Dejvice"
            && i6.id() == 12345
            && i6.owner() == "CVUT");
    i6.next();
    assert (!i6.atEnd()
            && i6.city() == "Prague"
            && i6.addr() == "Technicka"
            && i6.region() == "Dejvice"
            && i6.id() == 9873
            && i6.owner() == "CVUT");
    i6.next();
    assert (i6.atEnd());

    assert (x.add("Liberec", "Evropska", "Librec", 4552));
}

static void test1() {
    CLandRegister x;
    std::string owner;

    assert (x.add("Prague", "Thakurova", "Dejvice", 12345));
    assert (x.add("Prague", "Evropska", "Vokovice", 12345));
    assert (x.add("Prague", "Technicka", "Dejvice", 9873));
    assert (!x.add("Prague", "Technicka", "Hradcany", 7344));
    assert (!x.add("Brno", "Bozetechova", "Dejvice", 9873));
    assert (!x.getOwner("Prague", "THAKUROVA", owner));
    assert (!x.getOwner("Hradcany", 7343, owner));
    CIterator i0 = x.listByAddr();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Evropska"
            && i0.region() == "Vokovice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Technicka"
            && i0.region() == "Dejvice"
            && i0.id() == 9873
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Thakurova"
            && i0.region() == "Dejvice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (i0.atEnd());

    assert (x.newOwner("Prague", "Thakurova", "CVUT"));
    assert (!x.newOwner("Prague", "technicka", "CVUT"));
    assert (!x.newOwner("prague", "Technicka", "CVUT"));
    assert (!x.newOwner("dejvice", 9873, "CVUT"));
    assert (!x.newOwner("Dejvice", 9973, "CVUT"));
    assert (!x.newOwner("Dejvice", 12345, "CVUT"));
    assert (x.count("CVUT") == 1);
    CIterator i1 = x.listByOwner("CVUT");
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Thakurova"
            && i1.region() == "Dejvice"
            && i1.id() == 12345
            && i1.owner() == "CVUT");
    i1.next();
    assert (i1.atEnd());

    assert (!x.del("Brno", "Technicka"));
    assert (!x.del("Karlin", 9873));
    assert (x.del("Prague", "Technicka"));
    assert (!x.del("Prague", "Technicka"));
    assert (!x.del("Dejvice", 9873));
}

int main(void) {
    test0();
    test1();
    return EXIT_SUCCESS;
}

