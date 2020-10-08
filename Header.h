#pragma once
class MyClass
{
public:
	MyClass();
	~MyClass();

private:
	int a;
	int b;
};

MyClass::MyClass()
{
	a = 3;
}

MyClass::~MyClass()
{
}