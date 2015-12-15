
#pragma once

#include <vector>
#include <memory>

#include <TTreeReader.h>
#include <TTreeReaderValue.h>

#include "mut_framework/mut_dataformats/interface/MET.h"

typedef ROOT::Math::PtEtaPhiMVector PtEtaPhiMVector;
typedef ROOT::Math::PtEtaPhiEVector PtEtaPhiEVector;

class MET : public mut::MET {

  public:
    
    // for reading from VHBB_HEPPY
    TTreeReaderValue<float> * pt_tr_;
    TTreeReaderValue<float> * eta_tr_;
    TTreeReaderValue<float> * phi_tr_;
    TTreeReaderValue<float> * mass_tr_;
    
    MET() : 
     pt_tr_(nullptr),
     eta_tr_(nullptr),
     phi_tr_(nullptr),
     mass_tr_(nullptr) {}

    MET(TTreeReader & reader) : 
     pt_tr_(   new TTreeReaderValue<float>(reader, "met_pt")),
     eta_tr_(  new TTreeReaderValue<float>(reader, "met_eta")),
     phi_tr_ ( new TTreeReaderValue<float>(reader, "met_phi")),
     mass_tr_( new TTreeReaderValue<float>(reader, "met_mass"))
    {
    } 

    ~MET() {}
    // inherit constructors
    using mut::MET::MET;    

    void update() {

      PtEtaPhiMVector lv(**pt_tr_,
                         **eta_tr_,
                         **phi_tr_,
                         **mass_tr_); 
      // need to call explicit = operator from parent
      PtEtaPhiEVector::operator=(lv); 

    }
};

