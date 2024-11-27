#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <compare>

class CDate {
    public:
        CDate(int y,int m,int d)
        : m_Y(y),m_M(m),m_D(d) {}
        std::strong_ordering operator<=>(const CDate &other) const = default;
        friend std::ostream &operator<<(std::ostream &os, const CDate &d) {
            return os << d.m_Y << '-' << d.m_M << '-' << d.m_D;
        }

    private:
        int m_Y;
        int m_M;
        int m_D;
};

enum class ESortKey {
    NAME,
    BIRTH_DATE,
    ENROLL_YEAR
};

#endif /* __PROGTEST__ */

std::string editString(const std::string &fullName);

std::vector<std::string> stringToVector(const std::string &name);

class CStudent {
    public:
        CStudent(const std::string &name, const CDate &born, int enrolled)
            : m_name(name), m_born(born), m_enrolled(enrolled) {
            m_editName = editString(name);
        }
        const std::string &name() const { return m_name; }
        const std::string &editName() const {return  m_editName; }
        const CDate &born() const { return m_born; }
        int enrolled() const { return m_enrolled; }

        bool operator < (const CStudent &other) const;
        bool operator == (const CStudent &other) const;

        bool operator != (const CStudent &other) const;

        friend std::ostream &operator << (std::ostream &os, const CStudent &data);

    private:
        std::string m_name;
        CDate m_born;
        int m_enrolled;
        std::string m_editName;
};

struct sortByIndex{
    bool operator()(const std::pair<CStudent, size_t > &x1, const std::pair<CStudent, size_t> &x2) const {
        return x1.second < x2.second;
    }
};

struct sortByCStudent {
    bool operator()(const std::pair<CStudent, size_t > &x1, const std::pair<CStudent, size_t> &x2) const {
        return x1.first < x2.first;
    }
};

struct CFilter {
        CFilter(): m_fromTime({0,0,0}),m_toTime({INT_MAX,INT_MAX,INT_MAX}),
                   m_fromEnrollment(0),m_toEnrollment(INT_MAX), m_enrollFilter(false),m_birthFilter(false) {}
        //case(0)
        CFilter &name(const std::string &name);
        //case(1)
        CFilter &bornBefore(const CDate &date);
        //case(2)
        CFilter &bornAfter(const CDate &date);

        //case(3)
        CFilter &enrolledBefore(int year);
        //case(4)
        CFilter &enrolledAfter(int year);

        std::set<std::string> m_names;
        CDate m_fromTime;
        CDate m_toTime;
        int m_fromEnrollment;
        int m_toEnrollment;
        bool m_enrollFilter;
        bool m_birthFilter;
};

class CSort {
    public:
        CSort() = default;

        CSort &addKey(ESortKey key, bool ascending);
        bool operator () (const CStudent &first, const CStudent &second) const;
        bool defaultVal() const { return m_sort.empty(); }

    private:
        std::vector<std::pair<ESortKey,bool>> m_sort;
};

class CStudyDept {
    public:
        CStudyDept() = default;

        bool addStudent(const CStudent &x);
        bool delStudent(const CStudent &x);

        std::list<CStudent> search(const CFilter &flt, const CSort &sortOpt) const;
        std::set<std::string> suggest(const std::string &name) const;

        friend std::ostream &operator << (std::ostream &os, const CStudyDept &data);

    private:
        bool isValid(const CFilter &flt, const CStudent &check) const;

        size_t m_count = 0;
        std::set<std::pair<CStudent,size_t>,sortByIndex> m_added;
        std::set<std::pair<CStudent,size_t>,sortByCStudent> m_students;
};

//-------------------------------------

bool CStudyDept::addStudent(const CStudent &x) {
    if(m_students.find({x,0}) == m_students.end()) {
        m_students.insert({x,m_count});
        m_added.insert({x,m_count});
        ++m_count;
        return true;
    } return false;
}

bool CStudyDept::delStudent(const CStudent &x) {
    auto it = m_students.find({x,0});
    if(it != m_students.end()) {
        size_t index = it->second;
        m_students.erase(it);
        m_added.erase({x,index});
        return true;
    } return false;
}

bool CStudyDept::isValid(const CFilter &flt, const CStudent &check) const {
    if(flt.m_names.empty()) {
        if(check.born() > flt.m_fromTime && check.born() < flt.m_toTime) {
            if(check.enrolled() > flt.m_fromEnrollment && check.enrolled() < flt.m_toEnrollment)
                return true;
            return false;
        } return false;
    } if(check.born() > flt.m_fromTime && check.born() < flt.m_toTime) {
        if(check.enrolled() > flt.m_fromEnrollment && check.enrolled() < flt.m_toEnrollment) {
            if(flt.m_names.count(check.editName()) == 0)
                return false;
            return true;
        } return false;
    } return false;
}

std::list<CStudent> CStudyDept::search(const CFilter &flt, const CSort &sortOpt) const {
    std::list<CStudent> result;
    for(const auto &item: m_added)
        if(isValid(flt,item.first))
            result.push_back(item.first);
    result.sort(sortOpt);
    return result;
}

std::set<std::string> CStudyDept::suggest(const std::string &name) const {
    if(name.empty()) return {};
    std::vector<std::string> search = stringToVector(name);
    std::set<std::string> res;
    for(const auto &it: m_students) {
        bool found = true;
        for(const auto &in:search) {
            if(it.first.editName().find(in) == std::string::npos) {
                found = false;
                break;
            }
        } if(found) res.insert(it.first.name());
    } return res;
}

