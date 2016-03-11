#include <stdio.h>

#include <libclientserver.h>

class MyClass
{
	public:
		MyClass()
		{
			printf("Init\n");
		}
		
		~MyClass()
		{
			printf("Destroy\n");
		}
		
		void Func()
		{
			printf("Func\n");
		}
};

int main(int argc, char **argv)
{
	LazyScopedPtr<MyClass> x([]() { return new MyClass(); } );
	printf("Before\n");
	x->Func();
	printf("After\n");
	
	return 0;
}

