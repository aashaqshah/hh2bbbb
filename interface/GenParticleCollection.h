
#pragma once

#include <vector>
#include <algorithm>
#include <memory>

#include <TTreeReader.h>
#include <TTreeReaderArray.h>

#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"

#include "mut_framework/mut_dataformats/interface/Candidate.h"

typedef ROOT::Math::PtEtaPhiMVector PtEtaPhiMVector;

class GenParticleCollection : public std::vector<mut::Candidate> {

  public:

    // TTreeReaderValues for reading from VHBB_HEPPY
    TTreeReaderArray<float> * gen_particle_pts_;
    TTreeReaderArray<float> * gen_particle_etas_;
    TTreeReaderArray<float> * gen_particle_phis_;
    TTreeReaderArray<float> * gen_particle_masss_;

    GenParticleCollection() :
      gen_particle_pts_(nullptr),
      gen_particle_etas_(nullptr),
      gen_particle_phis_(nullptr),
      gen_particle_masss_(nullptr)
    {
    }
    
    GenParticleCollection(TTreeReader & reader, std::string obj_name) : 
     gen_particle_pts_(  new TTreeReaderArray<float>(reader, (obj_name+"_pt").c_str())),
     gen_particle_etas_( new TTreeReaderArray<float>(reader, (obj_name+"_eta").c_str())),
     gen_particle_phis_( new TTreeReaderArray<float>(reader, (obj_name+"_phi").c_str())),
     gen_particle_masss_(new TTreeReaderArray<float>(reader, (obj_name+"_mass").c_str()))
    {
    } 

    ~GenParticleCollection() {}
    
    void update() {

      // delete previous elements
      this->clear();

      // jets ordered in pt
      std::vector<int> order(gen_particle_pts_->GetSize());
      std::iota(order.begin(), order.end(), 0); 
      auto comparator = [&](int a, int b){ return (*gen_particle_pts_)[a] > (*gen_particle_pts_)[b]; };
      std::sort(order.begin(), order.end(), comparator);

      // iterate over jets in order 
      for (const auto & i : order) {
        
        PtEtaPhiMVector gen_particle_lv((*gen_particle_pts_)[i],
                                        (*gen_particle_etas_)[i],
                                        (*gen_particle_phis_)[i],
                                        (*gen_particle_masss_)[i]); 
        // new element using constructor from PtEtaPhiM
      	this->emplace_back(gen_particle_lv);
        
      }
    }
};

