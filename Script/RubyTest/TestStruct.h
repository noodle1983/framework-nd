#ifndef TESTSTRUCT_H
#define TESTSTRUCT_H

#include <string>
#include <ruby.h>
#include <iostream>

struct SubTestStruct
{
    int intM;
};

struct TestStruct
{
    int intM;
    std::string strM;
    SubTestStruct subStructM;
};

VALUE cSubTestStruct;
VALUE cTestStruct;
VALUE setInt(VALUE self, VALUE theValue)
{
    TestStruct * st;
    Data_Get_Struct(self, TestStruct, st);
    st->intM = NUM2INT(theValue);
    std::cout << "setInt:" << st->intM<< std::endl;
    return theValue;
}

VALUE getInt(VALUE self)
{
    TestStruct * st;
    Data_Get_Struct(self, TestStruct, st);
    std::cout << "getInt:" << st->intM << std::endl;
    return INT2NUM(st->intM);
}

void Init_TestStruct()
{
    /*
    cSubTestStruct = rb_define_class("SubTestStruct", rb_cData);
    rb_define_method(cSubTestStruct, "setI",  (VALUE (*)(...))setInt<SubTestStruct>, 1);
    rb_define_method(cSubTestStruct, "getI",  (VALUE (*)(...))getInt<SubTestStruct>, 0);
    */

    cTestStruct = rb_define_class("TestStruct", rb_cData);
    rb_define_method(cTestStruct, "setI",  (VALUE (*)(...))setInt, 1);
    rb_define_method(cTestStruct, "getI",  (VALUE (*)(...))getInt, 0);
    TestStruct testSt;
    testSt.intM = 1;
    VALUE rTestSt = Data_Wrap_Struct(cTestStruct, 0, 0, &testSt);
    rb_funcall(Qnil, rb_intern("test"), 1, rTestSt);


}




#endif /* TESTSTRUCT_H */