std::ostream &operator << (std::ostream &os, const CStudent &data) {
    return os << "(" << data.m_name << ", " << data.m_born << ", " << data.m_enrolled << ")";
}

std::ostream &operator << (std::ostream &os, const CStudyDept &data) {
    os << "Name: " << std::endl;
    for(const auto &it: data.m_students) {
        os << "{" << it.first.editName() << "}" << it.first << std::endl;
    } os << "index: " << std::endl;
    for(const auto &it: data.m_added) {
        os << it.first << "[" << it.second << "]" << std::endl;
    } return os;
}

//-------------------------------------
CSort &CSort::addKey(ESortKey key, bool ascending) {
    m_sort.emplace_back(key,ascending);
    return *this;
}

bool CSort::operator()(const CStudent &first, const CStudent &second) const {
    for (const auto &it: m_sort) {
        switch (it.first) {
            case ESortKey::NAME:
                if(first.name() != second.name()) {
                    if(it.second) return first.name() < second.name();
                    return first.name() > second.name();
                } break;
            case ESortKey::BIRTH_DATE:
                if(first.born() != second.born()) {
                    if(it.second) return first.born() < second.born();
                    return first.born() > second.born();
                } break;
            case ESortKey::ENROLL_YEAR:
                if(first.enrolled() != second.enrolled()) {
                    if(it.second) return first.enrolled() < second.enrolled();
                    return first.enrolled() > second.enrolled();
                } break;
        }
    } return false;
}

//-------------------------------------
CFilter &CFilter::name(const std::string &name) {
    std::string add = editString(name);
    m_names.insert(add);
    return *this;
}

CFilter &CFilter::bornBefore(const CDate &date) {
    m_birthFilter = true;
    if(m_toTime > date)
        m_toTime = date;
    return *this;
}

CFilter &CFilter::bornAfter(const CDate &date) {
    m_birthFilter = true;
    if(m_fromTime < date)
        m_fromTime = date;
    return *this;
}

CFilter &CFilter::enrolledBefore(int year) {
    m_enrollFilter = true;
    if(m_toEnrollment > year)
        m_toEnrollment = year;
    return *this;
}

CFilter &CFilter::enrolledAfter(int year) {
    m_enrollFilter = true;
    if(m_fromEnrollment < year)
        m_fromEnrollment = year;
    return *this;
}

//-------------------------------------
std::vector<std::string> stringToVector(const std::string &name) {
    std::istringstream is(name);
    std::string word;
    std::vector<std::string> res;
    while (std::getline(is,word,' ')) {
        std::transform(word.begin(),word.end(),word.begin(),::tolower);
        word.insert(word.begin(),' ');
        word.push_back(' ');
        res.emplace_back(word);
    } return res;
}

std::string editString(const std::string &fullName) {
    std::istringstream is(fullName);
    std::set<std::string> edit;
    std::string word;
    while (std::getline(is,word,' ')) {
        std::transform(word.begin(),word.end(),word.begin(),::tolower);
        edit.insert(word);
    } std::ostringstream os;
    if(!edit.empty()) os << " ";
    for(const auto &it: edit) {
        os << it << " ";
    } return os.str();
}

bool CStudent::operator < (const CStudent &other) const {
    if(m_name != other.m_name)
        return m_name < other.name();
    if(m_born != other.m_born)
        return m_born < other.born();
    return m_enrolled < other.enrolled();
}

bool CStudent::operator == (const CStudent &other) const {
    return (m_name == other.m_name && m_born == other.m_born
            && m_enrolled == other.m_enrolled);
}

bool CStudent::operator != (const CStudent &other) const {
    return !(*this == other);
}


#ifndef __PROGTEST__

int main() {
    CStudyDept x0;
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2016)));
    assert (x0.addStudent(CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014)));
    assert (x0.addStudent(CStudent("John Taylor", CDate(1981, 6, 30), 2012)));
    assert (x0.addStudent(CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)));
    assert (x0.addStudent(CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1982, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 8, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 17), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2012)));
    assert (x0.addStudent(CStudent("Bond James", CDate(1981, 7, 16), 2013)));
    assert (x0.search(CFilter(), CSort()) == (std::list<CStudent>
            {
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, false)) == (std::list<CStudent>
            {
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    assert (x0.search(CFilter(),
                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                              ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));
    assert (x0.search(CFilter().name("james bond"),
                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                              ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012)
            }));
    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
            ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
            }));
    assert (x0.search(CFilter().name("james"), CSort().addKey(ESortKey::NAME, true)) == (std::list<CStudent>
            {
            }));
    assert (x0.suggest("peter") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor",
                    "Peter Taylor"
            }));
    assert (x0.suggest("bond") == (std::set<std::string>
            {
                    "Bond James",
                    "James Bond"
            }));
    assert (x0.suggest("peter joHn") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }));
    assert (x0.suggest("peter joHn bond") == (std::set<std::string>
            {
            }));
    assert (x0.suggest("pete") == (std::set<std::string>
            {
            }));
    assert (x0.suggest("peter joHn PETER") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }));
    assert (!x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
            ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
            }));
    assert (!x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
