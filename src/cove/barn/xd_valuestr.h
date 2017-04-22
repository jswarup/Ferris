//_________________________________________________ xd_valuestr.h ____________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________


class Xd_ValueStr
{ 
    std::string         str;
    bool                valid;

public:
    Xd_ValueStr() 
        : str(), valid( false) 
    {}

    explicit Xd_ValueStr( const std::string &v) 
        : str(v), valid(true) 
    {}

    operator const char *()
    {
        return str.c_str();
    }

    operator bool()
    {
        bool    t;
        return (valid && (std::istringstream(str) >> t)) ? t : false;
    }

    operator short()
    {
        short t;
        return (valid && (std::istringstream(str) >> t)) ? t : 0;
    }

    operator unsigned short()
    {
        unsigned short t;
        return (valid && (std::istringstream(str) >> t)) ? t : 0;
    }

    operator int()
    {
        int t;
        return (valid && (std::istringstream(str) >> t)) ? t : 0;
    }

    operator unsigned int()
    {
        unsigned int t;
        return (valid && (std::istringstream(str) >> t)) ? t : 0;
    }

    operator long()
    {
        long t;
        return (valid && (std::istringstream(str) >> t)) ? t : 0;
    }

    operator unsigned long()
    {
        unsigned long t;
        return (valid && (std::istringstream(str) >> t)) ? t : 0;
    }

    operator float()
    {
        float t;
        return (valid && (std::istringstream(str) >> t)) ? t : 0;
    }

    operator double()
    {
        double t;
        return (valid && (std::istringstream(str) >> t)) ? t : 0;
    }

    operator long double()
    {
        long double t;
        return (valid && (std::istringstream(str) >> t)) ? t : 0;
    }

};

//_____________________________________________________________________________________________________________________________

class Xd_ValueStrMap
{
private:
    std::map<std::string, std::string>                  _map;
    std::map<std::string, std::vector<std::string> >    _append_map;
    std::set<std::string>                               _user_set;

public:

    Xd_ValueStrMap() : _map() {}

    const std::string &operator[](const std::string &d) const
    {
        std::map<std::string, std::string>::const_iterator it = _map.find(d);
        static const std::string empty = "";
        return (it != _map.end()) ? it->second : empty;
    }

    std::string &operator[](const std::string &d)
    {
        return _map[d];
    }

    bool is_set(const std::string &d) const
    {
        return _map.find(d) != _map.end();
    }

    bool is_set_by_user(const std::string &d) const
    {
        return _user_set.find(d) != _user_set.end();
    }

    void is_set_by_user(const std::string &d, bool yes)
    {
        if (yes)
        {
            _user_set.insert(d);
        }
        else
        {
            _user_set.erase(d);
        }
    }

    Xd_ValueStr get(const std::string &d) const
    {
        return (is_set(d)) ? Xd_ValueStr((*this)[d]) : Xd_ValueStr();
    }

    typedef std::vector<std::string>::iterator iterator;
    typedef std::vector<std::string>::const_iterator const_iterator;

    std::vector<std::string> &all(const std::string &d)
    {
        return _append_map[d];
    }

    const std::vector<std::string> all(const std::string &d) const
    {
        static const std::vector<std::string> empty;
        return (_append_map.find(d) == _append_map.end()) ?
            empty :
            _append_map.at(d);
    }

    
};

//_____________________________________________________________________________________________________________________________
