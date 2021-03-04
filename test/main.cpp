#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <ctime>

// #include <../src/window/window.hpp>

class vehical {
    public:
    vehical() {}
    ~vehical() {}

    void set_brand(std::string _brand) {brand = _brand;}
    void set_name(std::string _name) {name = _name;} 

    std::string get_brand() {return brand;}
    std::string get_name() {return name;}
    std::string get_type() {return type;}

    protected:

    std::string name;
    std::string brand;
    std::string type;
};

class car :public vehical {
    public:
    car () : vehical() 
    {
        type = "car";
    }
};

class cart :public vehical {
    public:
    cart() : vehical()
    {
        type = "cart";
    }
};

int main(int argc, char const *argv[])
{
    std::vector<std::unique_ptr<vehical>> port;
    port.emplace_back(std::make_unique<car>());
    port.emplace_back(std::make_unique<cart>());

    for (auto &a : port )
    {
        std::cout << "vechicle type: " << a->get_type() << std::endl;
    }
    return 0;
}