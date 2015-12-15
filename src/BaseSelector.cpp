#define BaseSelector_cxx
#include "../interface/BaseSelector.h"

// start of query (executed on client)
void BaseSelector::Begin(TTree * /*tree*/)
{

   std::string option = GetOption();

}

// right after begin (executed on slave)
void BaseSelector::SlaveBegin(TTree * /*tree*/)
{

   std::string option = GetOption();

}


// for each entry of the TTree
bool BaseSelector::Process(Long64_t entry)
{

  n_entries++;
  if ((n_entries%10000) == 0) std::cout << "processing " << n_entries << " entry" << std::endl; 

  // set TTreeReader entry
  reader_.SetLocalEntry(entry);
  // update objects
  jets_.update();

  std::cout << jets_.at(0).pt() << std::endl;
  

  return true;
}


// all entries have been processed (executed in slave)
void BaseSelector::SlaveTerminate()
{

}

// last function called (on client)
void BaseSelector::Terminate()
{

}
