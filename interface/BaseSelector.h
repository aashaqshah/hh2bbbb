
#ifndef BaseSelector_h
#define BaseSelector_h

#include <vector>
#include <string>
#include <memory>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>

#include "BaseOperator.h"




template <class EventClass> class BaseSelector : public TSelector {

  public:

  long n_entries{0};

  // associated with a TTree
  TTreeReader reader_;

  // event class 
  EventClass ev_; 

  // vector of operations
  std::vector<std::unique_ptr<BaseOperator<EventClass>>> ops_;


  BaseSelector(TTree * /*tree*/ =0, std::vector<std::string> hlt_bits = {}) :
    ev_(reader_, hlt_bits)
    {
       
    }
  virtual ~BaseSelector() {}

  virtual bool addOperator( BaseOperator<EventClass> * op ) { 
    ops_.emplace_back(op);
    return true;
  }

  // TSelector functions
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual bool    Notify();
  virtual bool    Process(Long64_t entry);
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();

};

#endif

// each new tree is opened
template <class EventClass> void BaseSelector<EventClass>::Init(TTree *tree)

{
  reader_.SetTree(tree);
}

// each new file is opened
template <class EventClass> bool BaseSelector<EventClass>::Notify()
{

   return kTRUE;
}

/// start of query (executed on client)
template <class EventClass> void BaseSelector<EventClass>::Begin(TTree * /*tree*/)
{

   std::string option = GetOption();

}

// right after begin (executed on slave)
template <class EventClass> void BaseSelector<EventClass>::SlaveBegin(TTree * /*tree*/)
{

   std::string option = GetOption();

}


// for each entry of the TTree
template <class EventClass> bool  BaseSelector<EventClass>::Process(Long64_t entry)
{

  n_entries++;
  if ((n_entries%10000) == 0) std::cout << "processing " << n_entries << " entry" << std::endl; 

  // set TTreeReader entry
  reader_.SetLocalEntry(entry);
  // update event objects
  ev_.update();

  for (auto & op : ops_) {
    if (!op->process(ev_)) return false; 
  }

  return true;
}


// all entries have been processed (executed in slave)
template <class EventClass> void BaseSelector<EventClass>::SlaveTerminate()
{


   for (auto & op : ops_) {                
     op->output(std::cout); 
   }

}

// last function called (on client)
template <class EventClass> void BaseSelector<EventClass>::Terminate()
{

}
