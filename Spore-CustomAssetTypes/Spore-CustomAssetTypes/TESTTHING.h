#pragma once

#include <Spore\BasicIncludes.h>
#include <Spore/UTFWin/TreeNode.h>

#define TESTTHINGPtr intrusive_ptr<TESTTHING>

class TESTTHING 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("TESTTHING");
	
	TESTTHING();
	~TESTTHING();

	int AddRef() override = 0;
	int Release() override = 0;
	virtual void* func_00659dd0() = 0;
    virtual int* func_00657300(int) = 0;
    virtual int func__getSomething() = 0;
    virtual int FUN_005f0e70(void) = 0;
    virtual int ReceiveMessage_func_00657b80(uint32_t, UTFWin::Message*) = 0;
    virtual void FUN_006582b0(int, int, int) = 0;
    virtual void FUN_00659df0() = 0;
    virtual void FUN_0065a950(char) = 0;
    virtual void FUN_006578d0(int) = 0;
    virtual void FUN_006599e0() = 0; //this is the important one we want to detour
    virtual void FUN_0065af50(char) = 0;
    virtual void FUN_00657dc0() = 0;
    virtual void FUN_00657420(uint32_t) = 0;
    virtual void FUN_006573b0() = 0;
    virtual void FUN_0065a030() = 0; //oh no this is what we want to detour, actually.
    virtual void FUN_00659380() = 0;
    virtual void FUN_00658400() = 0;
    virtual void FUN_00658990(char) = 0;
    virtual void FUN_00658c30(int) = 0;
    virtual void func30h_override(UTFWin::TreeNode*, int) = 0;

    char padding[0xff];

	/*
        013ffd5c 70 0e 5f 00     addr       FUN_005f0e70 a
        013ffd60 80 7b 65 00     addr       FUN_00657b80 a
        013ffd64 b0 82 65 00     addr       FUN_006582b0 a
        013ffd68 f0 9d 65 00     addr       FUN_00659df0 a
        013ffd6c 50 a9 65 00     addr       FUN_0065a950 a
        013ffd70 d0 78 65 00     addr       LAB_006578d0 a
        013ffd74 e0 99 65 00     addr       FUN_006599e0 a
        013ffd78 50 af 65 00     addr       LAB_0065af50 a
        013ffd7c c0 7d 65 00     addr       FUN_00657dc0 a
        013ffd80 20 74 65 00     addr       FUN_00657420 a
        013ffd84 b0 73 65 00     addr       LAB_006573b0 a
        013ffd88 30 a0 65 00     addr       FUN_0065a030 a
        013ffd8c 80 93 65 00     addr       FUN_00659380 a
        013ffd90 00 84 65 00     addr       FUN_00658400 a
        013ffd94 90 89 65 00     addr       FUN_00658990 a
        013ffd98 30 8c 65 00     addr       FUN_00658c30 a
        013ffd9c 20 2a 08 01     addr       UTFWin::TreeNode::func30h
*/
	
	//void* Cast(uint32_t type) const override;
};
