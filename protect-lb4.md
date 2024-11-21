![image](https://github.com/Xolozop/pr-codes/assets/50333438/b5054110-6720-47f5-9b4e-b5007c46a082)

```c++
#include <iostream>

class Animal {
public:
	Animal() = default;
	virtual ~Animal() = default;
	virtual void voice() {
		std::cout << "I am Animal\n";
	}
};

class Dog : public Animal {
public:
	Dog() = default;
	virtual void voice() override {
		std::cout << "gav\n";
	}
};

class Cat : public Animal {
public:
	Cat() = default;
	virtual void voice() override {
		std::cout << "myau\n";
	}
};

class Frog : public Animal {
public:
	Frog() = default;
	virtual void voice() override {
		std::cout << "kva\n";
	}
};
```

![image](https://github.com/Xolozop/pr-codes/assets/50333438/c7e42c1c-c1c1-43cf-bf79-4dc009bf2b01)

```c++
#include <iostream>

class Animal {
public:
	Animal() = default;
	virtual ~Animal() = default;
	void voice() {
		std::cout << "I am Animal\n";
	}
};

class Dog : public Animal {
public:
	Dog() = default;
	void voice() {
		Animal::voice();
		std::cout << "gav\n";
	}
};

class Cat : public Animal {
public:
	Cat() = default;
	void voice() {
		Animal::voice();
		std::cout << "myau\n";
	}
};

class Frog : public Animal {
public:
	Frog() = default;
	void voice() {
		Animal::voice();
		std::cout << "kva\n";
	}
};
```

![image](https://github.com/Xolozop/pr-codes/assets/50333438/3496a724-6d0a-4d9f-a8ab-0ece132e2091)

```c++
#include <iostream>
#include <cmath>

struct Point {
	int x;
	int y;
};

class Circle {
protected:
	Point k;
	double r;
public:
	Circle() = default;
	Circle(Point k, double r) : k(k), r(r) {}
	[[nodiscard]] double calcDistance(const Circle &cir) const {
		double res = std::sqrt(pow(cir.k.x - this->k.x, 2) + pow(cir.k.y - this->k.y, 2));
		return std::round(res*10)/10;
	}
};

class Sector : public Circle {
public:
	double angle;
	Sector(Point k, double r, double angle) : Circle(k, r), angle(angle) {}
	[[nodiscard]] double calcDistance(const Sector &sec) const {
		double res = std::sqrt(pow(sec.k.x - this->k.x, 2) + pow(sec.k.y - this->k.y, 2));
		return std::round(res*10)/10;
	}
};

int main() {
	Point p1 {0, 0};
	Sector cir1 {p1, 4, 120};
	Point p2 {3, 4};
	Sector cir2 {p2, 4, 90};
	printf("%f\n", cir1.calcDistance(cir2));
	return 0;
}
```