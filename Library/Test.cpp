#include "Test.hpp"
#include <Character/Character.hpp>
#include <Library/Library.hpp>
Test::Test(){}

Test::~Test(){}

void Test::output(){
	Library::init("my/data/folder");
	std::cout<<"This is a Test!"<<std::endl;
}
