#include <iostream>
#include <cstring>
#include <cstdlib>

class N {
public:
	explicit N(int value) : value(value) {}

	void setAnnotation(const char *str) {
		size_t len = std::strlen(str);
		std::memcpy(annotation, str, len);
	}

	// First virtual function in vtable — called in main via *(%edx)
	virtual int operator()(N *other) {
		return this->value + other->value;
	}

	int operator+(const N &other) const {
		return this->value + other.value;
	}

	int operator-(const N &other) const {
		return this->value - other.value;
	}

private:
	char annotation[100];
	int value;
};

int main(int argc, char **argv) {
	if (argc < 2) {
		std::exit(1);
	}

	N *obj1 = new N(5);
	N *obj2 = new N(6);

	obj1->setAnnotation(argv[1]);

	(*obj2)(obj1);

	return 0;
}
