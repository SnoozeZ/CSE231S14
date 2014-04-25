//https://sites.google.com/site/arnamoyswebsite/Welcome/updates-news/llvmpasstoinsertexternalfunctioncalltothebitcode
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#define HELPER_FUNCTIONS 160

using namespace llvm;
using namespace std;

namespace {
  struct DynamicInstructionCount : public ModulePass {
    static char ID;
    llvm::Function* hookCount;		//Required that you have a llvm Function to call
    llvm::Function* hookPrint;

    DynamicInstructionCount() : ModulePass(ID) {hookCount = NULL; hookPrint = NULL;}

    virtual bool runOnModule(Module &M){
                Constant *hookFunc;
                StringRef countingFunctionName;
                StringRef printFunctionName;

                hookFunc = M.getOrInsertFunction("_Z8countingPc",Type::getVoidTy(M.getContext()),Type::getInt8PtrTy(M.getContext()),(Type*)0);
                if (hookFunc) {
					hookCount= cast<Function>(hookFunc);
					hookCount->setCallingConv(CallingConv::C);
					Module::iterator F = M.begin();
					for (int i = 0 ; i < HELPER_FUNCTIONS ; i++)
						F++;
					for(Module::iterator E = M.end(); F!= E; ++F) {
						if (!F->getName().equals(printFunctionName) && !F->getName().equals(countingFunctionName))
							for(Function::iterator BB = F->begin(), E = F->end(); BB != E; ++BB) {
								DynamicInstructionCount::runOnBasicBlock(BB);
							}
					}
                }
                hookFunc = M.getOrInsertFunction("_Z5printv",Type::getVoidTy(M.getContext()),(Type*)0);
                if (hookFunc) {
                	hookPrint = cast<Function>(hookFunc);
                	Module::iterator F = M.end();
                	F--;
                	Function::iterator BB = F->end();
                	BB--;
                	BasicBlock::iterator BI = BB->end();
                	if (BI != BB->begin())
                		BI--;
                	Instruction *CI = dyn_cast<Instruction>(BI);
                	Instruction *newInst = CallInst::Create(hookPrint, "");
                	BB->getInstList().insert(CI, newInst);
                }

                return false;
            }

    virtual bool runOnBasicBlock(Function::iterator &BB) {
    			IRBuilder<> builder(BB);
    			string result = "";
                for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI) {
                	Instruction *CI = dyn_cast<Instruction>(BI);
                	result.append(CI->getOpcodeName());
                	result.append(" ");
                }
                BasicBlock::iterator BE = BB->end();
                if (BE != BB->begin())
                	BE--;
                builder.SetInsertPoint(BB,BE);
                Value* myStr = builder.CreateGlobalStringPtr(result,"myStr");
                builder.CreateCall(hookCount,myStr);
                return true;
    }
  };
}

char DynamicInstructionCount::ID = 0;
static RegisterPass<DynamicInstructionCount> X("dynamicInstructionCount", "DynamicInstructionCount", false, false);

