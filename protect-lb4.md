![image](https://github.com/Xolozop/pr-codes/assets/50333438/b5054110-6720-47f5-9b4e-b5007c46a082)
![image](https://github.com/Xolozop/pr-codes/assets/50333438/c7e42c1c-c1c1-43cf-bf79-4dc009bf2b01)
```c++
#include <iostream>

using namespace std; 

class Animal {
public:
    Animal() { }
    virtual ~Animal() {}
    void voice() {
        printf("I am Animal\n");
    }
};

class Dog : public Animal {
public:
    Dog() : Animal() { }
    void voice() {
        Animal::voice();
        printf("gav\n");
    }
};

class Cat : public Animal {
public:
    Cat()  : Animal() { }

    void voice() {
        Animal::voice();
        printf("myau\n");
    }
};

class Frog : public Animal {
public:
    Frog() : Animal() { }
    void voice() {
        Animal::voice();
        printf("kva\n");
    }
};
```
![image](https://github.com/Xolozop/pr-codes/assets/50333438/3496a724-6d0a-4d9f-a8ab-0ece132e2091)
```c++
#include <iostream>
#include <math.h>

using namespace std; 

struct Point {
	int x;
	int y;
};

class Circle {
protected:
    Point k;
    float r;
public:
    Circle();
    Circle(Point k, float r) {
        this->k = k;
        this->r = r;
    }
    float calcDistance(Circle &cir) {
        float res = sqrt(pow(cir.k.x - this->k.x, 2) + pow(cir.k.y - this->k.y, 2));
        return round(res*10)/10;
    }
};

class Sector : public Circle {
public:
    int angle;
    Sector(Point k, float r, int angle) : Circle(k, r) {
        this->angle = angle;
    }
    float calcDistance(Sector &sec) {
        float res = sqrt(pow(sec.k.x - this->k.x, 2) + pow(sec.k.y - this->k.y, 2));
        return round(res*10)/10;
    }
}; 

int main() {
    Point p1 = {0, 0};
    Sector cir1 = Sector(p1, 4, 120);
    Point p2 = {3, 4};
    Sector cir2 = Sector(p2, 4, 90);
    printf("%f\n", cir1.calcDistance(cir2));
    return 0;
}
```
