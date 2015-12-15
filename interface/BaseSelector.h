
#ifndef BaseSelector_h
#define BaseSelector_h

#include <vector>
#include <string>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>

#include "JetCollection.h"
#include "MET.h"
#include "GenParticleCollection.h"
#include "EventInfo.h"


class BaseSelector : public TSelector {

  public:

    long n_entries{0};

    // associated with a TTree
    TTreeReader reader_;

    // event content
    EventInfo eventInfo_;
    JetCollection jets_;
    MET met_;
    GenParticleCollection h_bosons_; 
    GenParticleCollection b_quarks_; // from higgs



  BaseSelector(TTree * /*tree*/ =0, bool isHH = false,
               std::vector<std::string> hlt_bits = {}) :
    eventInfo_(reader_, hlt_bits),
  	jets_(reader_),
    met_(reader_) 
    {
       
      if (isHH) {
        h_bosons_ = GenParticleCollection(reader_, "GenHiggsBoson");   
        b_quarks_ = GenParticleCollection(reader_, "GenBQuarkFromH"); 
      }
    }
  virtual ~BaseSelector() {}

  // TSelector functions
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();

};

#endif

#ifdef BaseSelector_cxx

// each new tree is opened
void BaseSelector::Init(TTree *tree)
{
  reader_.SetTree(tree);
}

// each new file is opened
Bool_t BaseSelector::Notify()
{

   return kTRUE;
}

#endif // #ifdef BaseSelector_cxx
