#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <ctime>

#include <../src/window/window.hpp>

class Complex {
public:
    Complex(const int _real, const int _image): real(_real), image(_image) {}
    Complex(Complex &obj) : real(obj.getreal()), image (obj.getimage()) {}

    void detail () const {
        std::cout << real << " + i" << image << std::endl;
    }

    int abs() const {return real*real + image*image;};
    int getreal () const {return real;};
    int getimage () const {return image;};

    void setreal (int _real) {real = _real;}

    // Operators
    Complex operator + (Complex const &obj) {
        Complex res (real+obj.real, image+obj.image);
        return res;
    }

    void operator = (const Complex &obj) {
        this->real = obj.real;
        this->image = obj.image;
    }
    void operator += (Complex const &obj) {
        real += obj.real;
        image += obj.image;
    }

    bool operator == (Complex const &obj) {
        return real == obj.real && image == obj.image;
    }

    int operator * (Complex const &obj) {
        return (real*obj.real + image*obj.image);
    }

    friend std::ostream& operator<<(std::ostream &out, const Complex &obj);

protected:
    int real;
    int image;
};

std::ostream& operator<<(std::ostream &out, const Complex &obj) {
    out << obj.getreal() << " + i" << obj.getimage();
    return out;
}

u32 cruelEngine::Window::count = 0;
bool cruelEngine::Window::glfw_inited = false;

struct every {
    public:
    
    every (std::string _str) : str (std::move(_str)) {
        std::cout << str << std::endl;
    }
    ~every () { std::cout << "my life ends: " + str << std::endl;}

    std::string str;
};

int main(int argc, char const *argv[])
{
    Complex a (5,5);
    Complex b (a);
    b.setreal(1);

    a = b;
    if (a == b) {
        std::cout << "That's amazing" << std::endl;
    }

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    Complex c = a+b;

    c += a;
    c.detail();
    std::cout << "(" << a.getreal() << " + i" << a.getimage() << ") * " << "(" << b.getreal() << " + i" << b.getimage() << ") = ";
    std::cout << a * b << std::endl;

    std::vector<std::shared_ptr<every>> everylist{};

    everylist.push_back(std::make_shared<every>("hello"));
    everylist.push_back(std::make_shared<every>("world"));


    cruelEngine::Window window;

    clock_t start_time = clock();

    while (!glfwWindowShouldClose(window.window)){
        glfwPollEvents();
    }

    std::cout << "MainWindow Running time: " << (clock() - start_time) / 1e6 << std::endl;

    everylist.clear();

    return 0;
}